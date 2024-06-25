#include "mcu_config.h"

int
main(void)
{
	feeder_init();

	// xTaskCreate(task_main, "MAIN", 100, NULL, configMAX_PRIORITIES - 1, NULL);
	// xTaskCreate(task_transmit, "TRANSMIT", 100, NULL, configMAX_PRIORITIES - 1, NULL);
	
	// vTaskStartScheduler();
	
	for (;;);

	return (0);
}