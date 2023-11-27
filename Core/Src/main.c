/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "files.h"
#include "math.h"
#include "pcm.h"
#include <stdio.h>
#include <string.h>
#include "lcdtp.h"
#include "xpt2046.h"
#include "pcm.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define usScreenWidth 240
#define usScreenHeight 320
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc2;

I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_tx;

SD_HandleTypeDef hsd;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */
 float value = 0.2;
 uint32_t var;
 uint32_t sine_val[100];

 #define PI 3.1415926

 void getSineVal() {
 	for (int i = 0; i < 100; i++) {
 		sine_val[i] = (((sin(i*2*PI/100)+1)*(255/2))*0.5);

 	}
 }
 uint8_t updateLCDStartPlay = 1;
 uint8_t playPCMFlag = 0;
 uint8_t drawGUIFlag = 1;
 uint8_t drawButtonsFlag = 1;
 void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == GPIO_PIN_0) {
    kay = 1;
  }
	if (GPIO_Pin == GPIO_PIN_13) {
		playPCMFlag = playPCMFlag == 0 ? 1 : 0;
		if (!playPCMFlag) {
			drawGUIFlag = 1;
		}
//		updateLCDStartPlay = 1;
 	}
}

// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
//     if (htim->Instance == TIM1) {
//         if (pcmDataToRead > 0) {
//             HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, pcmBuffer[pcmBufferIndex]);
//             HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_8B_R, pcmBuffer[pcmBufferIndex+1]);
//             pcmBufferIndex += 2;
//             pcmDataToRead--;
//         }
//     }
// }

const uint16_t triangle_wave[]  = {
0x400,0x800,0xc00,0x1000,0x1400,0x1800,0x1c00,0x2000,
0x2400,0x2800,0x2c00,0x3000,0x3400,0x3800,0x3c00,0x4000,
0x4400,0x4800,0x4c00,0x5000,0x5400,0x5800,0x5c00,0x6000,
0x6400,0x6800,0x6c00,0x7000,0x7400,0x7800,0x7c00,0x8000,
0x83ff,0x87ff,0x8bff,0x8fff,0x93ff,0x97ff,0x9bff,0x9fff,
0xa3ff,0xa7ff,0xabff,0xafff,0xb3ff,0xb7ff,0xbbff,0xbfff,
0xc3ff,0xc7ff,0xcbff,0xcfff,0xd3ff,0xd7ff,0xdbff,0xdfff,
0xe3ff,0xe7ff,0xebff,0xefff,0xf3ff,0xf7ff,0xfbff,0xffff,
0xfbff,0xf7ff,0xf3ff,0xefff,0xebff,0xe7ff,0xe3ff,0xdfff,
0xdbff,0xd7ff,0xd3ff,0xcfff,0xcbff,0xc7ff,0xc3ff,0xbfff,
0xbbff,0xb7ff,0xb3ff,0xafff,0xabff,0xa7ff,0xa3ff,0x9fff,
0x9bff,0x97ff,0x93ff,0x8fff,0x8bff,0x87ff,0x83ff,0x8000,
0x7c00,0x7800,0x7400,0x7000,0x6c00,0x6800,0x6400,0x6000,
0x5c00,0x5800,0x5400,0x5000,0x4c00,0x4800,0x4400,0x4000,
0x3c00,0x3800,0x3400,0x3000,0x2c00,0x2800,0x2400,0x2000,
0x1c00,0x1800,0x1400,0x1000,0xc00,0x800,0x400,0x0,
};
const uint16_t sin_tbl_int[91] = {	    0,   572,  1144,  1715,  2286,  2856,  3425,  3993,  4560,  5126,
									 5690,  6252,  6813,  7371,  7927,  8481,  9032,  9580, 10126, 10668,
									11207, 11743, 12275, 12803, 13328, 13848, 14365, 14876, 15384, 15886,
									16384, 16877, 17364, 17847, 18324, 18795, 19261, 19720, 20174, 20622,
									21063, 21498, 21926, 22348, 22763, 23170, 23571, 23965, 24351, 24730,
									25102, 25466, 25822, 26170, 26510, 26842, 27166, 27482, 27789, 28088,
									28378, 28660, 28932, 29197, 29452, 29698, 29935, 30163, 30382, 30592,
									30792, 30983, 31164, 31336, 31499, 31651, 31795, 31928, 32052, 32166,
									32270, 32365, 32449, 32524, 32588, 32643, 32688, 32723, 32748, 32763,
									32768 };
