#ifndef __MP3_H
#define __MP3_H

#include "fatfs.h"
#include <stdint.h>
#include <stdio.h>
#include <lcd.h>
#include <mp3dec.h>

#define bitrate 32 //kbps
#define DECODE_BUFFERSIZE 1024


void playMP3File(char* path);

#endif /* __MP3_H */
