/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include <stdlib.h>
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t count=0;
#define Address 0x08007800
uint32_t wait = 250; // 5 sec
#define FLASH_USER_START_ADDR 0x08007000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void Flash_ErasePage(uint32_t pageAddress);
void WriteData(uint8_t Data);
void RetriveData(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Flash_EraseSector(uint32_t address, uint32_t size)
{
	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef eraseInitStruct;
	uint32_t pageError;

	eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	eraseInitStruct.Page = address;
	eraseInitStruct.NbPages = (size+FLASH_PAGE_SIZE-1)/FLASH_PAGE_SIZE;
	if(HAL_FLASHEx_Erase(&eraseInitStruct, &pageError)!= HAL_OK)
	{

	}
	HAL_FLASH_Lock();

}
void RetriveData(void)
{
	//uint8_t Data =(uint8_t)(*((volatile uint32_t*)Address));
	uint8_t Data = *(volatile uint8_t*)Address;

	if(Data>=0 && Data <4)
	{
		count=Data;
	}else
	{
		count=3;
	}
}
void Flash_ErasePage(uint32_t pageAddress)
{
	HAL_FLASH_Unlock();

	FLASH_EraseInitTypeDef eraseInitStruct;
	uint32_t pageError;

	eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	eraseInitStruct.Page = (pageAddress - FLASH_BASE) / FLASH_PAGE_SIZE;
	eraseInitStruct.NbPages = 1;

	if (HAL_FLASHEx_Erase(&eraseInitStruct, &pageError) != HAL_OK)
	{
		// Error handling
	}

	HAL_FLASH_Lock();
}

void WriteData(uint8_t Data)
{
	HAL_StatusTypeDef status;
	HAL_FLASH_Unlock();
	status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (volatile uint32_t)Address, Data);
	HAL_FLASH_Lock();
}
void Flash_Write(uint32_t address, uint32_t *data, uint32_t length)
{
	HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < length; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data[i]) != HAL_OK) {
            // Error handling
            break;
        }
        address += 4;
    }

    HAL_FLASH_Lock();
}

void Flash_Read(uint32_t address, uint32_t *data, uint32_t length)
{
    for (uint32_t i = 0; i < length; i++) {
        data[i] = *(__IO uint32_t*)address;
        address += 4;
    }
}

void IntToStr(int num, char *str) {
    sprintf(str, "%d", num);
}

int StrToInt(const char *str) {
    return atoi(str);
}

void FloatToStr(float num, char *str) {
    sprintf(str, "%.2f", num);
}

float StrToFloat(const char *str) {
    return atof(str);
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
  /* USER CODE BEGIN 2 */
	uint32_t tickstart = HAL_GetTick();
	RetriveData();

	uint32_t dataToWrite[2] = {0x12345678, 0x9ABCDEF0};
	uint32_t dataRead[2] = {0};

//	    Flash_ErasePage(FLASH_USER_START_ADDR);
//	    Flash_Write(FLASH_USER_START_ADDR, dataToWrite, 2);
//	    Flash_Read(FLASH_USER_START_ADDR, dataRead, 2);
//
//	    char str[10];
//	    IntToStr(1234, str);
//	    int num = StrToInt(str);
//
//	    FloatToStr(1234.56, str);
//	    float fnum = StrToFloat(str);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		switch (count)
		{
		case 1:
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
			break;
		default:
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
			break;
		}
		if ((HAL_GetTick() - tickstart) > wait)
		{
			Flash_ErasePage(Address);
			WriteData(count);
			tickstart = HAL_GetTick();
		}
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_Pin */
  GPIO_InitStruct.Pin = KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


//void WriteData(uint32_t address, uint32_t *data, uint32_t length)
//{
//	 HAL_FLASH_Unlock();
//
//	    HAL_StatusTypeDef status;
//
//	    for (uint32_t i = 0; i < length; i++) {
//	        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data[i]);
//	        if (status != HAL_OK) {
//	            // Error handling
//	            break;
//	        }
//	        address += 4;
//	    }
//
//	    HAL_FLASH_Lock();
//}


//
//void Flash_EraseSector(uint32_t address, uint32_t size)
//{
//	HAL_FLASH_Unlock();
//	 FLASH_EraseInitTypeDef eraseInitStruct;
//	    uint32_t pageError;
//
//	    eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
//	    eraseInitStruct.Page = address;
//	    eraseInitStruct.NbPages = (size + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE;
//
//	    if (HAL_FLASHEx_Erase(&eraseInitStruct, &pageError) != HAL_OK) {
//	        // Error handling
//	    }
//
//	    HAL_FLASH_Lock();
//}
//
//void Flash_Write(uint32_t address, uint32_t *data, uint32_t length)
//{
//	HAL_FLASH_Unlock();
//
//for (uint32_t i = 0; i < length; i++) {
//    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data[i]) != HAL_OK) {
//        // Error handling
//        break;
//    }
//    address += 4;
//}
//
//HAL_FLASH_Lock();
//
//}
//
//void Flash_Read(uint32_t address, uint32_t *data, uint32_t length) {
//    for (uint32_t i = 0; i < length; i++) {
//        data[i] = *(__IO uint32_t*)address;
//        address += 4;
//    }
//}
//
//void IntToStr(int num, char *str) {
//    sprintf(str, "%d", num);
//}
//
//int StrToInt(const char *str) {
//    return atoi(str);
//}
//
//void FloatToStr(float num, char *str) {
//    sprintf(str, "%.2f", num);
//}
//
//float StrToFloat(const char *str) {
//    return atof(str);
//}
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
