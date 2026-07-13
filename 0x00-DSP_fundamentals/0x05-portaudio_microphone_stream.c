# include "dsp.h"

# define FRAMES_PER_BUFFER 1024

/**
 * passThroughCallback - callback function to send a microphone's audio sample
 * as input and yield it back as an output signal (passthrough program)
 * @inputBuffer: audio data fed from PortAudio (no audio)
 * @outputBuffer: audio data to be sent to the speaker
 * @framesPerBuffer: number of samples PortAudio expects when the
 * callback is called
 * @timeInfo: struct with timestamps of when the buffer will be played
 * on the speaker e.g when we need to synchronize audio with video
 * @statusFlags: warning flags set by PortAudio when something goes
 * wrong since last call e.g when looking for audio glitches
 * @userData: data passed from user specified from
 * calling Pa_OpenDefaultStream
 * This function is called back a multiple number of times by PortAudio
 * to send an empty audio stream that rings for 3 seconds before exiting
 * the callback function thus describes the audio data that PortAudio
 * will call
 * Return: paContinue the signal info to be sent back to PortAudio
 * indicating it wants to be called again (one of 3 return types)
 */

static int passThroughCallback(const void *inputBuffer, void *outputBuffer,
				unsigned long framesPerBuffer,
				const PaStreamCallbackTimeInfo *timeInfo,
				PaStreamCallbackFlags statusFlags,
				void *userData)
{
	const float *in = (const float *)inputBuffer;
	float *out = (float *)outputBuffer;

	if (in == NULL)
	{
		for (unsigned long i = 0; i < framesPerBuffer; i++)
		{
			*out++ = 0.0f;
			*out++ = 0.0f;
		}
	}
	else
	{
		for (unsigned long i = 0; i < framesPerBuffer; i++)
		{
			*out++ = *in++;
			*out++ = *in++;
		}
	}

	return (paContinue);
}

/**
 * main - function that initializes PortAudio, opens a stream, calls a
 * function with audio data recursively and closes the stream
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

	err = Pa_OpenDefaultStream(&stream,
					2,
					2,
					paFloat32,
					SAMPLE_RATE,
					FRAMES_PER_BUFFER,
					passThroughCallback,
					NULL);

	if (err != paNoError)
	{
		printf("Kunne ikke åpne stream: %s\n", Pa_GetErrorText(err));
		Pa_Terminate();
		return (1);
	}

	Pa_StartStream(stream);
	printf("Passthrough aktiv i 10 sekunder - snakk i mikrofonen... \n");
	Pa_Sleep(10000);
	Pa_StopStream(stream);

	Pa_CloseStream(stream);
	Pa_Terminate();

	printf("Stream lukket ordentlig.\n");
	return (0);
}

