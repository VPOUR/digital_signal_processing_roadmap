# include "dsp.h"

# define FREQUENCY 440.0

/**
 * sineCallback - callback function that sends sine wave data to main
 * @inputBuffer: audio data fed from PortAudio (no audio)
 * @outputBuffer: audio data to be sent to the speaker
 * @framesPerBuffer: number of samples PortAudio expects: when the
 * callback is called
 * @timeinfo: struct with timestamps of when the buffer will be played
 * on the speaker e.g when we need to synchronize audio with video
 * @statusFlags: warning flags set by PortAudio when something goes
 * wrong since last call e.g when looking for audio glitches
 * @userData: data passed from user specified from
 * calling Pa_OpenDefaultStream
 * This function is called back a multiple number of times by PortAudio
 * to send a A4 tone wave (440Hz) sinewave audio stream that
 * rings for 3 seconds before exiting. The callback function thus
 * describes the audio data that PortAudio will call
 * Return: paContinue the signal info to be sent back to PortAudio
 * indicating it wants to be called again (one of 3 return types)
 */

static int sineCallback(const void *inputBuffer, void *outputBuffer,
				unsigned long framesPerBuffer,
				const PaStreamCallbackTimeInfo *timeinfo,
				PaStreamCallbackFlags statusFlags,
				void *userData)
{
	PaData *data = (PaData *)userData;
	float *out = (float *)outputBuffer;

	for (unsigned long i = 0; i < framesPerBuffer; i++)
	{
		float sample = sinf(data->phase);
		*out++ = sample;
		*out++ = sample;

		data->phase += 2.0f * (float)M_PI * FREQUENCY / SAMPLE_RATE;
		if (data->phase >= 2.0f * (float)M_PI)
			data->phase -= 2.0f * (float)M_PI;
	}

	return (paContinue);
}

/**
 * main - function that initializes PortAudio, opens a stream, calls a
 * function with audio data recursively and closes the stream
 * This is the main function of a program that plays a 440Hz sinewave
 * for 3 seconds
 * Return: 0 if executed succesfully
 */

int main(void)
{
	PaStream *stream;
	PaError err;
	PaData data = {0};

	err = Pa_Initialize();

	if (err != paNoError)
	{
		printf("Feil: %s\n", Pa_GetErrorText(err));
		return (1);
	}

	err = Pa_OpenDefaultStream(&stream,
					0,
					2,
					paFloat32,
					SAMPLE_RATE,
					paFramesPerBufferUnspecified,
					sineCallback,
					&data);

	if (err != paNoError)
	{
		printf("Kunne ikke åpne stream: %s\n", Pa_GetErrorText(err));
		Pa_Terminate();
		return (1);
	}

	Pa_StartStream(stream);
	printf("Spiller A4 tone (440Hz i 3 sekunder)...\n");
	Pa_Sleep(3000);

	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();

	printf("Stream lukket ordentlig.\n");
	return (0);
}

