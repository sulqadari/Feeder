#include "mcu_config.h"
#include "communication.h"

/**
 * Copies a character from input buffer to the output buffer.
 */
static void
task_bluetoothInput(void* args __attribute__((unused)))
{
	for (;;) {
		char ch = readCharacter();
		writeCharacter(ch);
	}
}

/**
 * Outputs a data received from the BT to the console.
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

static void
task_blink(void* args __attribute((unused)))
{
	for (;;) {
		vTaskDelay(pdMS_TO_TICKS(200));
		gpio_toggle(GPIOC, GPIO13);
	}
}

int
main(void)
{
	feeder_init();

	xTaskCreate(task_consoleOuput, "console", 50, NULL, configMAX_PRIORITIES - 1, NULL);
	xTaskCreate(task_bluetoothInput, "bluetooth", 50, NULL, configMAX_PRIORITIES - 1, NULL);
	xTaskCreate(task_blink, "blink", 50, NULL, configMAX_PRIORITIES - 1, NULL);
	vTaskStartScheduler();
	
	for (volatile uint8_t i = 0; ; ++i) { }

	return (0);
}