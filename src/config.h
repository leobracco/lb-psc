#ifndef CONFIG_H
#define CONFIG_H
#include <stdint.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <OnOff.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <vector>
extern const uint16_t LOOP_TIME; // in msec = 20hz
extern uint32_t LoopLast;

extern bool AutoOn;
extern bool DEBUG;
extern bool MasterOn;

typedef uint8_t byte;
void iniciarLittleFS();

String loadConfig(const char *FileName);
bool saveConfig(DynamicJsonDocument &json, const char *FileName);
void ConfigNetWork();
void ConfigureNodo();
void ConfigureWifi();
void ResetConfig();
void cargarConfiguracionProgramas() ;
void listarArchivos();
void initializeConfig();
struct ChatID {
    String id;
    String value;
};

struct NodoConfig {
    char Name[10];
    char Version[10];
    char Token[100];
    std::vector<ChatID> chat_ids;
};  // Suponiendo que 'Nodo' es una instancia global de NodoConfig



// Representa un horario específico
struct Horario {
	String id;      // Identificación del día, por ejemplo, "1" para lunes
    String inicio;  // Hora de inicio, por ejemplo, "10:15"
    String fin;     // Hora de fin, por ejemplo, "11:15"
};

// Representa un día específico con una lista de horarios
struct Dia {
   
    String nombre;  // Nombre del día, por ejemplo, "lunes"
    std::vector<Horario> horarios;  // Lista de horarios para ese día
};

// Representa la configuración completa de todos los días
struct ProgramasConfig {
    std::vector<Dia> dias;  // Lista de días con sus horarios
};
struct WifiConfig
{
	char SSID[40];
	char Password[40];
	char UrlOTA[100];
};
struct MQTTConfig
{
	char BrokerAddress[40]; // Dirección IP o nombre de dominio del broker MQTT
	char Port[6];			// Puerto del broker MQTT
	char Username[20];		// Nombre de usuario para autenticación en el broker MQTT
	char Password[20];		// Contraseña para autenticación en el broker MQTT
	char ClientID[20];		// ID único del cliente MQTT
};



#define NUM_SECTIONS 4
extern const int relayPins[NUM_SECTIONS]; 
extern NodoConfig Nodo;
extern MQTTConfig MQTTConf;
extern WifiConfig WifiConf;
extern ProgramasConfig Programas;
extern float speedKmH;
extern RelayControl Toma1; 
extern RelayControl Toma2;
extern RelayControl Toma3;  
extern RelayControl Toma4; 
extern X509List cert;
extern WiFiClientSecure secured_client;
extern UniversalTelegramBot bot;

#endif // CONFIG_H
