#ifndef thread_pool_h
#define thread_pool_h

#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <functional>
#include "SafeQueue.hpp"


class Threadpool {

	using Task = std::function<void()>;

	class ThreadWorker
	{
		Threadpool* _threadpool;
	public:
		ThreadWorker(Threadpool* threadpool)
		{
			this->_threadpool = threadpool;
		}
		void operator()()  // overload () anrop
		{
			Task task;
			bool dequeued;
			while (!_threadpool->_stopping)
			{
				{
					std::unique_lock<std::mutex> lock(_threadpool->_mutex);
					while(_threadpool->_workQueue.empty()) // borde ändra till while
					{
						_threadpool->_cv.wait(lock);
					}
					dequeued = _threadpool->_workQueue.dequeue(task);
				}
				if (dequeued)
				{
					task();
				}
			}
		}
	};

	bool _stopping = false;
	std::vector<std::thread> _threadpool;
	SafeQueue<Task> _workQueue;
	std::mutex _mutex;
	std::condition_variable _cv;
	
public:

	Threadpool(size_t size)
	{
		_threadpool = std::vector<std::thread>(size);
		for (size_t i = 0; i < size; ++i)
		{
			_threadpool[i] = std::thread(ThreadWorker(this));
		}
	}
	~Threadpool()
	{
		_stopping = true;
		_cv.notify_all();
		for (size_t i = 0; i < _threadpool.size(); ++i)
		{
			_threadpool[i].join();
		}
	}

	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args)
	{
		auto task = std::make_shared<std::packaged_task<decltype(f(args...)) ()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		std::function<void()> wrapper_func = [task]() {
			(*task)();
		};

		_workQueue.enqueue(wrapper_func);
		_cv.notify_one();
		return task->get_future();
	}
};

#endif