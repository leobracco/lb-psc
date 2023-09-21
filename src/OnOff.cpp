#include "OnOff.h"


RelayControl::RelayControl(int pin) : _pin(pin), _block(true) {}  // Inicializamos _block en false

void RelayControl::begin() {
    pinMode(_pin, OUTPUT);
    turnOff();  // Asumimos que el relé empieza apagado
}

void RelayControl::turnOn() {
    if (!_block) {  // Si no está bloqueado
        digitalWrite(_pin, LOW);  // Encender el relé. Asegúrate de que HIGH es el estado para encenderlo.
          Serial.println("Encendio");
    }
    else 
    Serial.println("Relay is blocked");
}

void RelayControl::turnOff() {
    digitalWrite(_pin, HIGH);  // Apagar el relé. Asegúrate de que LOW es el estado para apagarlo.
}

void RelayControl::setBlock(bool value) {
    _block = value;  // Establecemos el estado de bloqueo
    if (_block) {
        turnOff();  // Si bloqueamos, aseguramos que el relay esté apagado
    }
}

bool RelayControl::isBlocked() const {
    return _block;  // Devolvemos el estado actual de bloqueo
}