/* Strings */
char * pStr = 0, cStr [10];

/* Flags */
int actionFlag = 0;
int inputSrc = 0 /* 0 = SD | 1 = 3.5mm */;		// See xpt2046.c \ audiofxConfig()
int kay = 0 /* K1 flag */, trueTone = 0 /* LDR trigger */, colourP = 0;	// Display colour preset
int shock = 0 /* Dynamic vibration trigger */, buzz = 1;	// 1 = Mildest | 3 = Strongest
int ruler;		// ADC2
char chovy[3];	// ADC2 Read buffer
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FSMC_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_DMA_Init(void);
static void MX_I2S2_Init(void);
static void MX_ADC2_Init(void);
/* USER CODE BEGIN PFP */
uint8_t virtualAudioPlayerTask() {
	if (playPCMFlag) {
		// LCD_Clear (0, 0, 240, 320, BACKGROUND);
		scanFiles("0:/");
		return 1;
	} else {
		return 0;
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void threeFPSRGB(void);

void audiofxConfigPage(void)
{

  	/* Title */
  	pStr = "Audio FX";
  	LCD_DrawString_Color (64, 24, pStr, BACKGROUND, BLACK );
	drawGUIFlag = 1;
	while (1)
	{
		if (!virtualAudioPlayerTask()) {
			if (drawGUIFlag) {
				drawGUIFlag = 0;
				LCD_Clear (0, 0, 240, 320, BACKGROUND);

				/* Button rendering */
				LCD_DrawBox(8, 8, 48, 48, BLACK);
				pStr = "BACK";
				LCD_DrawString_Color (16, 24, pStr, BACKGROUND, BLACK);

				/* Input source selection */
				LCD_DrawBox(184, 64, 48, 48, GREEN);
				pStr = "Input source";
				LCD_DrawString_Color (16, 80, pStr, BACKGROUND, BLACK);

				if (inputSrc == 0) {
					LCD_DrawBox(184, 64, 48, 48, GREEN);
					pStr = "SD";
					LCD_DrawString_Color (200, 80, pStr, BACKGROUND, GREEN);
				} else {
					LCD_DrawBox(184, 64, 48, 48, ORANGE);
					pStr = "3.5mm";
					LCD_DrawString_Color (192, 80, pStr, BACKGROUND, ORANGE);
				}
				ruler = HAL_ADC_GetValue(&hadc2);
				adaptiveBrightness(trueTone, ruler);
			}
			if (drawButtonsFlag) {
				drawButtonsFlag = 0;
				/* Voice effect selection: This is put inside the while-loop for updating */
				pStr = "Voice effect";
				LCD_DrawString_Color (16, 120, pStr, BACKGROUND, BLACK);

				pStr = "Selected";
				LCD_DrawString_Color (120, 120, pStr, BACKGROUND, GREEN);

				if (fxMode == 11) {
					LCD_DrawBox(32, 142, 48, 48, GREEN);
					pStr = "No";
					LCD_DrawString_Color (34, 150, pStr, BACKGROUND, GREEN);
					pStr = "effect";
					LCD_DrawString_Color (34, 166, pStr, BACKGROUND, GREEN);
				}
				else {
					LCD_DrawBox(32, 142, 48, 48, BLACK);
					pStr = "No";
					LCD_DrawString_Color (34, 150, pStr, BACKGROUND, BLACK);
					pStr = "effect";
					LCD_DrawString_Color (34, 166, pStr, BACKGROUND, BLACK);
				}

				if (fxMode == 12) {
					LCD_DrawBox(92, 142, 48, 48, GREEN);
					pStr = "Deep";
					LCD_DrawString_Color (94, 150, pStr, BACKGROUND, GREEN);
					pStr = "pitch";
					LCD_DrawString_Color (94, 166, pStr, BACKGROUND, GREEN);
				}
				else {
					LCD_DrawBox(92, 142, 48, 48, BLACK);
					pStr = "Deep";
					LCD_DrawString_Color (94, 150, pStr, BACKGROUND, BLACK);
					pStr = "pitch";
					LCD_DrawString_Color (94, 166, pStr, BACKGROUND, BLACK);
				}

				if (fxMode == 13) {
					LCD_DrawBox(152, 142, 48, 48, GREEN);
					pStr = "High";
					LCD_DrawString_Color (154, 150, pStr, BACKGROUND, GREEN);
					pStr = "pitch";
					LCD_DrawString_Color (154, 166, pStr, BACKGROUND, GREEN);
				}
				else {
					LCD_DrawBox(152, 142, 48, 48, BLACK);
					pStr = "High";
					LCD_DrawString_Color (154, 150, pStr, BACKGROUND, BLACK);
					pStr = "pitch";
					LCD_DrawString_Color (154, 166, pStr, BACKGROUND, BLACK);
				}

				if (fxMode == 21) {
					LCD_DrawBox(32, 198, 48, 48, GREEN);
					pStr = "Robot";
					LCD_DrawString_Color (34, 214, pStr, BACKGROUND, GREEN);
				}
				else {
					LCD_DrawBox(32, 198, 48, 48, BLACK);
					pStr = "Robot";
					LCD_DrawString_Color (34, 214, pStr, BACKGROUND, BLACK);
				}

				if (fxMode == 22) {
					LCD_DrawBox(92, 198, 48, 48, GREEN);
					pStr = "Dino";
					LCD_DrawString_Color (94, 214, pStr, BACKGROUND, GREEN);
				}
				else {
					LCD_DrawBox(92, 198, 48, 48, BLACK);
					pStr = "Dino";
					LCD_DrawString_Color (94, 214, pStr, BACKGROUND, BLACK);
				}

				if (fxMode == 23) {
					LCD_DrawBox(152, 198, 48, 48, GREEN);
					pStr = "R2-D2";
					LCD_DrawString_Color (154, 214, pStr, BACKGROUND, GREEN);
				}
				else {
					LCD_DrawBox(152, 198, 48, 48, BLACK);
					pStr = "R2-D2";
					LCD_DrawString_Color (154, 214, pStr, BACKGROUND, BLACK);
				}
			}

		if ( ucXPT2046_TouchFlag == 1 ) {
			drawButtonsFlag = 1;
			actionFlag = audiofxConfig();
			ucXPT2046_TouchFlag = 0;
			switch (actionFlag) {
				case 0:
					LCD_Clear (0, 0, 240, 320, BACKGROUND);
					return;
				case 1:
					if (inputSrc == 0) {
						LCD_Clear (184, 64, 48, 48, BACKGROUND);
						LCD_DrawBox(184, 64, 48, 48, ORANGE);
						pStr = "3.5mm";
						LCD_DrawString_Color (192, 80, pStr, BACKGROUND, ORANGE);
						inputSrc++;
					} else {
						LCD_Clear (184, 64, 48, 48, BACKGROUND);
						LCD_DrawBox(184, 64, 48, 48, GREEN);
						pStr = "SD";
						LCD_DrawString_Color (200, 80, pStr, BACKGROUND, GREEN );
						inputSrc--;
					}
					break;
				// Toggle FX
				case 11: fxMode = 11; break;
				case 12: fxMode = 12; break;
				case 13: fxMode = 13; break;
				case 21: fxMode = 21; break;
				case 22: fxMode = 22; break;
				case 23: fxMode = 23; break;
			}
		}

		}
		// HAL_Delay(50);
	}	
}

void displayConfigPage(void)
{
	drawGUIFlag = 1;
	LCD_Clear (0, 0, 240, 320, BACKGROUND);

	/* Button rendering */
	LCD_DrawBox(8, 8, 48, 48, BLACK);
	pStr = "BACK";
	LCD_DrawString_Color (16, 24, pStr, BACKGROUND, BLACK );

	/* LDR brightness control */
	pStr = "Auto";
	LCD_DrawString_Color (16, 72, pStr, BACKGROUND, BLACK );

	pStr = "Brightness";
	LCD_DrawString_Color (16, 88, pStr, BACKGROUND, BLACK );

//  Showing LDR value on screen
	sprintf(chovy, "%x",  ruler);
	LCD_Clear(128, 88, 48, 16, BACKGROUND);
	LCD_DrawString(128, 88, chovy);

	if (trueTone == 1) {
		LCD_DrawBox(184, 64, 48, 48, GREEN);
		pStr = "ON";
		LCD_DrawString_Color (200, 80, pStr, BACKGROUND, GREEN);
	} else {
		LCD_DrawBox(184, 64, 48, 48, ORANGE);
		pStr = "OFF";
		LCD_DrawString_Color (200, 80, pStr, BACKGROUND, ORANGE);
	}

	/* Breathing light */
	LCD_DrawBox(184, 120, 48, 48, BLACK);
	pStr = "Colour";
	LCD_DrawString_Color (16, 128, pStr, BACKGROUND, BLACK);

	pStr = "preset";
	LCD_DrawString_Color (16, 144, pStr, BACKGROUND, BLACK);

	sprintf(cStr, "%d", colourP);
	switch (colourP) {
		case 0:
			LCD_DrawBox(184, 120, 48, 48, RED);
			LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, RED);
			break;
		case 1:
			LCD_DrawBox(184, 120, 48, 48, ORANGE);
			LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, ORANGE);
			break;
		case 2:
			LCD_DrawBox(184, 120, 48, 48, YELLOW);
			LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, YELLOW);
			break;
		case 3:
			LCD_DrawBox(184, 120, 48, 48, GREEN);
			LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, GREEN);
			break;
		case 4:
			LCD_DrawBox(184, 120, 48, 48, CYAN);
			LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, CYAN);
			break;
		case 5:
			LCD_DrawBox(184, 120, 48, 48, BLUE);
			LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, BLUE);
			break;
		case 6:
			LCD_DrawBox(184, 120, 48, 48, MAGENTA);
			LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, MAGENTA);
			break;
	}
	LCD_DrawBox(184, 176, 48, 48, BLACK);
	pStr = "Start";
	LCD_DrawString_Color (16, 184, pStr, BACKGROUND, BLACK);

	pStr = "RGB";
	LCD_DrawString_Color (16, 200, pStr, BACKGROUND, BLACK);

	/* Title */
	pStr = "Display options";
	LCD_DrawString_Color ( 64, 24, pStr, BACKGROUND, BLACK);
	while (1)
	{
		ruler = HAL_ADC_GetValue(&hadc2);
		adaptiveBrightness(trueTone, ruler);

		if ( ucXPT2046_TouchFlag == 1 ) {
			actionFlag = backButton();
			ucXPT2046_TouchFlag = 0;
			switch (actionFlag) {
				case 0:
					LCD_Clear (0, 0, 240, 320, BACKGROUND);
					return;
				case 1:
					if (trueTone == 0) {
						LCD_Clear (184, 64, 48, 48, BACKGROUND);
						LCD_DrawBox(184, 64, 48, 48, GREEN);
						pStr = "ON";
						LCD_DrawString_Color (200, 80, pStr, BACKGROUND, GREEN);
						trueTone++;
					} else {
						LCD_Clear (184, 64, 48, 48, BACKGROUND);
						LCD_DrawBox(184, 64, 48, 48, ORANGE);
						pStr = "OFF";
						LCD_DrawString_Color (200, 80, pStr, BACKGROUND, ORANGE);
						inputSrc--;
					}
					break;
				case 2:
					colourP = (colourP == 6) ? 0 : colourP + 1;
					sprintf(cStr, "%d", colourP);
					switch (colourP) {
						case 0:
							LCD_DrawBox(184, 120, 48, 48, RED);
							LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, RED);
							break;
						case 1:
							LCD_DrawBox(184, 120, 48, 48, ORANGE);
							LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, ORANGE);
							break;
						case 2:
							LCD_DrawBox(184, 120, 48, 48, YELLOW);
							LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, YELLOW);
							break;
						case 3:
							LCD_DrawBox(184, 120, 48, 48, GREEN);
							LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, GREEN);
							break;
						case 4:
							LCD_DrawBox(184, 120, 48, 48, CYAN);
							LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, CYAN);
							break;
						case 5:
							LCD_DrawBox(184, 120, 48, 48, BLUE);
							LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, BLUE);
							break;
						case 6:
							LCD_DrawBox(184, 120, 48, 48, MAGENTA);
							LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, MAGENTA);
							break;
					}
					break;
				case 3: threeFPSRGB(); return;
			}
		}
		// HAL_Delay(50);
	}	
}

