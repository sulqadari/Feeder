#ifndef FREEDER_CONNECTIONS_H
#define FREEDER_CONNECTIONS_H

#include "mcu_config.h"

void writeCharacter(char ch);
char readCharacter(void);
void writeString(char* str);

#endif // !FREEDER_CONNECTIONS_H