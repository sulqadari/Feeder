#ifndef FREEDER_CONNECTIONS_H
#define FREEDER_CONNECTIONS_H

#include "mcu_config.h"

void sendCharToConsole(char ch);
char getCharFromBt(void);
void writeString(char* str);

#endif // !FREEDER_CONNECTIONS_H