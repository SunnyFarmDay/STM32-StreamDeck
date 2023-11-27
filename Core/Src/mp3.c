//
//#include "mp3.h"
//#include <string.h>
//// HMP3Decoder mp3Decoder;
//
//// uint8_t mp3DecodeBuf[DECODE_BUFFERSIZE];
//
//// FIL mp3file;
//// FRESULT res;
//// MP3Info mp3Info;
//// uint8_t* readptr;
//// int32_t byteLeft;
//// uint8_t	initMp3InfoFlag;
//// HMP3Decoder mp3Decoder;
//// uint32_t dmaBufSize;
//// uint32_t mp3DataStart = 0;
//// ID3V2_TagHead tagHead;
//
//// u_char mp3ReadBuffer[DECODE_BUFFERSIZE];
//// short int audioBuffer[2][MAX_NCHAN * MAX_NSAMP];
//
//// void playMP3File(char* path) {
////     char str[100];
////     sprintf(str, "Playing %s", path);
////     LCD_DrawString(0, 200, str);
//
////     uint8_t res=0;
//// 	uint32_t br=0;
//// 	uint32_t malloc=0;
//
////     res = f_open(&mp3file, path, FA_READ);
////     if (res != FR_OK) {
////         sprintf(str, "File open error: %d", res);
////         LCD_DrawString(0, 215, str);
////         return;
////     }
//
////     res = f_read(&mp3file, mp3ReadBuffer, DECODE_BUFFERSIZE, &br);
////     tagHead = (ID3V2_TagHead*)mp3ReadBuffer;
//
////     if(strncmp("ID3",(const char*)tagHead->id,3)==0)
//// 		{
//// 			mp3Info.DataStart=((uint32_t)tagHead->size[0]<<21)|((uint32_t)tagHead->size[1]<<14)|((uint32_t)tagHead->size[2]<<7)|tagHead->size[3];
//// 			f_lseek(&mp3file, mp3DataStart);
//// 			res=f_read(&mp3file,mp3ReadBuffer,DECODE_BUFFERSIZE,&br);
//// 			if(res!=FR_OK||br==0)
//// 			{
//// 				printf("Read file failed!\r\n");
//// 				res=3;
//// 			}
//// 		}
////     readptr = mp3ReadBuffer;
////     mp3Decoder = MP3InitDecoder();
////     while (1) {
////         res = f_read(&mp3file, mp3ReadBuffer, DECODE_BUFFERSIZE, &br);
////         if (res != FR_OK) {
////             sprintf(str, "File read error: %d", res);
////             LCD_DrawString(0, 215, str);
////             return;
////         }
////         if (br == 0) {
////             break;
////         }
////         dataToRead = br;
////         while (dataToRead > 0) {
////             int offset = 0;
////             int err = MP3Decode(mp3Decoder, &mp3ReadBuffer, &dataToRead, (short int*)audioBuffer, 0);
////             if (err != ERR_MP3_NONE) {
////                 sprintf(str, "MP3 decode error: %d", err);
////                 LCD_DrawString(0, 215, str);
////                 return;
////             }
////             offset += dataToRead;
////         }
////     }
//// }
//
//uint8_t Mp3DecodeBuf[DECODE_BUFFERSIZE];
//uint8_t outputBuf[DECODE_BUFFERSIZE];
//uint8_t TempBuf[DECODE_BUFFERSIZE];
//FIL Mp3File;
//MP3Info mp3Info;
//uint8_t *Readptr; // MP3解码读指针
//int32_t ByteLeft; // buffer还剩余的有效数据
//uint8_t InitMp3InfoFlag;
//HMP3Decoder Mp3Decoder;
//uint32_t DmaBufSize;
//
//uint8_t CloseFileFlag = 0;
//uint8_t EndFileFlag = 0;
//uint8_t FillBufFlag = 0xFF;
//
//uint8_t FillMp3Buf(uint8_t *Buf)
//{
//	uint32_t br = 0;
//	int32_t Offset;
//	int32_t err = 0;
//	int32_t i;
//	uint16_t *PlayPtr;
//	uint16_t *Mp3Ptr;
//	MP3FrameInfo Mp3FrameInfo;
//
//	Offset = MP3FindSyncWord(Readptr, ByteLeft); // 在readptr位置,开始查找同步字符
//	if (Offset < 0)								 // 没有找到同步字符,跳出帧解码循环
//	{
//		printf("Can not play the file!\r\n");
//		return 1;
//	}
//	Readptr += Offset;	// MP3读指针偏移到同步字符处.
//	ByteLeft -= Offset; // buffer里面的有效数据个数,必须减去偏移量
//	// printf("ByteLeft=%d\r\n",ByteLeft);
//	if (ByteLeft < MAINBUF_SIZE * 2) // 当数组内容小于2倍MAINBUF_SIZE的时候,必须补充新的数据进来.
//	{
//		memmove(TempBuf, Readptr, ByteLeft);
//		f_read(&Mp3File, TempBuf + ByteLeft, DECODE_BUFFERSIZE - ByteLeft, &br); // 补充余下的数据
//		// printf("rd=%d,br=%d\r\n",WAVEFILEBUFSIZE-ByteLeft,br);
//		if (br < DECODE_BUFFERSIZE - ByteLeft)
//		{
//			memset(TempBuf + ByteLeft + br, 0, DECODE_BUFFERSIZE - ByteLeft - br);
//			EndFileFlag = 1;
//		}
//		ByteLeft = DECODE_BUFFERSIZE;
//		Readptr = TempBuf;
//	}
//	err = MP3Decode(Mp3Decoder, &Readptr, &ByteLeft, (int16_t *)Mp3DecodeBuf, 0); // 解码一帧MP3数据
//	// printf("Decode error:%d\r\n",err);
//	if (err != 0)
//	{
//		printf("Decode error:%d\r\n", err);
//		return 2;
//	}
//
//	if (InitMp3InfoFlag == 0)
//	{
//		MP3GetLastFrameInfo(Mp3Decoder, &Mp3FrameInfo); // 得到刚刚解码的MP3帧信息
//		mp3Info.bitsPerSample = Mp3FrameInfo.bitsPerSample;
//		mp3Info.nChans = Mp3FrameInfo.nChans;
//		mp3Info.OutSamples = Mp3FrameInfo.outputSamps;
//		mp3Info.samprate = Mp3FrameInfo.samprate;
//		printf("bitsPerSample=%d\r\n", mp3Info.bitsPerSample);
//		printf("nChans=%d\r\n", mp3Info.nChans);
//		printf("OutSamples=%d\r\n", mp3Info.OutSamples);
//		printf("samprate=%d\r\n", mp3Info.samprate);
//		InitMp3InfoFlag = 1;
//		if (mp3Info.bitsPerSample != 16)
//		{
//			printf("Can not play the file!\r\n");
//			return 3;
//		}
//	}
//	Mp3Ptr = (uint16_t *)Mp3DecodeBuf;
//	PlayPtr = (uint16_t *)Buf;
//	if (mp3Info.nChans == 2)
//	{
//		for (i = 0; i < mp3Info.OutSamples; i++)
//		{
//			PlayPtr[i] = Mp3Ptr[i];
//		}
//	}
//	else
//	{
//		for (i = 0; i < mp3Info.OutSamples; i++)
//		{
//			PlayPtr[0] = Mp3Ptr[0];
//			PlayPtr[1] = Mp3Ptr[0];
//			PlayPtr += 2;
//			Mp3Ptr += 1;
//		}
//	}
//	return 0;
//}
//
//uint8_t PlayMp3File(char *path)
//{
//	uint8_t res = 0;
//	uint32_t br = 0;
//	uint32_t Mp3DataStart = 0;
//	CloseFileFlag = 0;
//	EndFileFlag = 0;
//	FillBufFlag = 0xFF;
//	ID3V2_TagHead *TagHead;
//	res = f_open(&Mp3File, (TCHAR *)path, FA_READ);
//	while (1)
//	{
//		if (res != FR_OK)
//		{
//			printf("Open file failed!\r\n");
//			res = 1;
//			break;
//		}
//		CloseFileFlag = 1;
//		res = f_read(&Mp3File, TempBuf, DECODE_BUFFERSIZE, &br);
//		if (res != FR_OK)
//		{
//			printf("Read file failed!\r\n");
//			res = 2;
//			break;
//		}
//		TagHead = (ID3V2_TagHead *)TempBuf;
//		// 得到MP3数据的开始位置
//		if (strncmp("ID3", (const char *)TagHead->id, 3) == 0)
//		{
//			mp3Info.DataStart = ((uint32_t)TagHead->size[0] << 21) | ((uint32_t)TagHead->size[1] << 14) | ((uint32_t)TagHead->size[2] << 7) | TagHead->size[3];
//			f_lseek(&Mp3File, Mp3DataStart);
//			res = f_read(&Mp3File, TempBuf, DECODE_BUFFERSIZE, &br);
//			if (res != FR_OK || br == 0)
//			{
//				printf("Read file failed!\r\n");
//				res = 3;
//				break;
//			}
//		}
//		Mp3Decoder = MP3InitDecoder();
//		if (Mp3Decoder == 0)
//		{
//			printf("Init Mp3Decoder failed!\r\n");
//			res = 4;
//			break;
//		}
//		Readptr = TempBuf;
//		ByteLeft = br;
//		InitMp3InfoFlag = 0;
//		// printf("still status %d OK?\r\n", res);
//		while (EndFileFlag == 0)
//		{
//			res = FillMp3Buf(outputBuf);
//			// printf("still status %d OK?\r\n", res);
//			if (res == 0)
//				break;
//		}
//		res = My_I2S2_Init(mp3Info.bitsPerSample, mp3Info.samprate);
//		printf("%d\r\n", res);
//		DmaBufSize = mp3Info.OutSamples * mp3Info.bitsPerSample * 4 / (8 * mp3Info.nChans);
//		printf("DmaBufSize=%d\r\n", DmaBufSize);
//		FillMp3Buf(outputBuf + DmaBufSize / 2);
//		break;
//	}
//	if (res != 0)
//	{
//		printf("res=%d\r\n", res);
//		if (CloseFileFlag)
//		{
//			f_close(&Mp3File);
//			CloseFileFlag = 0;
//		}
//		MP3FreeDecoder(Mp3Decoder);
//		return res;
//	}
//	HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t *)outputBuf, DmaBufSize / 2);
//	while (1)
//	{
//		if (EndFileFlag == 0)
//		{
//			if (FillBufFlag == 0)
//			{
//				FillMp3Buf(outputBuf);
//				FillBufFlag = 0xFF;
//			}
//			else if (FillBufFlag == 1)
//			{
//				FillMp3Buf(&outputBuf[DmaBufSize / 2]);
//				FillBufFlag = 0xFF;
//			}
//		}
//		else if (EndFileFlag == 3)
//		{
//			HAL_I2S_DMAStop(&hi2s2);
//			// printf("sr=%d\r\n",hi2s2.Instance->SR);
//			// printf("cr2=%d\r\n",hi2s2.Instance->CR2);
//			// printf("cfgr=%d\r\n",hi2s2.Instance->I2SCFGR);
//			__HAL_I2S_ENABLE(&hi2s2);
//			res = I2S_WaitFlagStateUntilTimeout(&hi2s2, I2S_FLAG_BSY, SET, 20);
//			// printf("res=%d\r\n",res);
//			__HAL_I2S_DISABLE(&hi2s2);
//			// printf("sr=%d\r\n",hi2s2.Instance->SR);
//			break;
//		}
//	}
//	if (CloseFileFlag)
//		f_close(&Mp3File);
//	MP3FreeDecoder(Mp3Decoder);
//	return 0;
//}
//
