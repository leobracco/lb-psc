#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <config.h>
#include "TelegramMenu.h"
#include <EEPROM.h>
#include <Programacion.h>
const int EEPROM_ADDR = 0;
WifiConfig WifiConf;
const unsigned long BOT_MTBS = 1000; 
long lastUpdateId = 0;
unsigned long bot_lasttime; 
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot("", secured_client);
long cargarDesdeMemoria()
{
  long value = 0;

  for (int i = 0; i < 4; i++)
  {
    value = (value << 8) + EEPROM.read(EEPROM_ADDR + i);
  }

  return value;
}

void guardarEnMemoria(long id)
{
  for (int i = 0; i < 4; i++)
  {
    EEPROM.write(EEPROM_ADDR + i, (id >> (8 * (3 - i))) & 0xFF);
  }
  EEPROM.commit(); // Asegúrate de que los cambios se escriban en EEPROM
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  // Iniciar EEPROM
  EEPROM.begin(4); // Reserva 4 bytes para lastUpdateId
  if (LittleFS.begin())
  {
    Serial.println(F("done."));
  }
  else
  {
    Serial.println(F("fail."));
  }
  
  ConfigureNodo();
  ConfigureWifi();
  secured_client.setTrustAnchors(&cert); 
  Serial.print("Conectando a ");
  Serial.println(WifiConf.SSID);
  WiFi.begin(WifiConf.SSID, WifiConf.Password);

  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);

  }
  Serial.print("WiFi conectado. IP address: ");
  Serial.println(WiFi.localIP());
  configTime(-3 * 3600, 0, "1.ar.pool.ntp.org", "time.nist.gov");

 
  time_t nowDate = time(nullptr);
  while (nowDate < 24 * 3600)
  {
    delay(100);
    nowDate = time(nullptr);
  }
  Serial.println("Hora sincronizada!");
  Toma1.begin();
  Toma2.begin();
  Toma3.begin();
  Toma4.begin();
  
  setTime(nowDate - 3 * 3600);
  cargarConfiguracionProgramas();
  
  cargarProgramacion();
  bot = UniversalTelegramBot(Nodo.Token, secured_client);
  
  Serial.print("Retrieving time: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
  lastUpdateId = cargarDesdeMemoria();
  sendInitTelegram(bot,Nodo);
}

void loop()
{
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  if (numNewMessages)
  {
    if (bot.messages[numNewMessages - 1].update_id != lastUpdateId)
    {
       lastUpdateId = bot.messages[numNewMessages - 1].update_id;
      guardarEnMemoria(lastUpdateId);
      handleNewMessages(bot, numNewMessages);
     
    }
  }

  Alarm.delay(1000);
}
