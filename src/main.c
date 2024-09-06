#include "mcu_config.h"
#include "communication.h"

/**
 * Copies a character from input buffer to the output buffer.
 */
static void
task_bluetoothInput(void* args __attribute__((unused)))
{
	for (;;) {
		char ch = getCharFromBt();
		sendCharToConsole(ch);
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
		while (xQueueReceive(termBuff, &ch, portMAX_DELAY) == pdFAIL)
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

	writeString("---=== Fish Feeder ===---\n");
	xTaskCreate(task_bluetoothInput, "bluetooth", 64, NULL, configMAX_PRIORITIES - 1, NULL);
	xTaskCreate(task_consoleOuput, "console", 64, NULL, configMAX_PRIORITIES - 1, NULL);
	vTaskStartScheduler();
	
	for (volatile uint8_t i = 0; ; ++i) { }

	return (0);
}