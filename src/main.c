#include "mcu_config.h"

static void
task_bluetoothInput(void* args __attribute__((unused)))
{
	for (;;) {
		char ch = readCharacter();
		writeCharacter(ch);
	}
}

static void
task_consoleOuput(void* args __attribute__((unused)))
{
	char ch;

	for (;;) {
		while (xQueueReceive(queue_TX2, &ch, portMAX_DELAY) != pdPASS)
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