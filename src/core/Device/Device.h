#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "Graphics/RenderPipeline.h"
namespace gpc{
	struct Dim3 {
		uint32_t x;
		uint32_t y;
		uint32_t z;
		operator bool() const{
			return x == 0 || y == 0 || z == 0;
		}
	};
	struct Tid3{
		uint32_t x;
		uint32_t y;
		uint32_t z;
	};
	
    class ITaskIssue {
    public:
		virtual void     init(uint32_t size) = 0;
        virtual uint32_t doTask(Tid3 const& tid) = 0;
		virtual Dim3     getTaskDim() const = 0;
	};
    
    class Device {
        class ExecuteCore {
        public:
			ExecuteCore(uint32_t id, Device* device,std::mutex* mutex, std::condition_variable *variable );
			~ExecuteCore();
			uint32_t getTid() const;
			uint32_t work(std::mutex* mutex, std::condition_variable * variable);
		protected:
			friend class Device;
            uint32_t m_id;
            Device*  m_device;
            std::thread m_thread;
        };
	public:
        Device();
		void bindTaskIssue(std::shared_ptr<ITaskIssue> taskIssue);
		void wait();
		void stop();
		uint32_t getExecuteCoreCount() const;
	private:
		void _WaitAllExecuteCoreWorkDone();
		void _NotifyDeviceWorkDone();
		bool _IsStop() const;
		uint32_t _DecreaseRunCount();
		friend class ExecuteCore;
		uint32_t _DoWork(Tid3 const& tid);
		bool _IdToTid3(uint32_t id, Tid3& tid) const;
	private:
		uint32_t                    m_bStop : 1;
		std::atomic<uint32_t>       m_coreRunCount;
        std::shared_ptr<ITaskIssue> m_taskIssues;
		std::condition_variable     m_wakeUpCore;
		std::condition_variable     m_wakeUpMainThread;
		std::mutex                  m_sleepCore;
		std::mutex                  m_sleepMainThread;
		std::vector<std::shared_ptr<ExecuteCore>> m_executeCore;

    };
}
