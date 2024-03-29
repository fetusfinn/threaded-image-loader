//
//
//
#pragma once

#include <functional>
#include <thread>

class CTask
{
protected:

	static int s_iTaskID;

	int m_iData;

public:

	CTask() : m_iData(s_iTaskID++)
	{
		
	}

	virtual ~CTask()
	{

	}

	virtual void operator()()
	{
		DoWork();
	}

	//
	// Main function
	//
	virtual void DoWork() 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds((rand() % 1000) + 200));

		printf("Task not implemented!\nTask %i complete!\n", m_iData);
	}


};
