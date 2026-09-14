#include "stdafx.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
	ClearAllThreads();
}

bool ThreadManager::CreateThread(const std::string& name)
{
	std::lock_guard<std::mutex> lock(managerMutex);

	if (managerThreads.find(name) == managerThreads.end())
	{
		auto thread = std::make_unique<WThread>();
		managerThreads[name] = std::move(thread);
		return true;
	}
	else {
		return false;
	}
}

bool ThreadManager::RemoveThread(const std::string& name)
{
	std::unique_ptr<WThread> threadToRemove;
	std::lock_guard<std::mutex> lock(managerMutex);
	auto thread = managerThreads.find(name);
	if (thread == managerThreads.end()) {
		return false;
	}
	threadToRemove = std::move(thread->second);
	managerThreads.erase(thread);

	if (threadToRemove) {
		threadToRemove->Stop();
	}

	return true;
}

bool ThreadManager::EnqueueTask(const std::string& name, std::function<void()> task)
{
	std::lock_guard<std::mutex> lock(managerMutex);
	auto thread = managerThreads.find(name);
	if (thread == managerThreads.end()) {
		return false;
	}
	thread->second->EnqueueTask(task);
	return true;
}

void ThreadManager::ClearAllThreads()
{
	std::vector<std::unique_ptr<WThread>> threadsToStop;
	{
		std::lock_guard<std::mutex> lock(managerMutex);
		for (auto& pair : managerThreads) {
			threadsToStop.push_back(std::move(pair.second));
		}
		managerThreads.clear();
	}
	for (auto& thread : threadsToStop) {
		if (thread) {
			thread->Stop();
		}
	}
}
