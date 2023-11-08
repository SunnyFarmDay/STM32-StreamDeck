
#include "fatfs.h"
#include <stdint.h>
#include <lcd.h>
// #include <mp3dec.h>

#define bitrate 32 //kbps
#define DECODE_BUFFERSIZE 1024

FRESULT scanMP3Files(char* path);
void playMP3File(char* path);
