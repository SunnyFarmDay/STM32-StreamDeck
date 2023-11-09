#include "pcm.h"


static volatile uint32_t timerCode = 0;

unsigned int pcmBr = 0;        
unsigned int pcmDataToRead = 0;
uint16_t pcmBufferIndex = 0;
uint8_t pcmBuffer[DECODE_BUFFERSIZE];
uint8_t playingFlag = 0;

void timerChange() {
    __HAL_TIM_SET_PRESCALER(&htim1, 72000 / AudioBitRate - 1);
    __HAL_TIM_SET_COUNTER(&htim1, 0);
}

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
// 	timerCode++;
//     if (htim->Instance == TIM1) {
//         if (pcmDataToRead > 0) {
//             HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, pcmBuffer[pcmBufferIndex]);
//             HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, pcmBuffer[pcmBufferIndex+1]);
//             pcmBufferIndex += 2;
//             pcmDataToRead--;
//         }
//     }
// }

void playPCMFile(char *filename) {
    FIL file;
    uint8_t res;

    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) {
        return;
    }

    HAL_TIM_Base_Start_IT(&htim1);
    while(1) {
        res = f_read(&file, pcmBuffer, DECODE_BUFFERSIZE, &pcmBr);

        if (res != FR_OK) {
            return;
        }
        if (pcmBr == 0) {
            break;
        }
        pcmDataToRead = pcmBr;
        HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
        HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
        while (pcmDataToRead > 0) {
            // HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, pcmBuffer[i]);
            // HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, pcmBuffer[i+1]);
            // i += 2;
            // dataToRead--;
        }
    }
}
