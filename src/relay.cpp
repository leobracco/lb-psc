#include <Arduino.h>
#include "relay.h"
#include "config.h"
// Constructor de la clase Relay
Relay::Relay(int pin) {
    _pin = pin;               // Establece el valor de _pin.
    pinMode(_pin, OUTPUT);    // Configura el pin como salida.
}

// Función para encender la sección.
void Relay::ON() {
    digitalWrite(_pin, LOW);  // Suponiendo que LOW enciende el relay.
}

// Función para apagar la sección.
void Relay::OFF() {
    digitalWrite(_pin, HIGH); // Suponiendo que HIGH apaga el relay.
}
void initializePins() {
    for(int i = 0; i < NUM_SECTIONS; i++) {
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], LOW);  // Asumiendo que queremos iniciarlos en estado bajo
    }
}