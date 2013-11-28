#include <bcm2835.h>

// Blinks on RPi pin GPIO 11
#define DIR RPI_GPIO_P1_05
#define STEP RPI_GPIO_P1_03


int main(int argc, char **argv)
{
	// If you call this, it will not actually access the GPIO
	// Use for testing
	//    bcm2835_set_debug(1);

	if (!bcm2835_init())
		return 1;

	// Set the pin to be an output
	bcm2835_gpio_fsel(STEP, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(DIR, BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_write(DIR, LOW);
	bcm2835_gpio_write(STEP, LOW);

	// Blink
	while (1)
	{
		// Turn it on
		bcm2835_gpio_write(STEP, HIGH);

		// wait a bit
		delay(1);

		// turn it off
		bcm2835_gpio_write(STEP, LOW);

		// wait a bit
		delay(1);
	}

	return 0;
}
