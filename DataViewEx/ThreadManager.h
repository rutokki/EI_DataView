#pragma once
#include "WThread.h"
class ThreadManager
{
public:
	static ThreadManager& GetInstance()
	{
		static ThreadManager instance;
		return instance;
	}
	ThreadManager(const ThreadManager&) = delete;
	ThreadManager& operator=(const ThreadManager&) = delete;
	ThreadManager(ThreadManager&&) = delete;
	ThreadManager& operator=(ThreadManager&&) = delete;

	bool CreateThread(const std::string& name);
	bool RemoveThread(const std::string& name);
	bool EnqueueTask(const std::string& name, std::function<void()> task);
	void ClearAllThreads();
private:
	ThreadManager();
	virtual ~ThreadManager();

	std::unordered_map<std::string, std::unique_ptr<WThread>> managerThreads;
	std::mutex managerMutex;
};

