#include <Arduino.h>
#include <stdio.h>

void setup();
void loop();

uint8_t SIM_SFR[65536] = {0};
uint8_t SIM_MEM[65536] = {0};

void cli()
{
    fprintf(stderr, "[MCU] Global interrupt disabled\n");
}

void sei()
{
    fprintf(stderr, "[MCU] Global interrupt enabled\n");
}

int initArduino(void)
{
    unsigned int loopcount = 0;

	fprintf(stderr, "ARDUSIM 0.1 - A Practical Arduino Simulator (C)2012 Written by Stanley Huang\n\n");

	init();
#if defined(USBCON)
	USB.attach();
#endif

	setup();
//
//	for (;;) {
//		loop();
//		//if (serialEventRun) serialEventRun();
//		fprintf(stderr, "Loop %d\r", ++loopcount);
//		usleep(50000);
//	}
	return 0;
}
