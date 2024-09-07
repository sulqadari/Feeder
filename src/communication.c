#include "communication.h"

/**
 * Receives a string of characters from Bluetooth module.
 * When the CH-5 sends the string to STM32, an interruption
 * arises which in turn calls this function.
 * Within the while loop each successive byte is written into
 * btoothBuff buffer.
 */
void
USART2_IRQHandler(void)
{
	char ch;
	BaseType_t hpTask = pdFALSE;

	if ((USART_SR(USART2) & USART_SR_RXNE) != 0) {
		ch = usart_recv(USART2);
		xQueueSendToBackFromISR(btoothBuff, &ch, &hpTask);
	}

	gpio_toggle(GPIOC, GPIO13);
}

char
getCharFromBt(void)
{
	char ch;

	/* Fetch a char which was stored in the buffer from the last call
	 * of USART2_IRQHandler(). */
	while (xQueueReceive(btoothBuff, &ch, 0) == pdFAIL)
		taskYIELD();

	return ch;
}

/**
 * Writes a subsequent character into output buffer.
 * If queue is full, then this function passes control over
 * to the successive task.
 */
void
sendCharToConsole(char ch)
{
_again:
	while (xQueueSendToBack(termBuff, &ch, 0) == pdFAIL)
		taskYIELD();

	switch (ch) {
		case '\n':
			ch = '\r';
		goto _again;
		case '\0':
			ch = '\n';
		goto _again;
	}
}

void
writeString(char* str)
{
	while (*str != '\0')
		sendCharToConsole(*str++);
}