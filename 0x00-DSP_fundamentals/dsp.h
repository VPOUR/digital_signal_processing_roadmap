# ifndef DSP_H
# define DSP_H

# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include <portaudio.h>


# define SAMPLE_RATE 44100

/**
 * struct padata_s - short description of audiophase data
 * @phase: the current phase angle/value of the signal
 */
typedef struct padata_s
{
	float phase;
} PaData;

# endif /* DSP_H */
