# include <stdio.h>
# include <stdlib.h>
# include <portaudio.h>
# include <time.h>

# define SAMPLE_RATE 44100
# define FRAMES_PER_BUFFER 256

/**
 * noiseCallback - callback function that plays whitenoise audio data to
 * main
 * @inputBuffer: audio data fed from PortAudio (no audio)
 * @outputBuffer: audio data to be sent to the speaker
 * @framesPerBuffer: number of samples PortAudio expects when the
 * callback is called
 * @timeinfo: struct with timestamps of when the buffer will be played
 * on the speaker e.g when we need to synchronize audio with video
 * @statusFlags: warning flags set by PortAudio when something goes
 * wrong since last call e.g when looking for audio glitches
 * @userData: data passed from user specified from
 * calling Pa_OpenDefaultStream
 * This function is called back a multiple number of times by PortAudio
 * to send whitenoise audio stream that rings for 3 seconds before exiting
 * the callback function thus describes the audio data that PortAudio
 * will call
 * Return: paContinue the signal info to be sent back to PortAudio
 * indicating it wants to be called again (one of 3 return types)
 */

static int noiseCallback(const void *inputBuffer, void *outputBuffer,
				unsigned long framesPerBuffer,
				const PaStreamCallbackTimeInfo *timeinfo,
				PaStreamCallbackFlags statusFlags,
				void *userData)
{
	float *out = (float *)outputBuffer;

	for (unsigned long i = 0; i < framesPerBuffer; i++)
	{
		float sample = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;

		*out++ = sample;
		*out++ = sample;
	}

	return (paContinue);
}

/**
 * main - function that initializes PortAudio, opens a stream, calls a
 * function with audio data recursively and closes the stream
 *  - Pa_OpenDefaultStream - the function that starts a stream by
 *  connecting PortAudio to the default audio device, and you can
 *  specify your preferred channels, sample format & the callback func.
 *  to be called repeatedly
 *  - Pa_StartStream - function that starts the actual stream
 *  - Pa_Sleep - time the callback function will be instantiated
 *  - Pa_StopStream - stop the stream
 *  - Pa_CloseStream - free up resources used by stream
 * Return: 0 if executed succesfully
 */

int main(void)
{
	PaStream *stream;
	PaError err;

	err = Pa_Initialize();

	if (err != paNoError)
	{
		printf("Feil: %s\n", Pa_GetErrorText(err));
		return (1);
	}

	srand(time(NULL));

	err = Pa_OpenDefaultStream(&stream,
					0,
					2,
					paFloat32,
					SAMPLE_RATE,
					FRAMES_PER_BUFFER,
					noiseCallback,
					NULL);

	if (err != paNoError)
	{
		printf("Kunne ikke åpne stream: %s\n", Pa_GetErrorText(err));
		Pa_Terminate();
		return (1);
	}

	err = Pa_StartStream(stream);
	printf("Stream startet. Kjører i 3 sekunder (stillhet)...\n");
	Pa_Sleep(3000);

	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();

	printf("Stream lukket ordentlig.\n");
	return (0);
}

