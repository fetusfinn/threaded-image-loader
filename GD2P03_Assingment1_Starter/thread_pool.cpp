//
//	thread_pool.cpp
//
#include "thread_pool.h"

//
//
//
CThreadPool::CThreadPool() : m_iTasksProcessed(0), m_iNumThreads(std::thread::hardware_concurrency())
{
	// Start our threads
	for (int i = 0; i < m_iNumThreads; i++)
	{
		m_vWorking.push_back(false);
		m_arrThreads.push_back(std::thread(&CThreadPool::WorkerThread, this, std::move(i)));
	}

}

//
//
//
CThreadPool::~CThreadPool()
{
	this->Stop();
}

//
// Adds the given task to our queue
//
void CThreadPool::AddTask(CTask* _rTask)
{
	m_arrTaskQueue.Push(_rTask);
}

//
// Waits until all threads have finished working
//
void CThreadPool::WaitTilFinished()
{
	while (std::find(m_vWorking.begin(), m_vWorking.end(), true) != m_vWorking.end())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}


//
// Kills all threads in our pool
//
void CThreadPool::Stop()
{
	m_bFinished = true;

	m_arrTaskQueue.UnblockAll();

	for (int i = 0; i < m_iNumThreads; i++)
	{
		if (m_arrThreads.at(i).joinable())
		{
			m_arrThreads.at(i).join();
		}
	}
}

//
// Returns the total number of tasks completed
//
int CThreadPool::TasksProcessed()
{
	return m_iTasksProcessed;
}

//
// Function run in our thread to handle the tasks
//
void CThreadPool::WorkerThread(int i)
{
	CTask* rTask;
	while (!m_bFinished)
	{
		if (m_arrTaskQueue.Pop(rTask))
		{
			m_vWorking.at(i) = true;

			rTask->DoWork();

			++m_iTasksProcessed;

			// Clean up
			delete rTask;

			m_vWorking.at(i) = false;
		}
	}
}
