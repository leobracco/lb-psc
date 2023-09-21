#ifndef ONOFF_H
#define ONOFF_H

#include <Arduino.h>
class RelayControl {
public:
    RelayControl(int pin);
    void begin();
    void turnOn();
    void turnOff();
    void setBlock(bool value);
    bool isBlocked() const;

private:
    int _pin;
    bool _block;  
};


#endif  // ONOFF_H
