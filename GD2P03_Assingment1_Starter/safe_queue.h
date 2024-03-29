//
//	safe_queue.h
//
#pragma once

#include <queue>
#include <mutex>
#include <atomic>

template<typename T>
class CSafeQueue
{
private:

	std::queue<T> m_arrQueue;
	std::mutex m_rMutex;
	std::condition_variable m_rCV;

	std::atomic_bool m_bEscape;

public:

	CSafeQueue()
	{

	}

	~CSafeQueue()
	{

	}

	//
	// Adds item to our queue
	//
	void Push(const T& _rItem)
	{
		std::lock_guard<std::mutex> rLock(m_rMutex);

		m_arrQueue.push(_rItem);

		m_rCV.notify_one();
	}

	//
	// Pops the first item in our queue
	//
	bool Pop(T& _rItem)
	{
		std::lock_guard<std::mutex> rLock(m_rMutex);

		if (m_arrQueue.empty())
			return false;

		_rItem = std::move(m_arrQueue.front());
		m_arrQueue.pop();

		return true;
	}

	//
	//
	//
	bool BlockPop(T& _rItem)
	{
		std::unique_lock<std::mutex> rLock(m_rMutex);

		m_rCV.wait(rLock, [this]
		{
			return (!m_arrQueue.empty() || m_bEscape);
		});

		if (!m_bEscape)
		{
			_rItem = std::move(m_arrQueue.front());
			m_arrQueue.pop();
		}

		return !m_bEscape;
	}

	//
	// 
	//
	void UnblockAll()
	{
		m_bEscape = true;
		m_rCV.notify_all();
	}

	//
	// Returns the size of our queue
	//
	int Size() const
	{
		return m_arrQueue.size();
	}

};