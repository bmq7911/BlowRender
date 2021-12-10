#include "Device.h"

namespace gpc {
	Device::ExecuteCore::ExecuteCore( uint32_t id, Device* device, std::mutex *mutex, std::condition_variable * variable) 
		: m_id( id)
		, m_device( device)
		, m_state( kStart)
	{
		m_thread = std::move(std::thread(&Device::ExecuteCore::work, this, mutex,variable));
	}
	
	Device::ExecuteCore::~ExecuteCore() {
	
	}
	
	uint32_t Device::ExecuteCore::getTid() const {
		return m_id;
	}

	uint32_t Device::ExecuteCore::work(std::mutex* mutex, std::condition_variable* variable) {
		while (!m_device->_IsStop()) {
			m_state = Device::ExecuteCore::kStart;
			uint32_t id = m_id;
			Tid3 tid;
			while ( m_device->_IdToTid3( id, tid)) {
						m_state = State::kRun;
						m_device->_DoWork(tid);
						id += m_device->getExecuteCoreCount();
			}
			uint32_t count = m_device->_DecreaseRunCount();
			if (m_device->_IsStop()) {
				break;
			}
			m_state = State::kWait;
			if (0 == count) {
				m_device->_NotifyDeviceWorkDone(); /// 通知主线程，任务已执行完成
			}
			std::unique_lock<std::mutex> locker(*mutex);
			variable->wait(locker);
		}
		m_device->_NotifyDeviceWorkDone();
		m_state = State::kStop;
		return 0;
	}

	Device::ExecuteCore::State Device::ExecuteCore::getState() const {
		return m_state;
	}

    Device::Device()
		: m_bStop( false )
	{
		uint32_t size = std::thread::hardware_concurrency();
		m_coreRunCount = size;
		for (size_t i = 0; i < size; ++i) {
			m_executeCore.push_back(std::make_shared<Device::ExecuteCore>( i, this,&m_sleepCore,&m_wakeUpCore));
		}
	}
	
	void Device::bindTaskIssue(std::shared_ptr<ITaskIssue> taskIssue) {
		if(m_taskIssues ){
			_WaitAllExecuteCoreWorkDone();
		}
		m_taskIssues = taskIssue;
		m_coreRunCount = m_executeCore.size();
		m_wakeUpCore.notify_all();
	}

	void Device::wait() {
		_WaitAllExecuteCoreWorkDone();
	}
	
	void Device::stop() {
		m_bStop = true;
		m_wakeUpCore.notify_all();
		_WaitAllExecuteCoreWorkDone();
	}

	uint32_t Device::getExecuteCoreCount() const {
		return m_executeCore.size();
	}

	void Device::_WaitAllExecuteCoreWorkDone() {
		if (0 != m_coreRunCount) {
			std::unique_lock<std::mutex> locker(m_coreWait);
			m_wakeUpWait.wait(locker);
		}
	}
	
	void Device::_NotifyDeviceWorkDone() {
		m_wakeUpWait.notify_one();
	}

	bool Device::_IsStop() const{
		return m_bStop;
	}

	uint32_t Device::_DecreaseRunCount() {
		--m_coreRunCount;
		return m_coreRunCount;
	}

	uint32_t Device::_DoWork(Tid3 const& tid) {
		return m_taskIssues->doTask(tid);
	}

	bool Device::_IdToTid3(uint32_t id, Tid3 & tid) const{
		if (m_taskIssues) {
			Dim3 dim = m_taskIssues->getTaskDim();
			size_t size = dim.x * dim.y * dim.z;
			if (id < size) {
				{
					tid.z = id % dim.z;
					id =  id / dim.z;
					tid.x = id / dim.y;
					tid.y = id % dim.y;
					LOG_TRACE("Tid", "id = %d tid = [%d,%d,%d]", tid.x, tid.y, tid.z);
					return true;
				}
			}
		}
		return false;
	}
}