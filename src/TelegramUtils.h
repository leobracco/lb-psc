#ifndef TELEGRAM_UTILS_H
#define TELEGRAM_UTILS_H
#include <config.h>
#include <ArduinoJson.h>
#include <FS.h>  // Para el manejo de archivos
String formatProgramasForTelegram(const ProgramasConfig& Programas);
bool borrarPorId(const String& idABorrar);
bool parsearBorrarComando(const String& texto, String& id) ;
String obtenerProximoID(const String& dia, const ProgramasConfig& Programas) ;
bool agregarNuevoHorario(const String& dia, const String& inicio, const String& fin, ProgramasConfig& Programas) ;
void convertirProgramasAJson(const ProgramasConfig& programas, DynamicJsonDocument& doc);
bool guardarProgramas(const ProgramasConfig& programas, const char* FileName) ;
void agregarChatID(NodoConfig& Nodo, const String& chatIDValue) ;
String listarChatIDs(const NodoConfig& Nodo);
bool borrarChatID(NodoConfig& Nodo, const String& id) ;
bool guardarConfiguracion(const NodoConfig& Nodo);
String obtenerFechaHora();
String horaFormato(int digits);
int dayOfYear() ;
// Declaración de funciones
bool esDiaValido(String dia);
/**
 * Parsea un mensaje de Telegram en busca de horarios de inicio y fin.
 * 
 * @param texto Texto recibido por Telegram.
 * @param horaInicio Referencia a la cadena donde se guardará la hora de inicio.
 * @param horaFin Referencia a la cadena donde se guardará la hora de fin.
 * @return Verdadero si se pudo parsear el mensaje correctamente, falso en caso contrario.
 */
bool parsearHorarios(String texto,  String &dia, String &horaInicio, String &horaFin);
DynamicJsonDocument parsearHorarios(const String& texto);
/**
 * Crea un JSON con las horas proporcionadas y lo guarda en un archivo.
 * 
 * @param horaInicio Hora de inicio a guardar.
 * @param horaFin Hora de fin a guardar.
 * @param archivo Nombre del archivo donde se guardará el JSON.
 * @return Verdadero si el JSON se guardó con éxito, falso en caso contrario.
 */
bool guardarHorariosEnJson(String horaInicio, String horaFin, const char* archivo);

#endif // TELEGRAM_UTILS_H
