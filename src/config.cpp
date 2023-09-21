#include <Arduino.h>
#include "config.h"
#include "OnOff.h"
#include <ArduinoJson.h>
ProgramasConfig Programas;
NodoConfig Nodo;
// Configuración de sensor
void iniciarLittleFS()
{
  if (LittleFS.begin())
  {
    Serial.println(F("done."));
  }
  else
  {
    Serial.println(F("fail."));
  }
}


void listarArchivos() {
    Dir dir = LittleFS.openDir("/");  // Abre el directorio raíz

    Serial.println("Listando archivos en LittleFS:");

    while (dir.next()) {  // Mientras haya más archivos en el directorio
        String archivoNombre = dir.fileName();
        size_t tamano = dir.fileSize();

        Serial.print("Nombre del archivo: ");
        Serial.println(archivoNombre);
        
        Serial.print("Tamaño del archivo: ");
        Serial.println(tamano);
        
        Serial.println("--------------");
    }
}
String loadConfig(const char *FileName)
{
  String configContent = "";
  File configFile = LittleFS.open(FileName, "r");
  if (!configFile)
  {
    Serial.println("No se puede abrir el archivo en Load");
  }
  else
  {
    while (configFile.available())
    {
      configContent += char(configFile.read());
    }
    configFile.close();
  }
  return configContent;
}

bool saveConfig(DynamicJsonDocument &json,
                const char *FileName)
{

  File configFile = LittleFS.open(FileName, "w");
  if (!configFile)
  {
    Serial.println("No se puede abrir el archivo para grabar");
    return false;
  }
  else
    Serial.println("Se grabo correctamente");
  serializeJson(json, Serial);
  serializeJson(json, configFile);
  configFile.close();
  return true;
}

void ConfigureNodo()
{
  Serial.println("Ingresa a Config Nodo...");

  StaticJsonDocument<768> json;

  DeserializationError error = deserializeJson(json, loadConfig("/NodoConfig.json"));
  serializeJson(json, Serial);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  
  strcpy(Nodo.Name, json["Name"]);
  strcpy(Nodo.Version, json["Version"]);
  strcpy(Nodo.Token, json["Token"]);

  // Limpiar la lista de chat_ids actual en Nodo
  Nodo.chat_ids.clear();

  // Cargar los chat_ids desde el JSON
  JsonArray chat_ids_array = json["chat_id"];
  for(JsonObject chat_id_obj : chat_ids_array) {
      ChatID chat;
      chat.id = chat_id_obj["id"].as<String>();
      chat.value = chat_id_obj["value"].as<String>();
      Nodo.chat_ids.push_back(chat);
  }
}
void cargarConfiguracionProgramas() 
{
   Serial.println("Ingresa a cargarConfiguracionProgramas...");

    StaticJsonDocument<2048> json;

    DeserializationError error = deserializeJson(json, loadConfig("/Programas.json"));
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    for (JsonPair kv : json.as<JsonObject>()) {
        Dia dia;
        dia.nombre = kv.key().c_str();

        JsonArray horariosArray = kv.value().as<JsonArray>();
        for (JsonObject horarioObj : horariosArray) {
            Horario horario;
             horario.id = horarioObj["id"].as<String>();
            horario.inicio = horarioObj["inicio"].as<String>();
            horario.fin = horarioObj["fin"].as<String>();
            dia.horarios.push_back(horario);
        }

        Programas.dias.push_back(dia);
    }
}
/**********************Carga las variables del Wifi desde el Archivo WifiConfig.json*/
void ConfigureWifi()
{
  Serial.println("Ingresa a Config Wifi...");

  StaticJsonDocument<768> json;

  DeserializationError error = deserializeJson(json, loadConfig("/WifiConfig.json"));
  serializeJson(json, Serial);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  strcpy(WifiConf.SSID, json["SSID"]);
  strcpy(WifiConf.Password, json["Password"]);
  // strcpy(WifiConf.UrlOTA, json["UrlOTA"]);
  // strcpy(WifiConf.PasswordOTA, json["PasswordOTA"]);
}
void ResetConfig()
{
  DynamicJsonDocument doc(256);
  DynamicJsonDocument docmqtt(256);

  doc["SSID"] = "-";
  doc["Password"] = "-";


  if (saveConfig(doc, "/WifiConfig.json"))
  {
    Serial.print("Config Save");

    strcpy(WifiConf.SSID, doc["SSID"]);
    strcpy(WifiConf.Password, doc["Password"]);
  }
  

  delay(1000);
  ESP.restart();
}


void initializeConfig() {
    
    
    // Establece el certificado raíz para secured_client
    //secured_client.setTrustAnchors(&cert);
    //secured_client.setFingerprint("f2 ad 29 9c 34 48 dd 8d f4 cf 52 32 f6 57 33 68 2e 81 c1 90");
     bot = UniversalTelegramBot(Nodo.Token, secured_client);
}

bool AutoOn = true;
bool MasterOn = false;
bool DEBUG = false;
const uint16_t LOOP_TIME = 50; // in msec = 20hz
uint32_t LoopLast = LOOP_TIME;
float speedKmH;

const int relayPins[NUM_SECTIONS]={13, 12, 14, 5};

RelayControl Toma1(D4);
RelayControl Toma2(D2);
RelayControl Toma3(D5);
RelayControl Toma4(D6);