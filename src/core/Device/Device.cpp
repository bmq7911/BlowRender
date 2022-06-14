#include "Device.h"

namespace gpc {
	Device::ExecuteCore::ExecuteCore( uint32_t id, Device* device, std::mutex *mutex, std::condition_variable * variable) 
		: m_id( id)
		, m_device( device)
	{
		m_thread = std::move(std::thread(&Device::ExecuteCore::work, this, mutex,variable));
	}
	
	Device::ExecuteCore::~ExecuteCore() {
		m_thread.join( );
	}
	
	uint32_t Device::ExecuteCore::getTid() const {
		return m_id;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="mutex"> Device::m_sleepCore</param>
	/// <param name="variable">Device::m_wakeUpCore</param>
	/// <returns></returns>
	uint32_t Device::ExecuteCore::work(std::mutex* mutex, std::condition_variable* variable) {
		while (!m_device->_IsStop()) {
			uint32_t id = m_id;
			Tid3 tid;
			while ( m_device->_IdToTid3( id, tid)) {
				m_device->_DoWork(tid);
				id += m_device->getExecuteCoreCount();
			}

			if (m_device->_IsStop()) {
				m_device->_DecreaseRunCount();
				break;
			}
			/// <summary>
			///  thread 2 be suspend in there,and next time don't have change to be wake up
			/// </summary>
			/// <param name="mutex"></param>
			/// <param name="variable"></param>
			/// <returns></returns>
			std::unique_lock<std::mutex> locker(*mutex);
			uint32_t count = m_device->_DecreaseRunCount();
			if (0 == count) {
				m_device->_NotifyDeviceWorkDone(); /// 通知主线程，任务已执行完成
			}
			variable->wait(locker);
		}
		m_device->_DecreaseRunCount( );
		m_device->_NotifyDeviceWorkDone();
		return 0;
	}


    Device::Device()
		: m_bStop( false )
		, m_bWait( true )
	{
		uint32_t size = std::thread::hardware_concurrency();
		m_coreRunCount = size;
		for (size_t i = 0; i < size; ++i) {
			m_executeCore.push_back(std::make_shared<Device::ExecuteCore>( uint32_t(i), this,&m_sleepCore,&m_wakeUpCore));
		}
	}
	
	void Device::bindTaskIssue(std::shared_ptr<ITaskIssue> taskIssue) {
		if(m_taskIssues ){
			_WaitAllExecuteCoreWorkDone();
		}
		m_taskIssues = taskIssue;
		m_coreRunCount = uint32_t(m_executeCore.size());
		m_taskIssues->init( m_coreRunCount );
		m_wakeUpCore.notify_all();
		//std::cout <<"&&&&&&" << std::endl;
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
		return static_cast<uint32_t>(m_executeCore.size());
	}

	void Device::_WaitAllExecuteCoreWorkDone() {
		if (0 != m_coreRunCount) {
			/// <summary>
			/// 最好还是一把锁对应一个条件变量,而非一把锁对应多个条件变量
			/// </summary>
			std::unique_lock<std::mutex> locker(m_sleepMainThread);
			///std::unique_lock<std::mutex> locker(m_sleepCore);
			m_wakeUpMainThread.wait(locker, [this]()-> bool {
				return 0 == m_coreRunCount;
			});
		}
		m_taskIssues = nullptr;
	}
	
	void Device::_NotifyDeviceWorkDone() {
		{
			std::unique_lock<std::mutex> locker(m_sleepMainThread);
		}
		m_wakeUpMainThread.notify_one( );
		//std::cout <<"******" << std::endl;
	}

	bool Device::_IsStop() const{
		return m_bStop;
	}

	uint32_t Device::_DecreaseRunCount() {
		m_coreRunCount--;
		std::cout << m_coreRunCount << std::endl;
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
					//std::cout << tid.x << ","<<tid.y << ","<< tid.z << std::endl;
					return true;
				}
			}
		}
		return false;
	}
}