void dvConfigPage(void)
{
	drawGUIFlag = 1;
	while (1)
	{
		if (!virtualAudioPlayerTask()) {
			if (drawGUIFlag) {
				drawGUIFlag = 0;
				LCD_Clear (0, 0, 240, 320, BACKGROUND);

				/* Button rendering */
				LCD_DrawBox(8, 8, 48, 48, BLACK);
				pStr = "BACK";
				LCD_DrawString_Color (16, 24, pStr, BACKGROUND, BLACK );

				/* Toggle */
				pStr = "Enable";
				LCD_DrawString_Color (16, 72, pStr, BACKGROUND, BLACK );

				pStr = "effect";
				LCD_DrawString_Color (16, 88, pStr, BACKGROUND, BLACK );

				if (shock == 1) {
					LCD_DrawBox(184, 64, 48, 48, GREEN);
					pStr = "ON";
					LCD_DrawString_Color (200, 80, pStr, BACKGROUND, GREEN);
				} else {
					LCD_DrawBox(184, 64, 48, 48, ORANGE);
					pStr = "OFF";
					LCD_DrawString_Color (200, 80, pStr, BACKGROUND, ORANGE);

				/* Peak strength */
				pStr = "Max vibration";
				LCD_DrawString_Color (16, 120, pStr, BACKGROUND, BLACK );

				pStr = "strength";
				LCD_DrawString_Color (16, 136, pStr, BACKGROUND, BLACK );

				pStr = "1=Mild | 3=Strong";
				LCD_DrawString_Color (16, 136, pStr, BACKGROUND, BLACK );

				sprintf(cStr, "%d", buzz);
				}
				switch (buzz) {
					case 1:
						LCD_DrawBox(184, 120, 48, 48, CYAN);
						LCD_DrawString_Color (204, 136, cStr, BACKGROUND, CYAN);
						break;
					case 2:
						LCD_DrawBox(184, 120, 48, 48, GREEN);
						LCD_DrawString_Color (204, 136, cStr, BACKGROUND, GREEN);
						break;
					case 3:
						LCD_DrawBox(184, 120, 48, 48, ORANGE);
						LCD_DrawString_Color (204, 136, cStr, BACKGROUND, ORANGE);
						break;
				}

				/* Title */
				pStr = "Dynamic Vibration";
				LCD_DrawString_Color (64, 24, pStr, BACKGROUND, BLACK );

			}

			ruler = HAL_ADC_GetValue(&hadc2);
			adaptiveBrightness(trueTone, ruler);
			if ( ucXPT2046_TouchFlag == 1 ) {
				actionFlag = backButton();
				ucXPT2046_TouchFlag = 0;
				switch (actionFlag) {
					case 0:
						LCD_Clear (0, 0, 240, 320, BACKGROUND);
						return;
					case 1:		// Toggle
						if (shock == 0) {
							LCD_Clear (184, 64, 48, 48, BACKGROUND);
							LCD_DrawBox(184, 64, 48, 48, GREEN);
							pStr = "ON";
							LCD_DrawString_Color (200, 80, pStr, BACKGROUND, GREEN);
							shock++;
						} else {
							LCD_Clear (184, 64, 48, 48, BACKGROUND);
							LCD_DrawBox(184, 64, 48, 48, ORANGE);
							pStr = "OFF";
							LCD_DrawString_Color (200, 80, pStr, BACKGROUND, ORANGE);
							shock--;
						}
						break;
					case 2:		// Cascade
						buzz = (buzz == 3) ? 1 : buzz + 1;
						sprintf(cStr, "%d", buzz);
						switch (buzz) {
							case 1:
								LCD_DrawBox(184, 120, 48, 48, CYAN);
								LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, CYAN);
								break;
							case 2:
								LCD_DrawBox(184, 120, 48, 48, GREEN);
								LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, GREEN);
								break;
							case 3:
								LCD_DrawBox(184, 120, 48, 48, ORANGE);
								LCD_DrawString_Color ( 204, 136, cStr, BACKGROUND, ORANGE);
								break;
						}
				}
			}

		}
	}	
}

