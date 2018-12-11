/*
* Task.cpp
*
* Created: 06/12/2018 11:54:15
* Author: jelle
*/


#include "Task.h"
#define maxTask 10;

class Task{
	public: Task tasks [maxTasks];
	virtual int canProcess()=0;
	virtual void process()=0;
	void run(){
		if (canProcess())
		{
			process();
		}
	}
	void addTask(Task *task){
		tasks[taskcounter] = task;
		taskcounter++;
	}
	private:
	int taskcounter = 0;
};