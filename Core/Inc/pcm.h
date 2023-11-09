#ifndef __PCM_H
#define __PCM_H

#include "stm32f1xx_hal.h"
#include "fatfs.h"
#include <stdint.h>
#include "main.h"
#include "pcm.h"

#define AudioBitRate 32 //kbps
#define DECODE_BUFFERSIZE 1024

extern unsigned int pcmBr;        
extern unsigned int pcmDataToRead;
extern uint16_t pcmBufferIndex;
extern uint8_t pcmBuffer[DECODE_BUFFERSIZE];
extern uint8_t playingFlag;

void playPCMFile(char *filename);

#endif /* __PCM_H */