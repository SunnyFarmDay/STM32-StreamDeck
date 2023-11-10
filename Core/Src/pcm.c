#include "pcm.h"


static volatile uint32_t timerCode = 0;

unsigned int pcmBr = 0;        
unsigned int pcmDataToRead = 0;
uint16_t pcmBufferIndex = 0;
uint8_t pcmBuffer[DECODE_BUFFERSIZE];
uint8_t playingFlag = 0;

void timerChange() {
    __HAL_TIM_SET_PRESCALER(&htim1, 72 / AudioBitRate - 1);
    __HAL_TIM_SET_COUNTER(&htim1, 1000*5-1);
}

void nextFreq(TIM_HandleTypeDef *htim) {
	timerCode++;
    if (htim->Instance == TIM1) {
        if (pcmDataToRead > 0) {
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, pcmBuffer[pcmBufferIndex]);
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, pcmBuffer[pcmBufferIndex+1]);
            pcmBufferIndex += 2;
            pcmDataToRead--;
        }
    }
}

void playPCMFile(char *filename) {
    timerChange();
    FIL file;
    uint8_t res;

    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) {
        return;
    }
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    HAL_DAC_Start(&hdac, DAC_CHANNEL_2);

    DWORD readingStart = f_tell(&file);
    // HAL_TIM_Base_Start_IT(&htim1);
    while(1) {
        res = f_read(&file, pcmBuffer, DECODE_BUFFERSIZE, &pcmBr);

        if (res != FR_OK) {
            return;
        }
        if (pcmBr == 0) {
            break;
        }
        pcmDataToRead = pcmBr;
        pcmBufferIndex = 0;
        while (pcmDataToRead > 0) {
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, pcmBuffer[pcmBufferIndex]);
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, pcmBuffer[pcmBufferIndex+1]);
            pcmBufferIndex += 2;
            pcmDataToRead -= 2;
            HAL_Delay(1);
        }
        HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
        HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);


    }
    f_close(&file);
    // HAL_TIM_Base_Stop_IT(&htim1);
}
