#include "pcm.h"

static volatile uint32_t timerCode = 0;

unsigned int pcmBr = 0;
unsigned int pcmDataToRead = 0;
uint16_t pcmBufferIndex = 0;
uint16_t pcmBuffer[DECODE_BUFFERSIZE];
uint16_t* inBuffer;
uint8_t fillingBuffer = 0;
uint8_t playingFlag = 0;
uint8_t fillBufFlag = 0;
FIL file;
uint8_t res;


uint8_t playPCMFileInit(char *filename)
{

    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK)
    {
        return 0;
    }
    res = f_read(&file, pcmBuffer, DECODE_BUFFERSIZE * 2, &pcmBr);

    if (res != FR_OK)
    {
        return 0;
    }
    if (pcmBr == 0)
    {
        return 0;
    }
    pcmBufferIndex = 0;
    playingFlag = 1;
    fillBufFlag = 0;
    fillingBuffer = 1;
    HAL_I2S_Transmit_DMA(&hi2s2, &pcmBuffer, DECODE_BUFFERSIZE);
    return 1;
}

void playPCMFile(char *filename) {
    playPCMFileInit(filename);
    while (playingFlag)
    {
        if (fillBufFlag)
        {
            res = f_read(&file, inBuffer, DECODE_BUFFERSIZE, &pcmBr);
            if (res != FR_OK)
            {
                playingFlag = 0;
                break;
            }
            if (pcmBr == 0)
            {
                playingFlag = 0;
                break;
            }
            pcmBufferIndex = 0;
            fillBufFlag = 0;
        }
    }
    playPCMFileEnd();
}

void playPCMFileEnd()
{
    playingFlag = 0;
    f_close(&file);
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    inBuffer = &pcmBuffer[0];
    fillingBuffer = 0;
    fillBufFlag = 1;
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    inBuffer = &pcmBuffer[DECODE_BUFFERSIZE / 2];
    fillingBuffer = 1;
    fillBufFlag = 1;
}
