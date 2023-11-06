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
#include <string.h>
#include "lcdtp.h"
#include "xpt2046.h"
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
 SD_HandleTypeDef hsd;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */
char * pStr = 0;
int actionFlag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FSMC_Init(void);
static void MX_SDIO_SD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void audiofxConfigPage(void)
{
  LCD_Clear (0, 0, 240, 320, BACKGROUND);
  /* Button rendering */
  LCD_DrawBox(8, 8, 48, 48);
  pStr = "BACK";
  LCD_DrawString_Color ( 16, 24, pStr, BACKGROUND, BLACK );
  /* Title */
  pStr = "Audio FX";
  LCD_DrawString_Color ( 64, 28, pStr, BACKGROUND, BLACK );	
  while (1)
  {
    if ( ucXPT2046_TouchFlag == 1 ) {
			actionFlag = backButton();
      ucXPT2046_TouchFlag = 0;
      if (actionFlag == 0) {
        LCD_Clear (0, 0, 240, 320, BACKGROUND);
        return;
      }
    }					
	  HAL_Delay(50);
  }	
}

void displayConfigPage(void)
{
  LCD_Clear (0, 0, 240, 320, BACKGROUND);
  /* Button rendering */
  LCD_DrawBox(8, 8, 48, 48);
  pStr = "BACK";
  LCD_DrawString_Color ( 16, 24, pStr, BACKGROUND, BLACK );
  /* Title */
  pStr = "Display options";
  LCD_DrawString_Color ( 64, 28, pStr, BACKGROUND, BLACK );	
  while (1)
  {
    if ( ucXPT2046_TouchFlag == 1 ) {
			actionFlag = backButton();
      ucXPT2046_TouchFlag = 0;
      if (actionFlag == 0) {
        LCD_Clear (0, 0, 240, 320, BACKGROUND);
        return;
      }
    }					
	  HAL_Delay(50);
  }	
}

void dvConfigPage(void)
{
  LCD_Clear (0, 0, 240, 320, BACKGROUND);
  /* Button rendering */
  LCD_DrawBox(8, 8, 48, 48);
  pStr = "BACK";
  LCD_DrawString_Color ( 16, 24, pStr, BACKGROUND, BLACK );
  /* Title */
  pStr = "Dynamic Vibration";
  LCD_DrawString_Color ( 64, 28, pStr, BACKGROUND, BLACK );	
  while (1)
  {
    if ( ucXPT2046_TouchFlag == 1 ) {
			actionFlag = backButton();
      ucXPT2046_TouchFlag = 0;
      if (actionFlag == 0) {
        LCD_Clear (0, 0, 240, 320, BACKGROUND);
        return;
      }
    }					
	  HAL_Delay(50);
  }	
}

void hiddenConfigPage(void)
{
  LCD_Clear (0, 0, 240, 320, BACKGROUND);
  /* Button rendering */
  LCD_DrawBox(8, 8, 48, 48);
  pStr = "BACK";
  LCD_DrawString_Color ( 16, 24, pStr, BACKGROUND, BLACK );
  /* Title */
  pStr = "Advanced options";
  LCD_DrawString_Color ( 64, 28, pStr, BACKGROUND, BLACK );	
  while (1)
  {
    if ( ucXPT2046_TouchFlag == 1 ) {
			actionFlag = backButton();
      ucXPT2046_TouchFlag = 0;
      if (actionFlag == 0) {
        LCD_Clear (0, 0, 240, 320, BACKGROUND);
        return;
      }
    }					
	  HAL_Delay(50);
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
  /* USER CODE BEGIN 2 */
  macXPT2046_CS_DISABLE();
  LCD_INIT();
  /* Boot splash */
  pStr = "Welcome to Stream Deck!";			
  LCD_DrawString_Color ( ( usScreenWidth - ( strlen ( pStr ) - 7 ) * WIDTH_EN_CHAR ) >> 2, usScreenHeight >> 1, pStr, BACKGROUND, BLACK );	
  HAL_Delay(2000);
  LCD_Clear(0, 0, 240, 320, BACKGROUND);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /* Main menu */
      pStr = "Stream Deck";			// Title
	    LCD_DrawString_Color ( 8, 8, pStr, BACKGROUND, BLACK );

	    /* Options rendering */
	    LCD_DrawBox(176, 32, 48, 48);
	    pStr = "Audio FX";
	    LCD_DrawString_Color (104, 48, pStr, BACKGROUND, BLACK );
	    // pStr = "Audio";     // Replace me with the icon!
	    // LCD_DrawString_Color (180, 48, pStr, BACKGROUND, BLACK );
      LCD_DrawBox(184, 48, 8, 16);
      LCD_DrawEllipse (194, 56, 12, 6, BLACK);
      LCD_DrawEllipse (202, 56, 16, 6, BLACK);
      LCD_DrawEllipse (210, 56, 20, 6, BLACK);

	    LCD_DrawBox(176, 96, 48, 48);
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

	    LCD_DrawBox(176, 160, 48, 48);
	    pStr = "Dynamic Vibration";
	    LCD_DrawString_Color (32, 176, pStr, BACKGROUND, BLACK );
      /* Icon */
      LCD_DrawBox(182, 166, 36, 36);
	    LCD_DrawEllipse (200, 184, 12, 12, BLACK);

	    LCD_DrawBox(176, 224, 48, 48);
	    pStr = "Options";
      /* Icon */
	    LCD_DrawString_Color (112, 240, pStr, BACKGROUND, BLACK );
	    LCD_DrawEllipse (200, 248, 12, 12, BLACK);

      // Main dish :)
      if ( ucXPT2046_TouchFlag == 1 ) {
        actionFlag = menuButton();
        ucXPT2046_TouchFlag = 0;
        switch (actionFlag) {
          case 0: audiofxConfigPage(); break;
          case 1: displayConfigPage(); break;
          case 2: dvConfigPage(); break;
          case 3: hiddenConfigPage(); break;
        }
      }					
      HAL_Delay(50);
    /* USER CODE BEGIN 3 */
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
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
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
  hsd.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

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