void hiddenConfigPage(void)
{
	drawGUIFlag = 1;
	LCD_Clear (0, 0, 240, 320, BACKGROUND);

	/* Button rendering */
	LCD_DrawBox(8, 8, 48, 48, BLACK);
	pStr = "BACK";
	LCD_DrawString_Color ( 16, 24, pStr, BACKGROUND, BLACK );

	/* Breathing light */
	LCD_DrawBox(184, 64, 48, 48, BLACK);
	pStr = "MLG";
	LCD_DrawString_Color ( 16, 72, pStr, BACKGROUND, BLACK );

	pStr = "RGB";
	LCD_DrawString_Color ( 16, 88, pStr, BACKGROUND, BLACK );

	/* Title */
	pStr = "Advanced options";
	LCD_DrawString_Color ( 64, 24, pStr, BACKGROUND, BLACK );
	HAL_Delay(2000);
	while (1)
	{
		ruler = HAL_ADC_GetValue(&hadc2);
		adaptiveBrightness(trueTone, ruler);

//		colourP = 0;
//		threeFPSRGB();
	    if ( ucXPT2046_TouchFlag == 1 ) {
		    actionFlag = backButton();
		    ucXPT2046_TouchFlag = 0;
		    switch (actionFlag) {
			    case 0:
			        LCD_Clear (0, 0, 240, 320, BACKGROUND);
				    return;
			    case 1:
				    colourP = 8;
				    threeFPSRGB();
				    return;
		    }
	    }
		HAL_Delay(50);
	}	
}

