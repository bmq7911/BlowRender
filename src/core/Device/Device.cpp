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
			// 这里的wait所有工作线程把任务完成,只有最后一条工作线程完成任务后才会进入到这里
			// 这时后最后一条工作线程还未wait我们就调用notify_all,导致最后一条工作线程无法工作,任务无法完成而阻塞
			_WaitAllExecuteCoreWorkDone();
		}
		m_taskIssues = taskIssue;
		m_coreRunCount = uint32_t(m_executeCore.size());
		m_taskIssues->init( m_coreRunCount );
		{
			// 这里加锁原因也是一样,可能先唤醒后阻塞
			// 这里一定是工作线程先拿到锁(且已经拿到锁了),我在这里等待工作线程wait时把锁释放,当主线程在这里拿到锁的时候
			// 工作线程就已经wait了,我们的notify_all能正常唤醒所有工作线程,所以notify前面必须加锁
			std::unique_lock<std::mutex> locker(m_sleepCore);
		}
		m_wakeUpCore.notify_all(); /// there may be a thread not wait
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
			// 当我们准备取唤醒主线程时,有可能主线程还未进入wait状态,导致先唤醒后阻塞
			// 这时候我们加锁,当主线程先进入时,就必然会wait然后当前线程才能notify,这时正常notify
			// 当当前线程先获取到锁时,那么主线需要阻塞的条件也不在成立,因此主线程获取到锁的时候wait条件不成立也就不在wait
			std::unique_lock<std::mutex> locker(m_sleepMainThread);
			// 这里我们必须释放锁,如果在这里notify主线程,主线程被唤醒但是却获取不到锁,就还是会死锁
		}
		m_wakeUpMainThread.notify_one( );
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
					//std::cout << tid.x << ","<<tid.y << ","<< tid.z << std::endl;
					return true;
				}
			}
		}
		return false;
	}
}