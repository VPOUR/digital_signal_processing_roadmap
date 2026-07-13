# include "dsp.h"

/**
 * main - program that checks if PortAudio has been initialized
 * successfully using;
 * - Pa_Initialize -  to prepare PortAudio to
 * comunicate with the audio system of the OS by;
 *   - initializing Host APIs and setting up available audio backends
 *   (JACK, PulseAudio, ALSA etc)
 *   - discovers audio devices (all connected sound hardware)
 *   - allocate internal memory for real time audio streams
 *   - resetting device indices (setting the baseline index
 *   used to target specific input and output devices)
 * Return: 0 if successfully executed
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

	printf("PortAudio startet uten problemer.\n");
	printf("Versjon: %s\n", Pa_GetVersionText());

	Pa_Terminate();
	printf("PortAudio avsluttet.\n");

	return (0);
}