void switchFPSRGB() {
	
		ruler = HAL_ADC_GetValue(&hadc2);
		adaptiveBrightness(trueTone, ruler);
		switch (colourP) {
			case 0:
				LCD_Clear (0, 0, 240, 320, RED_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, RED_C);

				LCD_Clear (0, 0, 240, 320, RED_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, RED_B);

				LCD_Clear (0, 0, 240, 320, RED_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, RED_A);

				LCD_Clear (0, 0, 240, 320, RED);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, RED);

				LCD_Clear (0, 0, 240, 320, RED_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, RED_A);

				LCD_Clear (0, 0, 240, 320, RED_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, RED_B);

				LCD_Clear (0, 0, 240, 320, RED_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, RED_C);

				break;
			case 1:
				LCD_Clear (0, 0, 240, 320, ORANGE_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, ORANGE_C);

				LCD_Clear (0, 0, 240, 320, ORANGE_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, ORANGE_B);

				LCD_Clear (0, 0, 240, 320, ORANGE_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, ORANGE_A);

				LCD_Clear (0, 0, 240, 320, ORANGE);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, ORANGE);

				LCD_Clear (0, 0, 240, 320, ORANGE_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, ORANGE_A);

				LCD_Clear (0, 0, 240, 320, ORANGE_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, ORANGE_B);

				LCD_Clear (0, 0, 240, 320, ORANGE_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, ORANGE_C);

				break;
			case 2:
				LCD_Clear (0, 0, 240, 320, YELLOW_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, YELLOW_C);

				LCD_Clear (0, 0, 240, 320, YELLOW_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, YELLOW_B);

				LCD_Clear (0, 0, 240, 320, YELLOW_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, YELLOW_A);

				LCD_Clear (0, 0, 240, 320, YELLOW);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, YELLOW);

				LCD_Clear (0, 0, 240, 320, YELLOW_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, YELLOW_A);

				LCD_Clear (0, 0, 240, 320, YELLOW_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, YELLOW_B);

				LCD_Clear (0, 0, 240, 320, YELLOW_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, YELLOW_C);

				break;
			case 3:
				LCD_Clear (0, 0, 240, 320, GREEN_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, GREEN_C);

				LCD_Clear (0, 0, 240, 320, GREEN_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, GREEN_B);

				LCD_Clear (0, 0, 240, 320, GREEN_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, GREEN_A);

				LCD_Clear (0, 0, 240, 320, GREEN);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, GREEN);

				LCD_Clear (0, 0, 240, 320, GREEN_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, GREEN_A);

				LCD_Clear (0, 0, 240, 320, GREEN_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, GREEN_B);

				LCD_Clear (0, 0, 240, 320, GREEN_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, GREEN_C);

				break;
			case 4:
				LCD_Clear (0, 0, 240, 320, CYAN_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, CYAN_C);

				LCD_Clear (0, 0, 240, 320, CYAN_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, CYAN_B);

				LCD_Clear (0, 0, 240, 320, CYAN_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, CYAN_A);

				LCD_Clear (0, 0, 240, 320, CYAN);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, CYAN);

				LCD_Clear (0, 0, 240, 320, CYAN_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, CYAN_A);
				LCD_Clear (0, 0, 240, 320, CYAN_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, CYAN_B);

				LCD_Clear (0, 0, 240, 320, CYAN_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, CYAN_C);

				break;
			case 5:
				LCD_Clear (0, 0, 240, 320, BLUE_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLUE_C);

				LCD_Clear (0, 0, 240, 320, BLUE_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLUE_B);

				LCD_Clear (0, 0, 240, 320, BLUE_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLUE_A);

				LCD_Clear (0, 0, 240, 320, BLUE);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLUE);

				LCD_Clear (0, 0, 240, 320, BLUE_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLUE_A);

				LCD_Clear (0, 0, 240, 320, BLUE_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLUE_B);

				LCD_Clear (0, 0, 240, 320, BLUE_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLUE_C);

				break;
			case 6:
				LCD_Clear (0, 0, 240, 320, MAGENTA_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, MAGENTA_C);

				LCD_Clear (0, 0, 240, 320, MAGENTA_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, MAGENTA_B);

				LCD_Clear (0, 0, 240, 320, MAGENTA_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, MAGENTA_A);

				LCD_Clear (0, 0, 240, 320, MAGENTA);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, MAGENTA);

				LCD_Clear (0, 0, 240, 320, MAGENTA_A);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, MAGENTA_A);

				LCD_Clear (0, 0, 240, 320, MAGENTA_B);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, MAGENTA_B);

				LCD_Clear (0, 0, 240, 320, MAGENTA_C);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, MAGENTA_C);

				break;
			case 7:
				LCD_Clear (0, 0, 240, 320, RED);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, RED);

				LCD_Clear (0, 0, 240, 320, ORANGE);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, ORANGE);

				LCD_Clear (0, 0, 240, 320, YELLOW);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, YELLOW);

				LCD_Clear (0, 0, 240, 320, GREEN);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, GREEN);

				LCD_Clear (0, 0, 240, 320, CYAN);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, CYAN);

				LCD_Clear (0, 0, 240, 320, BLUE);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLUE);

				LCD_Clear (0, 0, 240, 320, MAGENTA);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, MAGENTA);

				break;
			case 8:
				LCD_Clear (0, 0, 240, 320, BACKGROUND);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLACK);

				LCD_Clear (0, 0, 240, 320, GREY);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, GREY, BLACK);

				LCD_Clear (0, 0, 240, 320, BLACK);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BLACK, BACKGROUND);

				LCD_Clear (0, 0, 240, 320, GREY);
				pStr = "Press K1 to exit";
				LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, GREY, BLACK);

				break;
		}
}

