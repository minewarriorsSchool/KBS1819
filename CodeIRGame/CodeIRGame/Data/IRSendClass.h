//
// Created by jelle on 23/11/2018.
//

#ifndef CODEIRGAME_IRSENDCLASS_H
#define CODEIRGAME_IRSENDCLASS_H


#include <stdint-gcc.h>
class IRSendClass {

public:
    IRSendClass();
    void IRSend(uint8_t Protocol, uint16_t data);
protected:

private:
};


#endif //CODEIRGAME_IRSENDCLASS_H
