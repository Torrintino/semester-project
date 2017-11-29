#ifndef IRSENDER_H
#define IRSENDER_H

#include <string.h>
#include <math.h>
#include <pigpio.h>

#uses pigpio library 
#define MAX_COMMANDS 512
//Max Pulses aus pigpio
#define MAX_PULSES 12000

//Puls zur Wellengenerierung hinzufügen
static inline void addPulse(uint32_t onPins, uint32_t offPins, uint32_t duration, gpioPulse_t *irSignal, int *pulseCount)
{
int index = *pulseCount;

	irSignal[index].gpioOn = onPins;
	irSignal[index].gpioOff = offPins;
	irSignal[index].usDelay = duration;

	(*pulseCount)++;
}

//Welle generieren mit:
//Dauer in Microsekunden = duration
//Frequenz in Hz = frequency
// Als Output auf: outPin
static inline void carrierFrequency(uint32_t outPin, int frequency, double dutyCycle, double duration, gpioPulse_t *irSignal, int *pulseCount)
{
	double oneCycleTime = 1000000.0/frequency; //1000000 Microsekunden in einer Sekunde
	int onDuration = (int)round(oneCycleTime * dutyCycle);
	int offDuration = (int)round(oneCycleTime * (1.0 - dutyCycle));

	int totalCycles = (int)round(duration / oneCycleTime);
	int totalPulses = totalCycles * 2;

	for(int i = 0; i < totalPulses; i++)
	{
		if(i%2 == 0)
		{
		//High Signal
		addPulse(1 << outPin, 0, onDuration, irSignal, pulseCount);
		}
		else
		{
		//Low Signal
		addPulse(0, 1 << outPin, offDuration, irSignal, pulseCount);
		}
	}
}

static inline void gap(uint32_t outPin, double duration, gpioPulse_t *irSignal, int *pulseCount)
{
	addPulse(0, 0, duration, irSignal, pulseCount);
}

static inline int irSend(uint32_t outPin,
int frequency,
double dutyCycle,
const int *pulses,
int numPulses)
{
	gpioPulse_t irSignal[MAX_PULSES];
	int pulseCount = 0;

	for(int i; i<numPulses; i++)
	{
		if(i%2 == 0)
		{
			carrierFrequency(outPin, frequency, dutyCycle, pulses[i], irSignal, &pulseCount);
		}
		else
		{
			gap(outPin, pulses[i], irSignal, &pulseCount);
		}
	}

	printf("pulse count is %i\n", pulseCount);

	gpioInitialise();

	gpioSetMode(outPin, PI_OUTPUT);

	gpioWaveClear();

	gpioWaveAddGeneric(pulseCount, irSignal);
	int waveId =gpioWaveCreate();

	gpioWaveTxSend(waveId, PI_WAVE_MODE_ONE_SHOT);
	while(gpioWaveTxBusy())
	{
	time_sleep(0.1);
	}
	gpioWaveDelete(waveId);
	gpioTerminate();
	return 0;
}

#endif