void threeFPSRGB(void)
{
	drawGUIFlag = 1;
	while (1) {
		switchFPSRGB();
		if (kay == 1) {
			kay = 0;
			LCD_Clear(0, 0, 240, 320, BACKGROUND);
			if (colourP == 7 || colourP == 8) colourP = 6;
			return;
		}
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_DMA_Init();
  MX_I2S2_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
	FATFS myFATFS;
	FIL myFILE;
	UINT numberofbytes;
	char myPath[] = "TEST.TXT\0";
	char myData[] = "Hello World\0";
	FRESULT res;

	res = f_mount(&myFATFS,SDPath,1);
	if (res == FR_OK)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_RESET);
		f_open(&myFILE, myPath, FA_WRITE |FA_CREATE_ALWAYS);
		f_write(&myFILE, myData, sizeof(myData), &numberofbytes);
		f_close(&myFILE);
		HAL_Delay(1000);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_RESET);
	}

  /* USER CODE BEGIN 2 */
	macXPT2046_CS_DISABLE();
	LCD_INIT();
	/* Boot splash */
	pStr = "Welcome to Stream Deck!";
	LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLACK );
	HAL_Delay(1000);
	LCD_Clear(0, 0, 240, 320, BACKGROUND);
	HAL_ADCEx_Calibration_Start(&hadc2);
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, 500);
	while( ! XPT2046_Touch_Calibrate () );
	LCD_Clear(0, 0, 240, 320, BACKGROUND);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//	if (updateLCDStartPlay) {
