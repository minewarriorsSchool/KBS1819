#include <iostream>
#include "../Tasks/MainTaskClass.h"

const int numberOfTasks = 10;
MainTaskClass tasks[numberOfTasks];


int main() {
    while(1){
        for (int i = 0; i < numberOfTasks ; ++i) {
            tasks[i].run();
        }
    }
    return 0;
}