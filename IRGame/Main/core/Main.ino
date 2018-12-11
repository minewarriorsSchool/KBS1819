#include "data"
#include "tasks"
#define maxTask 10;

Task tasks[maxTask];

Task.addTask(testTask);

IRCommunication IR();
int main(void){
	for (int i= 0; i<maxTask; i++)
	{
		tasks[i].run();
	}
}