#include "hal_gpio.h"

void
gpio_set_mode(uint32_t gpioport, uint8_t mode, uint8_t cnf, uint16_t gpios)
{
	uint16_t i, offset = 0;
	uint32_t crl = 0, crh = 0, tmp32 = 0;

	/*
	 * We want to set the config only for the pins mentioned in gpios,
	 * but keeping the others, so read out the actual config first.
	 */
	crl = GPIO_CRL(gpioport);
	crh = GPIO_CRH(gpioport);

	/* Iterate over all bits, use i as the bitnumber. */
	for (i = 0; i < 16; i++) {
		/* Only set the config if the bit is set in gpios. */
		if (!((1 << i) & gpios)) {
			continue;
		}

		/* Calculate bit offset. */
		offset = (i < 8) ? (i * 4) : ((i - 8) * 4);

		/* Use tmp32 to either modify crl or crh. */
		tmp32 = (i < 8) ? crl : crh;

		/* Modify bits are needed. */
		tmp32 &= ~(0xf << offset);	/* Clear the bits first. */
		tmp32 |= (mode << offset) | (cnf << (offset + 2));

		/* Write tmp32 into crl or crh, leave the other unchanged. */
		crl = (i < 8) ? tmp32 : crl;
		crh = (i >= 8) ? tmp32 : crh;
	}

	GPIO_CRL(gpioport) = crl;
	GPIO_CRH(gpioport) = crh;
}

void
gpio_toggle(uint32_t gpioport, uint16_t gpios)
{
	uint32_t port = GPIO_ODR(gpioport);
	GPIO_BSRR(gpioport) = ((port & gpios) << 16 | (~port & gpios));
}

void
gpio_set(uint32_t gpioport, uint16_t gpios)
{
	GPIO_BSRR(gpioport) = gpios;
}

void
gpio_clear(uint32_t gpioport, uint16_t gpios)
{
	GPIO_BSRR(gpioport) = (gpios << 16);
}