//		LCD_DrawString(100, 100, "Press to start playing");
//		updateLCDStartPlay = 0;
//	}
	if (!virtualAudioPlayerTask()) {

	     ruler = HAL_ADC_GetValue(&hadc2);
	   	adaptiveBrightness(trueTone, ruler);
	   	if (drawGUIFlag) {
	 	 	/* Main menu */
	   		drawGUIFlag = 0;
	   		LCD_Clear(0, 0, 240, 320, BACKGROUND);
	 	 	pStr = "Stream Deck";			// Title
	 	 	LCD_DrawString_Color ( 8, 8, pStr, BACKGROUND, BLACK );

	 	 	/* Options rendering */
	 	 	LCD_DrawBox(176, 32, 48, 48, BLACK);
	 	 	pStr = "Audio FX";
	 	 	LCD_DrawString_Color (104, 48, pStr, BACKGROUND, BLACK );
	 	 	// pStr = "Audio";     // Replace me with the icon!
	 	 	// LCD_DrawString_Color (180, 48, pStr, BACKGROUND, BLACK );
	 	 	LCD_DrawBox(184, 48, 8, 16, BLACK);
	 	 	LCD_DrawEllipse (194, 56, 12, 6, BLACK);
	 	 	LCD_DrawEllipse (202, 56, 16, 6, BLACK);
	 	 	LCD_DrawEllipse (210, 56, 20, 6, BLACK);

	 	 	LCD_DrawBox(176, 96, 48, 48, BLACK);
	 	 	pStr = "Display";
	 	 	LCD_DrawString_Color (112, 112, pStr, BACKGROUND, BLACK );
	 	 	// pStr = "Disp";     // Replace me with the icon!
	 	 	// LCD_DrawString_Color (180, 112, pStr, BACKGROUND, BLACK );
	 	 	/* Icon */
	 	 	// Vertical deco
	 	 	LCD_DrawLine (200, 100, 200, 104, BLACK );
	 	 	LCD_DrawLine (200, 136, 200, 140, BLACK );
	 	 	// Horizontal deco
	 	 	LCD_DrawLine (216, 120, 220, 120, BLACK );
	 	 	LCD_DrawLine (180, 120, 184, 120, BLACK );
	 	 	// Slanted deco
	 	 	LCD_DrawLine (182, 102, 186, 106, BLACK );
	 	 	LCD_DrawLine (182, 138, 186, 134, BLACK );
	 	 	LCD_DrawLine (218, 102, 214, 106, BLACK );
	 	 	LCD_DrawLine (218, 138, 214, 134, BLACK );
	 	 	LCD_DrawEllipse (200, 120, 12, 12, BLACK);

	 	 	LCD_DrawBox(176, 160, 48, 48, BLACK);
	 	 	pStr = "Dynamic Vibration";
	 	 	LCD_DrawString_Color (32, 176, pStr, BACKGROUND, BLACK );
	 	 	/* Icon */
	 	 	LCD_DrawBox(182, 166, 36, 36, BLACK);
	 	 	LCD_DrawEllipse (200, 184, 12, 12, BLACK);

	 	 	LCD_DrawBox(176, 224, 48, 48, BLACK);
	 	 	pStr = "Options";
	 	 	/* Icon */
	 	 	LCD_DrawString_Color (112, 240, pStr, BACKGROUND, BLACK );
	 	 	LCD_DrawEllipse (200, 248, 12, 12, BLACK);

	   	}


 	 	// Main dish :)
		 //  hiddenConfigPage();
 	 	if ( ucXPT2046_TouchFlag == 1 ) {
 	 	    actionFlag = menuButton();
 	 	    ucXPT2046_TouchFlag = 0;
 	 	    switch (actionFlag) {
 	 	    	case 0: audiofxConfigPage(); drawGUIFlag = 1; break;
 	 	    	case 1: displayConfigPage(); drawGUIFlag = 1; break;
 	 	    	case 2: dvConfigPage(); drawGUIFlag = 1; break;
 	 	    	case 3: hiddenConfigPage(); drawGUIFlag = 1; break;
 	 	    }
 	 	}
	}

  // }
     }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_I2S2;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  PeriphClkInit.I2s2ClockSelection = RCC_I2S2CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief I2S2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S2_Init(void)
{

  /* USER CODE BEGIN I2S2_Init 0 */

  /* USER CODE END I2S2_Init 0 */

  /* USER CODE BEGIN I2S2_Init 1 */

  /* USER CODE END I2S2_Init 1 */
  hi2s2.Instance = SPI2;
  hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B_EXTENDED;
  hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_16K;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
  if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S2_Init 2 */

  /* USER CODE END I2S2_Init 2 */

}

/**
  * @brief SDIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 4;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 100-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 72-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_DISABLE;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchro(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE2 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Disconnect NADV
  */

  __HAL_AFIO_FSMCNADV_DISCONNECTED();

  /* USER CODE BEGIN FSMC_Init 2 */

  /* USER CODE END FSMC_Init 2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
