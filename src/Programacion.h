#ifndef PROGRAMACION_H
#define PROGRAMACION_H

#include <Arduino.h>
#include <TimeAlarms.h>
#include <vector>
#include <config.h>


void cargarProgramacion();
int obtenerHora(const String& tiempo);
int obtenerMinuto(const String& tiempo);



#endif  // PROGRAMACION_H
