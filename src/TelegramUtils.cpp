#include <ArduinoJson.h>  
#include <FS.h> 
#include <TelegramUtils.h> 
#include <config.h>
#include <TimeLib.h>

bool esDiaValido(String dia) {
    const char* diasValidos[] = {"lunes", "martes", "miercoles", "jueves", "viernes", "sabado", "domingo", "todos"};
    
    for (int i = 0; i < 8; i++) {
        if (dia == diasValidos[i]) {
            return true;
        }
    }
    return false;
}
// Función para agregar un chat_id a tu estructura
void agregarChatID(NodoConfig& Nodo, const String& chatIDValue) {
    ChatID nuevoChat;
    nuevoChat.id = String(Nodo.chat_ids.size() + 1); 
    nuevoChat.value = chatIDValue;
    Nodo.chat_ids.push_back(nuevoChat);
}
String listarChatIDs(const NodoConfig& Nodo) {
    String lista = "Usuarios:\n";

    for (const ChatID& chat : Nodo.chat_ids) {
        lista += "ID: " + chat.id + "-> Usuario: " + chat.value + "\n";
    }

    return lista;
}



bool borrarChatID(NodoConfig& Nodo, const String& id) {
    for (size_t i = 0; i < Nodo.chat_ids.size(); ++i) {
        if (Nodo.chat_ids[i].id == id) {
            Nodo.chat_ids.erase(Nodo.chat_ids.begin() + i);
            return true;  // Retorna true si el chat_id se borró con éxito
        }
    }
    return false;  // Retorna false si no se encontró el chat_id
}
bool guardarConfiguracion(const NodoConfig& Nodo) {
    DynamicJsonDocument json(2048);

    
    json["Token"] = Nodo.Token;
    json["Name"] = Nodo.Name;
    json["Version"] = Nodo.Version;

    JsonArray chat_ids_array = json.createNestedArray("chat_id");
    for (const ChatID& chat : Nodo.chat_ids) {
        JsonObject chat_id_obj = chat_ids_array.createNestedObject();
        chat_id_obj["id"] = chat.id;
        chat_id_obj["value"] = chat.value;
    }

   
    return saveConfig(json, "/NodoConfig.json");
}
bool parsearHorarios(String texto, String &dia, String &horaInicio, String &horaFin) {
    
    if (texto.startsWith("/programa")) {
        
        int posDia = texto.indexOf(' ', 9) + 1; 
        int posInicio = texto.indexOf(' ', posDia) + 1;  
        int posFin = texto.indexOf(' ', posInicio);  
        
       
        if(posDia == -1 || posInicio == -1 || posFin == -1) {
            return false;
        }

        dia = texto.substring(posDia, posInicio - 1);  
        if(!esDiaValido(dia)) {
            return false;
        }
        
        horaInicio = texto.substring(posInicio, posFin);  
        horaFin = texto.substring(posFin + 1);  

        return true;
    }

    return false;  
}


bool guardarHorariosEnJson(String horaInicio, String horaFin, const char* archivo) {
    DynamicJsonDocument doc(512);

    doc["horaInicio"] = horaInicio.c_str();
    doc["horaFin"] = horaFin.c_str();

    
    return true;
}
bool parsearBorrarComando(const String& texto, String& id) {
    
    if (texto.startsWith("/borra")) {
        id = texto.substring(7);
        id.trim();  
        return true;
    }
    return false;
}
String obtenerProximoID(const String& dia, const ProgramasConfig& Programas) {
    int maxID = 0;

    for (const Dia& d : Programas.dias) {
        if (d.nombre == dia) {
            Serial.print("Dia: ");
            Serial.println(d.nombre);
            for (const Horario& h : d.horarios) {
                int currentID = h.id.substring(1).toInt();
                Serial.print("ID: ");
                Serial.println(currentID);
                if (currentID > maxID) {
                    maxID = currentID;
                }
            }
        }
    }

    // Construye el nuevo ID
    char prefix = toupper(dia[0]);
    maxID++;  // Incrementa el ID máximo

    // Construye el ID con el relleno necesario
    String nuevoID = String(prefix) + String((maxID < 10 ? "00" : (maxID < 100 ? "0" : ""))) + String(maxID);

    return nuevoID;
}


bool agregarNuevoHorario(const String& dia, const String& inicio, const String& fin, ProgramasConfig& Programas) {
    Horario nuevoHorario;
    nuevoHorario.inicio = inicio;
    nuevoHorario.fin = fin;
    nuevoHorario.id = obtenerProximoID(dia, Programas);

    for (Dia& d : Programas.dias) {
        if (d.nombre == dia) {
            d.horarios.push_back(nuevoHorario);
            return true;
        }
    }

    // Si el día no existe, agregamos un nuevo día con el horario
    Dia nuevoDia;
    nuevoDia.nombre = dia;
    nuevoDia.horarios.push_back(nuevoHorario);
    Programas.dias.push_back(nuevoDia);

    return true;
}
void convertirProgramasAJson(const ProgramasConfig& programas, DynamicJsonDocument& doc) {
    for (const Dia& dia : programas.dias) {
        JsonArray horariosArray = doc.createNestedArray(dia.nombre);
        for (const Horario& horario : dia.horarios) {
            JsonObject horarioObj = horariosArray.createNestedObject();
            horarioObj["id"] = horario.id;
            horarioObj["inicio"] = horario.inicio;
            horarioObj["fin"] = horario.fin;
        }
    }
}
bool guardarProgramas(const ProgramasConfig& programas, const char* FileName) {
    DynamicJsonDocument doc(2048);  // Ajusta el tamaño si es necesario
    convertirProgramasAJson(programas, doc);

    return saveConfig(doc, FileName);
}
bool borrarPorId(const String& idABorrar) {
    for (Dia& dia : Programas.dias) {
        for (size_t i = 0; i < dia.horarios.size(); ++i) {
            if (dia.horarios[i].id == idABorrar) {
                dia.horarios.erase(dia.horarios.begin() + i);
                return true;  // Retornar verdadero si se encontró y borró el ID
            }
        }
    }
    return false;  // Retornar falso si el ID no se encontró
}
String formatProgramasForTelegram(const ProgramasConfig& configuracion) {
    String mensaje = "";

    for (const Dia& dia : configuracion.dias) {
        mensaje += dia.nombre + "\n";

        for (const Horario& horario : dia.horarios) {
            mensaje += "ID: " + horario.id + " Desde " + horario.inicio + " hasta " + horario.fin + "\n";
        }

        mensaje += "\n";  // Una línea en blanco para separar los días
    }

    return mensaje;
}


const char* dias[] = {"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"};
String obtenerFechaHora() {
  String resultado;

  resultado += "Hora: ";
  resultado += horaFormato(hour()) + ":";
  resultado += horaFormato(minute()) + ":";
  resultado += horaFormato(second()) + "\n";

  resultado += "Fecha: ";
  resultado += String(day()) + "/";
  resultado += String(month()) + "/";
  resultado += String(year()) + "\n";

  resultado += "Día de la semana: " + String(dias[weekday() - 1]) + "\n";

  resultado += "Día del año: " + String(dayOfYear()) + "\n";

  return resultado;
}

String horaFormato(int digits) {
  if (digits < 10) {
    return "0" + String(digits);
  } else {
    return String(digits);
  }
}
int dayOfYear() {
  int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (year() % 4 == 0 && (year() % 100 != 0 || year() % 400 == 0)) {
    // Año bisiesto
    monthDays[1] = 29;
  }
  int daysPassed = day();
  for (int i = 0; i < month() - 1; i++) {
    daysPassed += monthDays[i];
  }
  return daysPassed;
}

