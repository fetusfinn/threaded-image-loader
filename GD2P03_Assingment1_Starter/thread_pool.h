//
//	thread_pool.h
//
#pragma once

#include <vector>
#include <atomic>
#include <thread>

#include "safe_queue.h"
#include "task.h"

class CThreadPool
{
private:

	// List of all our threads
	std::vector<std::thread> m_arrThreads;

	// Pointers so that the child classes arent cast
	// back into a CTask object 
	CSafeQueue<CTask*>	m_arrTaskQueue;

	// Whether or not we should kill all tasks
	std::atomic_bool	m_bFinished;

	// Total number of tasks processed
	std::atomic_int		m_iTasksProcessed;

	// Total number of threads we have available to use on our system
	unsigned int		m_iNumThreads;

	// Whether or not the corresponding thread is working
	std::vector<bool> m_vWorking;

	void WorkerThread(int _iIndex);

public:

	CThreadPool();
	~CThreadPool();

	void AddTask(CTask* _rTask);

	void WaitTilFinished();

	void Stop();

	int TasksProcessed();

};
