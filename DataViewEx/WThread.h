#pragma once
#include "stdafx.h"
class WThread
{
public:
	WThread();
	~WThread();

	void Start();
	void Stop();

	void EnqueueTask(std::function<void()> task);
	void DequeueTask();
	size_t GetQueueSize();
private:
	void Run();

	std::thread m_thread;
	std::queue<std::function<void()>> m_tasks;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::atomic<bool> m_running;
};

