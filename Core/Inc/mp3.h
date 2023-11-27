//#ifndef __MP3_H
//#define __MP3_H
//
//#include "fatfs.h"
//#include <stdint.h>
//#include <stdio.h>
//#include "lcdtp.h"
//#include <mp3dec.h>
//#include "main.h"
//
//#define bitrate 32 //kbps
//#define DECODE_BUFFERSIZE 4608
//
//
//typedef __packed struct
//{
//	uint8_t id[3];			//ID
//	uint8_t mversion;		//主版本号
//	uint8_t sversion;		//子版本号
//	uint8_t flags;			//标签头标志
//	uint8_t size[4];		//标签信息大小(不包含标签头10字节).所以,标签大小=size+10.
//} ID3V2_TagHead;
//
//typedef struct
//{
//	uint32_t bitsPerSample;
//	uint32_t OutSamples;
//	uint32_t nChans;
//	uint32_t DataStart;
//	uint32_t samprate;
//} MP3Info;
//
//
//void playMP3File(char* path);
//
//#endif /* __MP3_H */
