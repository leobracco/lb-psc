# lb-psc
https://leonardobracco.com<br>
**Sistema de Control de Relé para un Termo Eléctrico con Interfaz de Telegram**

Descripción:

El sistema de control que hemos diseñado permite a los usuarios manejar y programar con precisión un termo eléctrico utilizando la popular plataforma de mensajería, Telegram. Gracias a este sistema, ya no es necesario estar físicamente cerca del termo para encenderlo o apagarlo; todo lo que se necesita es un dispositivo con acceso a Telegram.

Características principales:

1. **Programación de Días y Horas:** A través de comandos simples, los usuarios pueden establecer en qué días de la semana y a qué horas desean que el termo eléctrico se encienda o apague. Esto es ideal para aquellos que tienen rutinas fijas y desean que el agua esté caliente en momentos específicos, como por la mañana antes de ir a trabajar o por la noche antes de dormir.

2. **Interfaz Intuitiva en Telegram:** Con un set de comandos amigables y respuestas inmediatas, los usuarios pueden programar, revisar y modificar los horarios de encendido/apagado directamente desde un chat en Telegram. No es necesario instalar ninguna aplicación adicional, ya que todo se maneja a través de esta plataforma de mensajería.

3. **Seguridad:** El sistema está diseñado para ser utilizado sólo por aquellos que tienen el "chat_id" autorizado. Esto garantiza que solo las personas con permiso puedan modificar la programación del termo.

4. **Estado en Tiempo Real:** A través de Telegram, el usuario puede solicitar y obtener información en tiempo real sobre el estado del termo, sabiendo si está encendido, apagado o cuál es la próxima programación establecida.

5. **Adaptabilidad:** Aunque el sistema está diseñado para termos eléctricos, su núcleo puede ser adaptado para controlar otros dispositivos eléctricos que requieran una programación horaria.

6. **Notificaciones:** El sistema puede enviar notificaciones automáticas a Telegram informando al usuario cuando el termo se enciende o apaga, o si hay algún problema con el dispositivo.

Este sistema combina la comodidad de la automatización con la facilidad de uso de una plataforma de mensajería ampliamente utilizada. La posibilidad de programar a distancia y recibir notificaciones hace que el control del termo eléctrico sea más eficiente y adecuado para la vida moderna. Es una solución ideal para aquellos que buscan maximizar la eficiencia energética y la comodidad en su hogar.




### Introducción:

Esta guía ofrece una descripción paso a paso para configurar y utilizar un ESP8266 con la API de Telegram. Asegúrese de seguir cada paso cuidadosamente.

### Preparación:

1. Asegúrese de tener todos los archivos necesarios en la raíz de su proyecto:

    - `WifiConfig.json`
    - `Programas.json`
    - `NodoConfig.json`

2. Asegúrese de tener una herramienta para cargar estos archivos JSON en el sistema de archivos del ESP8266, como la herramienta ESP8266 LittleFS data upload.

### Configuración:

1. **Cree un Bot en Telegram**:
    - Inicie una conversación con [@BotFather](https://t.me/botfather) en Telegram.
    - Siga las instrucciones para crear un nuevo bot.
    - Al final del proceso, @BotFather le proporcionará un **TOKEN**. Anote este TOKEN.

2. **Obtener el Chat ID**:
    - Inicie una conversación con IDBot.
    - Utiliza el comando /getid.
    - El bot respondera su chat_id.
    - No comparta ni publique su token o chat ID por razones de seguridad.

3. **Configuración de Archivos JSON**:

    - **WifiConfig.json**: Ingrese las credenciales de su red Wi-Fi.
      ```json
      {
        "SSID": "tussid",
        "Password": "tupassword"
      }
      ```

    - **NodoConfig.json**: Ingrese el **TOKEN** y **Chat ID** que obtuvo de los pasos anteriores.
      ```json
      {
        "Token": "xxxxxxxxxxxx_xxxxxxxxxxxxxxxxxxx",
        "Name": "LB-PSC",
        "Version": "1.0.0",
        "chat_id": [
          {
            "id": "1",
            "value": "xxxxxxxxx"
          }    
        ]
      }
      ```

    - **Programas.json**: Este archivo contiene la programación de los días. Modifíquelo según sus necesidades.
      ```json
      {
        "lunes": [
          {
            "id": "L001",
            "inicio": "11:15",
            "fin": "12:15"
          }
        ],
        "martes": [
          {
            "id": "M001",
            "inicio": "09:30",
            "fin": "10:45"
          }
        ]
      }
      ```

4. **Configuración en PlatformIO**:

    En el archivo de configuración de PlatformIO, ingrese la información relevante:
    ```json
    [env:d1_mini]
platform = espressif8266
board = d1_mini
framework = arduino
upload_port = /dev/ttyUSB0
monitor_speed = 115200
lib_deps = 
    bblanchon/ArduinoJson@^6.21.2
    bbx10/DNSServer@^1.1.0
    witnessmenow/UniversalTelegramBot@^1.3.0
    paulstoffregen/TimeAlarms@0.0.0-alpha+sha.c291c1ddad
    paulstoffregen/Time@^1.6.1
board_build.filesystem = littlefs
;monitor_filters = esp8266_exception_decoder
;build_flags = -D TELEGRAM_DEBUG
;[build_flags]
;-DPLATFORMIO_BUILD_FILTER=main.cpp

    ```

5. **Carga de Archivos al ESP8266**:
    - Utilice su herramienta elegida para cargar los archivos JSON al sistema de archivos del ESP8266.
    - Una vez cargados, el ESP8266 debería poder acceder a estos archivos y leer las configuraciones proporcionadas.

### Uso:

1. Energice su ESP8266.
2. Si todo se ha configurado correctamente, el ESP8266 intentará conectarse a la red Wi-Fi proporcionada. Una vez conectado, interactuará con la API de Telegram.
3. Inicie una conversación con su bot de Telegram y envíe comandos según la funcionalidad programada en su ESP8266.

### Notas:

- Si realiza cambios en los archivos JSON después de cargarlos al ESP8266, deberá volver a cargarlos para que los cambios surtan efecto.
- Asegúrese de que su ESP8266 siempre tenga acceso a la red para que pueda interactuar con la API de Telegram.

