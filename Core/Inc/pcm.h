#ifndef __PCM_H
#define __PCM_H

#include "fatfs.h"
#include <stdint.h>
#include "main.h"
#include "pcm.h"

#define AUDIO_FREQUENCY        I2S_AUDIOFREQ_48K
#define DECODE_BUFFERSIZE     1024*16

extern unsigned int pcmBr;        
extern unsigned int pcmDataToRead;
extern uint16_t pcmBufferIndex;
extern uint8_t playingFlag;

void playPCMFile(char *filename);

#endif /* __PCM_H */
