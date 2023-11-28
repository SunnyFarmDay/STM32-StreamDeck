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
extern uint32_t targetAudioFrequency;
extern uint16_t fxMode;
extern uint8_t nextSongFlag;
extern uint16_t currentAudioIntensity;

void playPCMFile(char *filename);

#endif /* __PCM_H */
