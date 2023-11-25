#include "pcm.h"

static volatile uint32_t timerCode = 0;

unsigned int pcmBr = 0;
unsigned int dataToRead = 0;
uint16_t pcmBufferIndex = 0;
uint16_t decodeBuffer[DECODE_BUFFERSIZE];
uint16_t pcmBuffer[PCM_BUFFERSIZE];
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
    res = f_read(&file, decodeBuffer, DECODE_BUFFERSIZE, &pcmBr);

    if (res != FR_OK)
    {
        return 0;
    }
    if (pcmBr == 0)
    {
        return 0;
    }
    dataToRead = pcmBr;
    pcmBufferIndex = 0;
    playingFlag = 1;
    fillBufFlag = 0;
    fillingBuffer = 1;
    memcpy(pcmBuffer, decodeBuffer, PCM_BUFFERSIZE * sizeof(uint16_t));
    HAL_I2S_Transmit_DMA(&hi2s2, &pcmBuffer[0], PCM_BUFFERSIZE);
    dataToRead -= PCM_BUFFERSIZE;
    pcmBufferIndex += PCM_BUFFERSIZE;
    return 1;
}

void playPCMFile(char *filename) {
    playPCMFileInit(filename);
    while (playingFlag)
    {
        if (fillBufFlag)
        {
            memcpy(inBuffer, decodeBuffer + pcmBufferIndex, PCM_BUFFERSIZE * sizeof(uint16_t) / 2);
            dataToRead -= PCM_BUFFERSIZE;
            pcmBufferIndex += PCM_BUFFERSIZE;
            if (dataToRead == 0)
            {
                res = f_read(&file, decodeBuffer, DECODE_BUFFERSIZE, &pcmBr);
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
                dataToRead = pcmBr;
                pcmBufferIndex = 0;
            }
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
    inBuffer = pcmBuffer;
    fillingBuffer = 0;
    fillBufFlag = 1;
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    inBuffer = pcmBuffer +  PCM_BUFFERSIZE / 2;
    fillingBuffer = 1;
    fillBufFlag = 1;
}
