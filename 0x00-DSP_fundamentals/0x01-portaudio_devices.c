# include "dsp.h"

/**
 * main - program that lists all devices that are found by PortAudio
 * uses functions like;
 *  - Pa_GetDeviceCount - PortAudio checks for how many audio
 *  devices it sees, including input and output devices
 *  - Pa_GetDeviceInfo - for every i, this has a pointer to a struct
 *  showing full details of the specific device as we loop from 0
 *  to numDevices
 *  - Pa_GetDefaultInputDevice, Pa_GetOutputDevice - returns the idex of
 *  the devices considered standard, used by default under the hood
 *  Return: 0 if successfully executed
 */

int main(void)
{
	PaError err;

	err = Pa_Initialize();
	if (err != paNoError)
	{
		printf("Feil ved initialisering: %s\n", Pa_GetErrorText(err));
		return (1);
	}

	int numDevices = Pa_GetDeviceCount();

	if (numDevices < 0)
	{
		printf("Feil: Pa_GetDeviceCount returnerte %d\n", numDevices);
		Pa_Terminate();
		return (1);
	}

	printf("Fant %d lydenheter:\n\n", numDevices);

	for (int i = 0; i < numDevices; i++)
	{
		const PaDeviceInfo *info = Pa_GetDeviceInfo(i);

		printf("Enhet %d: %s\n", i, info->name);
		printf(" Maks input-kanaler: %d\n", info->maxInputChannels);
		printf(" Maks output-kanaler: %d\n", info->maxOutputChannels);
		printf(" Standard samplerate: %.0f Hz\n", info->defaultSampleRate);
		printf("\n");
	}

	int defaultInput = Pa_GetDefaultInputDevice();
	int defaultOutput = Pa_GetDefaultOutputDevice();

	printf("Standard input-enhet: %d\n", defaultInput);
	printf("Standard output-enhet: %d\n", defaultOutput);

	Pa_Terminate();
	return (0);
}
