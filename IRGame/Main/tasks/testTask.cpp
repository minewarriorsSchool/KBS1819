/*
* testTask.cpp
*
* Created: 06/12/2018 11:53:17
* Author: jelle
*/


#include <>

class testTask: public Task{
	public:
	int canProcess(){
		if(1){
			return 1;
		}
	}

	void process(){
		Serial.println("Daaaaamn het werkt");
	}
