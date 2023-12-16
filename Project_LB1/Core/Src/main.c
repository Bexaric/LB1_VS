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
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include <locale.h>
#include <time.h>
#include "disp1color.h"
#include "font.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define f6x8_MONO_WIDTH 6
#define WIDTH 32
#define HEIGHT 16
#define MAX_LENGTH 100
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t data1[16], data2[16], data3[16], data4[16];

int16_t k = 1;
int8_t status = 0;
int8_t r = 10;
int8_t poz = 0;
float T2 = 0.196;
float T1 = 0.077;
float t = 0;
float tp = 0;
float f1,f2,f3,f4,f5,F = 0, Fv = 0;
int8_t state = 0;
float ppp = 0;

int8_t hitbox1[WIDTH] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void disp_row(uint16_t row) {

	if (row == 0) {

		for (uint8_t i = 0; i < 6; i++) {
			HAL_SPI_Transmit(&hspi1, &data1, 16, 10);
		}

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);
	}
	if (row == 1) {

		for (uint8_t i = 0; i < 6; i++) {
			HAL_SPI_Transmit(&hspi1, &data2, 16, 10);
		}

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);
	}

	if (row == 2) {

		for (uint8_t i = 0; i < 6; i++) {
			HAL_SPI_Transmit(&hspi1, &data3, 16, 10);
		}

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);
	}

	if (row == 3) {

		for (uint8_t i = 0; i < 6; i++) {
			HAL_SPI_Transmit(&hspi1, &data4, 16, 10);
		}

		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);
	}

	HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_SET);
	for (uint32_t x = 0; x <= 300; x++) {
	};
	HAL_GPIO_WritePin(nOE_GPIO_Port, nOE_Pin, GPIO_PIN_RESET);
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
  MX_TIM1_Init();
  MX_ADC3_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  status = !HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
	  if (status == 1 && poz != 1) {
		  r = rand() % 4;
		  tp = 0;
		  poz = 1;
	  }
	  else if (status == 0 && poz == 1) {
		  poz = 10;
	  }

	  if (r == 0) {
	  	Fv = 0.1 * (1.0206 * exp(-0.2 * tp) * sin(0.9798 * tp));
	  }
	  else if (r == 1) {
	  	Fv = 0.3 * (1.0206 * exp(-0.2 * tp) * sin(0.9798 * tp));
	  }
	  else if (r == 2) {
	  	Fv = 0.55 * (1.0206 * exp(-0.2 * tp) * sin(0.9798 * tp));
	  }
	  else if (r == 3) {
	  	Fv = 0.7 * (1.0206 * exp(-0.2 * tp) * sin(0.9798 * tp));
	  }

	  f1 = sqrt((4*pow(T2,2) - pow(T1,2))/(4*pow(T2,4)));
	  f2 = -T1 * k * exp(-T1*t/(2*pow(T2,2)))*sin(t * f1);
	  f3 = 2 * pow(T2,2) * k * f1;
	  f4 = - 2 * pow(T2,2) * k * f1 * exp(-T1*t/(2*pow(T2,2))) * cos(t * f1);
	  f5 = 2 * pow(T2,2) * f1;
	  F = (f2 + f3 + f4) / f5 + Fv;
	  ppp = round(F * 10);

	  // случай, когда массив не заполнен
	  if (hitbox1[0] == 0) {
	  	for (int8_t i = WIDTH-1; i >= 0; i--) {
	  		if (hitbox1[i] == 0) {
	  			hitbox1[i] = ppp;
	  			break;
	  		}
	  	}
	  }
	  // случай, когда весь массив заполнен
	  if (hitbox1[0] != 0) {
	  	hitbox1[31] = 0;
	  	for (int8_t i = WIDTH-1; i >= 0; i--) {
	  		hitbox1[i] = hitbox1[i-1];
	  	}
	  	hitbox1[0] = ppp;
	  }
	  // рисуем на доске
	  for (uint16_t i = 0; i < HEIGHT; i++) {
	  		for (int8_t j = WIDTH-1; j >= 0; j--) {
	  			disp1color_DrawPixel(j, i, 0);
	  		}
	  }

	  for (int8_t i = WIDTH-1; i >= 0; i--) {
	  	  disp1color_DrawPixel(i, hitbox1[i], 1);
	  }

	  disp1color_UpdateFromBuff();
	  prepare_data();
	  for (uint8_t i = 0; i < 20; i++) {
		  disp_row(0);
		  disp_row(1);
		  disp_row(2);
		  disp_row(3);
	  }

    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
