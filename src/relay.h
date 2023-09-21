#ifndef RELAY_h
#define RELAY_h

#include "Arduino.h"
void initializePins();
class Relay {
public:
    // Constructor que establece el pin de la sección.
    Relay(int pin);

    // Función para encender la sección (relay).
    void ON();

    // Función para apagar la sección (relay).
    void OFF();

private:
    int _pin;  // Pin al que está conectada la sección (relay).
};

#endif
