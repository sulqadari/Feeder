#include "mcu_config.h"
#include "communication.h"

/**
 * Copies a character from input buffer to the output buffer.
 */
static void
task_bluetoothInput(void* args __attribute__((unused)))
{
	writeString("---=== Fish Feeder ===---\n");
	for (;;) {
		char ch = readCharacter();
		writeCharacter(ch);

	/* TODO: the below flow control doesn't work because an application
	 * doesn't send '\r\n' sequence. Thus there is two workarounds:
	 * 1. padding \r\n\ when the length on queue equals 0.
	 * 2. \r\n\ sequence shall be sent from application. */
		if (ch == '\r' || ch == '\n' || ch == '\0')
			writeString("\n>> ");
	}
}

/**
 * Outputs to the console a data received from the BT.
 * This function blocks until queue has another portion of data
 * to be sent.
 */
static void 
task_consoleOuput(void* args __attribute__((unused)))
{
	char ch;

	for (;;) {
		while (xQueueReceive(queue_TX2, &ch, portMAX_DELAY) == pdFAIL)
			taskYIELD();
		
		while (!usart_get_flag(USART1, USART_SR_TXE))
			taskYIELD();

		usart_send(USART1, ch);
	}
}

int
main(void)
{
	feeder_init();

	xTaskCreate(task_consoleOuput, "console", 50, NULL, configMAX_PRIORITIES - 1, NULL);
	xTaskCreate(task_bluetoothInput, "bluetooth", 50, NULL, configMAX_PRIORITIES - 1, NULL);
	vTaskStartScheduler();
	
	for (volatile uint8_t i = 0; ; ++i) { }

	return (0);
}