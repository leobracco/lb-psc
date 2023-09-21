#include "TelegramMenu.h"

#include "TelegramUtils.h"

#include "config.h"

void sendMenu(UniversalTelegramBot & bot, String chat_id) {
  String menuText = "¡Bienvenido al menú principal! ¿Qué deseas hacer?";

  String keyboardJson = "[[\"/programar\", \"/listar\"],[\"/usuarios\", \"/fecha\"],[\"/bloquear\", \"/activar\"],[\"/encender\", \"/apagar\"]]";

  bot.sendMessageWithReplyKeyboard(chat_id, menuText, "", keyboardJson);
}

void sendSimpleMenu(UniversalTelegramBot & bot, String chat_id) {
  String menu = "";
  menu += "/start: Muestra este menú.\n";
  menu += "/programar: Permite programar un nuevo horario.\n";
  menu += "/listar: Lista los horarios programados.\n";
  menu += "/usuarios: Muestra los chat_ids permitidos.\n";
  menu += "/agregarUsuario [chat_id]: Agrega un chat_id a la lista de usuarios permitidos.\n";
  menu += "/borrarUsuario [id]: Borra un chat_id basado en su ID.\n";
  menu += "/fecha: Muestra la fecha y hora actual.\n";
  menu += "/bloquear: Bloquea la toma, evitando que se encienda.\n";
  menu += "/activar: Activa la toma.\n";
  menu += "/encender: Enciende la toma manualmente.\n";
  menu += "/apagar: Apaga la toma manualmente.\n";
  //menu += "/reset: Resetea el sistema.\n";

  bot.sendMessage(chat_id, menu, "");
}
void sendMenuProgramar(UniversalTelegramBot & bot, String chat_id) {

  String menuText = "Para programar ingresa el comando como se describe:\n\n";
  menuText += "/programa lunes 10:15 11:15\n";
  menuText += "De esta manera todos los lunes inicia 10:15 y termina 11:15\n";
  menuText += "Otro ejemplo seria para todos los dias \n";
  menuText += "/programa todos 12:30 15:25\n";

  bot.sendMessage(chat_id, menuText, "");
}
String idPrograma;
String Dia;
String horaInicio;
String horaFin;
bool chatIdPermitido(String chat_id) {
  for (ChatID chat: Nodo.chat_ids) {
    Serial.print("ChatID: ");
    Serial.println(chat.id);
    if (chat.value == chat_id) {
      return true;
    }
  }
  return false;
}
void sendInitTelegram(UniversalTelegramBot & bot,
  const NodoConfig & Nodo) {

  for (const ChatID & chat: Nodo.chat_ids) {

    bot.sendMessage(chat.value, "Se inicia el controlador, recorda activarlo! O te vas acagar de frio.");
  }

}
void handleNewMessages(UniversalTelegramBot & bot, int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    if (!chatIdPermitido(chat_id)) {
      bot.sendMessage(chat_id, "No tienes permiso para usar este bot.");
      continue;
    }
    if (text.startsWith("/programa "))
      if (parsearHorarios(text, Dia, horaInicio, horaFin)) {
        Serial.println("Hora de inicio: " + horaInicio);
        Serial.println("Hora de fin: " + horaFin);
        if (agregarNuevoHorario(Dia, horaInicio, horaFin, Programas)) {
          if (guardarProgramas(Programas, "/Programas.json"))
            bot.sendMessage(chat_id, "Horario programado", "");
          else
            bot.sendMessage(chat_id, "No se pudo programar el horario", "");

        } else
          bot.sendMessage(chat_id, "No se pudo programar el horario", "");

      }
    if (text.startsWith("/borrar "))
      if (parsearBorrarComando(text, idPrograma)) {
        if (borrarPorId(idPrograma)) {
          if (guardarProgramas(Programas, "/Programas.json"))
            bot.sendMessage(chat_id, "El ID fue borrado exitosamente", "");
          else
            bot.sendMessage(chat_id, "No se pudo borrar el ID", "");
        } else
          bot.sendMessage(chat_id, "No se pudo encontrar el ID", "");

      }

    if (text == "/start" || text == "/help") {
      sendSimpleMenu(bot, chat_id);
    } else if (text == "/programar") {

      sendMenuProgramar(bot, chat_id);
    } else if (text == "/menu") {

      sendMenu(bot, chat_id);
    } else if (text == "/listar") {
      String mensaje = formatProgramasForTelegram(Programas);
      bot.sendMessage(chat_id, mensaje, "");
    } else if (text == "/status") {

      bot.sendMessage(chat_id, "Estoy ready papa!", "");
    }/*else if (text == "/reset") {

      bot.sendMessage(chat_id, "Envio a resetear!", "");
      delay(1000);
      ESP.restart();
    }*/

    /*******************USUARIOS**********************/
    if (text.startsWith("/usuarios")) {
      String respuesta = listarChatIDs(Nodo);

      bot.sendMessage(chat_id, respuesta, "");

    } else if (text.startsWith("/borrarUsuario ")) {
      String id = text.substring(15);
      bool borrado = borrarChatID(Nodo, id);

      if (borrado && guardarConfiguracion(Nodo))
        bot.sendMessage(chat_id, "Chat ID borrado con éxito, uno menos!", "");
      else
        bot.sendMessage(chat_id, "Chat ID no encontrado", "");

    } else if (text.startsWith("/agregarUsuario ")) {
      String chatIDValue = text.substring(16);
      agregarChatID(Nodo, chatIDValue);
      if (guardarConfiguracion(Nodo))
        bot.sendMessage(chat_id, "Chat ID agrego con éxito, ojo que este te puede hacer tomar menos! ", "");
      else
        bot.sendMessage(chat_id, "Chat ID no se pudo agregar", "");

    } else if (text.startsWith("/fecha")) {
      String mensaje = obtenerFechaHora();
      bot.sendMessage(chat_id, mensaje, "");

    } else if (text.startsWith("/bloquear")) {
      Toma1.setBlock(true);
      bot.sendMessage(chat_id, "Se bloqueo, no encendera hasta que la vuelvas a activar. Te vas a ahorar platita para unas birras!", "");

    } else if (text.startsWith("/activar")) {
      Toma1.setBlock(false);
      bot.sendMessage(chat_id, "Se activo con exito! Ya te podes dar unos bidetazos con agua caliente!", "");

    } else if (text.startsWith("/encender")) {
      Toma1.turnOn();
      bot.sendMessage(chat_id, "Se encendio con exito! Ya te podes dar unos bidetazos con agua caliente!", "");

    } else if (text.startsWith("/apagar")) {
      Toma1.turnOff();
      bot.sendMessage(chat_id, "Se apago con exito! Se te va a enfriar la colita!!", "");

    }
  }
}