#include "mcu_config.h"

QueueHandle_t queue_TX2, queue_RX2;

static void
init_LED(void)
{
	gpio_set_mode(
		GPIOC,
		GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO13
	);

	gpio_set(GPIOC, GPIO13);
}

static void
init_ATcomamnds(void)
{
	gpio_set_mode(
		GPIOA,
		GPIO_MODE_OUTPUT_10_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO5
	);

	gpio_set(GPIOA, GPIO5);
}

static void
init_clock(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_USART1);
}

static void
init_uart1_console(void)
{
	nvic_enable_irq(NVIC_USART1_IRQ);

	gpio_set_mode(
		GPIOA,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		GPIO_USART1_TX
	);

	gpio_set_mode(
		GPIOA,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT,
		GPIO_USART1_RX
	);

	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX_RX);
	usart_set_parity(USART1, USART_PARITY_NONE);

	usart_enable_rx_interrupt(USART1);
	usart_enable(USART1);
}

static void
init_uart2_bt(void)
{
	nvic_enable_irq(NVIC_USART2_IRQ);

	gpio_set_mode(
		GPIOA,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		GPIO_USART2_TX
	);

	gpio_set_mode(
		GPIOA,
		GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT,
		GPIO_USART2_RX
	);

	usart_set_baudrate(USART2, 115200);
	usart_set_databits(USART2, 8);
	usart_set_stopbits(USART2, USART_STOPBITS_1);
	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_parity(USART2, USART_PARITY_NONE);

	usart_enable_rx_interrupt(USART2);
	usart_enable(USART2);
}

void
feeder_init(void)
{
	queue_TX2 = xQueueCreate(64, sizeof(char));
	queue_RX2 = xQueueCreate(64, sizeof(char));

	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

	init_clock();
	init_LED();
	init_ATcomamnds();
	init_uart1_console();
	init_uart2_bt();
}