#include "stdafx.h"
#include "WThread.h"

WThread::WThread()
{
	m_running = false;
}
//스레드 중지 및 종료
WThread::~WThread()
{
	Stop();
}
// 스레드 실행
void WThread::Start()
{
	if (m_running) return; // 수정: 이미 실행 중이면 리턴
	m_running = true;
	m_thread = std::thread(&WThread::Run, this);
}

// 수정된 Stop
void WThread::Stop()
{
	if (!m_running) return;
	m_running = false;     // 수정: 종료 플래그 설정
	m_cv.notify_one();
	if (m_thread.joinable()) {
		m_thread.join();
	}
}
void WThread::EnqueueTask(std::function<void()> task)
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_tasks.push(task);
	}
	m_cv.notify_one();
}

void WThread::DequeueTask()
{
}

size_t WThread::GetQueueSize()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_tasks.size();
}

void WThread::Run()
{
	while (m_running)
	{
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, [this] { return !m_tasks.empty() || !m_running; });
			if (!m_running && m_tasks.empty())
				return;
			task = std::move(m_tasks.front());
			m_tasks.pop();
		}
		if (task)
		{
			task();
		}
	}
}
