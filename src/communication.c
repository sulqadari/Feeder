#include "communication.h"

/**
 * Receives a string of characters from Bluetooth module.
 * When the CH-5 sends the string to STM32, an interruption
 * arises which in turn calls this function.
 * Within the while loop each successive byte is written into
 * queue_RX2 buffer.
 */
void
USART2_IRQHandler(void)
{
	char ch;
	BaseType_t hpTask = pdFALSE;

	if ((USART_SR(USART2) & USART_SR_RXNE) != 0) {
		ch = usart_recv(USART2);
		xQueueSendToBackFromISR(queue_RX2, &ch, &hpTask);
	}

	gpio_toggle(GPIOC, GPIO13);
}

char
readCharacter(void)
{
	char ch;

	while (xQueueReceive(queue_RX2, &ch, 0) == pdFAIL)
		taskYIELD();

	return ch;
}

/**
 * Writes a subsequent character into output buffer.
 * If queue is full, then this function passes control over
 * to the successive task.
 */
void
writeCharacter(char ch)
{
	while (xQueueSendToBack(queue_TX2, &ch, 0) == pdFAIL)
		taskYIELD();
	
	if (ch == '\0') {
		ch = '\n';
		xQueueSendToBack(queue_TX2, &ch, 0);
	}
}

void
writeString(char* str)
{
	while (*str != '\0')
		writeCharacter(*str++);
}