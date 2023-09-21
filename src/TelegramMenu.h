#ifndef TelegramMenu_h
#define TelegramMenu_h

#include <UniversalTelegramBot.h>
#include <config.h>

void sendSimpleMenu(UniversalTelegramBot& bot, String chat_id);
void sendMenu(UniversalTelegramBot &bot, String chat_id) ;
void handleNewMessages(UniversalTelegramBot& bot, int numNewMessages);
void sendInitTelegram(UniversalTelegramBot &bot,const NodoConfig& Nodo); 
#endif
