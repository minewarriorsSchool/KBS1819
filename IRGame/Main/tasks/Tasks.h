// Tasks.h

#ifndef _TASKS_h
#define _TASKS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class AbstractTaskClass{
	public:
	virtual void f() = 0;		//making the class abstract
	AbstractTaskClass();
	boolean canProcess();
	void process(){};
	void run(){
		if(canProcess()){
			process();
		}
	};
	private:
};

class Task1 : public AbstractTaskClass{
	Task1() : AbstractTaskClass(){};
	canProcess(){
		if(1){
			return true;
		}
		return false;
	}
	
	process(){
		Serial.println("Task1");
	}
};

class Task2 : public AbstractTaskClass{
	Task2() : AbstractTaskClass(){};
	canProcess(){
		if(1){
			return true;
		}
		return false;
	}
	
	process(){
		Serial.println("Task2");
	}
};

#endif

