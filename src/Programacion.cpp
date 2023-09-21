#include "Programacion.h"
#include "OnOff.h"
#include "config.h"

int obtenerHora(const String &tiempo)
{
    int posColon = tiempo.indexOf(':');
    return tiempo.substring(0, posColon).toInt();
}

int obtenerMinuto(const String &tiempo)
{
    int posColon = tiempo.indexOf(':');
    return tiempo.substring(posColon + 1).toInt();
}
void turnOnToma1Callback()
{
    Toma1.turnOn();
}

void turnOffToma1Callback()
{
    Toma1.turnOff();
}
void cargarProgramacion()
{
    // Recorremos cada día programado
    for (Dia &dia : Programas.dias)
    {
        for (Horario &horario : dia.horarios)
        {
            // Obtenemos las horas y minutos de los horarios
            int inicioHora = obtenerHora(horario.inicio);
            int inicioMinuto = obtenerMinuto(horario.inicio);
            int finHora = obtenerHora(horario.fin);
            int finMinuto = obtenerMinuto(horario.fin);

            // Según el nombre del día, configuramos las alarmas
            if (dia.nombre == "lunes")
            {
                Alarm.alarmRepeat(dowMonday, inicioHora, inicioMinuto, 0, turnOnToma1Callback);
                Alarm.alarmRepeat(dowMonday, finHora, finMinuto, 0, turnOffToma1Callback);
            }
            else if (dia.nombre == "martes")
            {
                Alarm.alarmRepeat(dowTuesday, inicioHora, inicioMinuto, 0, turnOnToma1Callback);
                Alarm.alarmRepeat(dowTuesday, finHora, finMinuto, 0, turnOffToma1Callback);
            }
            else if (dia.nombre == "miercoles")
            {
                Alarm.alarmRepeat(dowWednesday, inicioHora, inicioMinuto, 0, turnOnToma1Callback);
                Alarm.alarmRepeat(dowWednesday, finHora, finMinuto, 0, turnOffToma1Callback);
            }
            else if (dia.nombre == "jueves")
            {
                Alarm.alarmRepeat(dowThursday, inicioHora, inicioMinuto, 0, turnOnToma1Callback);
                Alarm.alarmRepeat(dowThursday, finHora, finMinuto, 0, turnOffToma1Callback);
            }
            else if (dia.nombre == "viernes")
            {
                Alarm.alarmRepeat(dowFriday, inicioHora, inicioMinuto, 0, turnOnToma1Callback);
                Alarm.alarmRepeat(dowFriday, finHora, finMinuto, 0, turnOffToma1Callback);
            }
            else if (dia.nombre == "sabado")
            {
                Alarm.alarmRepeat(dowSaturday, inicioHora, inicioMinuto, 0, turnOnToma1Callback);
                Alarm.alarmRepeat(dowSaturday, finHora, finMinuto, 0, turnOffToma1Callback);
            }
            else if (dia.nombre == "domingo")
            {
                Alarm.alarmRepeat(dowSunday, inicioHora, inicioMinuto, 0, turnOnToma1Callback);
                Alarm.alarmRepeat(dowSunday, finHora, finMinuto, 0, turnOffToma1Callback);
            }

            else if (dia.nombre == "todos")
            {
                Alarm.alarmRepeat(inicioHora, inicioMinuto, 0, turnOnToma1Callback);
                Alarm.alarmRepeat(finHora, finMinuto, 0, turnOffToma1Callback);
            }
        }
    }
}
