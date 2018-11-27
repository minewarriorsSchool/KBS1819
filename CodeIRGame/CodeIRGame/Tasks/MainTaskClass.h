//
// Created by jelle on 27/11/2018.
//

#ifndef CODEIRGAME_MAINTASKCLASS_H
#define CODEIRGAME_MAINTASKCLASS_H


class MainTaskClass {
public:
    MainTaskClass();
    virtual int canProcess() = 0;
    virtual void process() = 0;
    void run(){
        if(MainTaskClass::canProcess()){
            MainTaskClass::process();
        }
    }
};


#endif //CODEIRGAME_MAINTASKCLASS_H
