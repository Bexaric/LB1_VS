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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "disp1color.h"
#include "font.h"
#include <locale.h>
#include <time.h>
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
ADC_HandleTypeDef hadc3;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim12;

/* USER CODE BEGIN PV */

extern uint8_t data1[16], data2[16], data3[16], data4[16];

uint16_t x = 3; // Отвечает за отключение платы для загрузки данных
uint16_t score = 0; // Счётчик очков игры
uint16_t game_over = 3; // Состояние игры (3 - начальный экран, 0 - игровой процесс, 2 - конечный экран)
uint16_t poz = 0; // Состояние прыжка (0 – нет прыжка, 1 – состояние прыжка, 2 – промежуточное состояние)
uint16_t adc_button = 0; // Состояние кнопки
uint16_t move = 1; // Состояние анимации динозаврика
uint16_t h = 0; // Расстояние ног динозаврика до поверхности (состояние прыжка)
uint16_t jump_state = 0; // Счетчик состояния динозаврика в прыжке
int16_t mcact = -5; // Отвечает за сдвиг кактуса на экране (эффект движения)
uint16_t timer = 5; // Отвечает за время для генерации нового кактуса
uint16_t count = 0; // Отвечает за время нахождения одного кактуса на экране
uint16_t numb = 100; // Отвечает за вид кактуса (0 - узкая длинная, 1 - узкая короткая, 2 - широкая длинная, 3 - широкая короткая)
uint16_t beg = 0; // Отвечает за состояние анимации мерцании
uint16_t flag = 0; // Отвечает за состояние анимации вывода текста на экран ( 2 - вывод фразы и счёта, 3 - вывод счёта)
uint16_t jump_time = 100; // Время зажатия кнопки (Если >=2 - высокий прыжок, <2 - низкий прыжок)
uint16_t kof = 0; // Отвечает за корректную работу двух типов прыжка
char hitbox[HEIGHT][WIDTH + 10]; // Отвечает за хитбоксы кактусов и динозаврика. При встрече двух хитбоксов игра заканчивается.

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM9_Init(void);
static void MX_TIM12_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// функция для загрузки данных по spi
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

// функция отрисовки пола на экране
void draw_screen() {
	for (uint16_t i = 0; i < HEIGHT; i++) {
		for (uint16_t j = 0; j < WIDTH; j++) {
			if (i > HEIGHT - 2) {
				disp1color_DrawPixel(j, i, 1);
			} else {
				disp1color_DrawPixel(j, i, 0);
			}
		}
	}
}

// функция отрисовки анимации мерцания экрана в начале игры и в конце игры
void gameove(uint16_t k) {
	for (uint16_t i = 0; i < HEIGHT; i++) {
		for (uint16_t j = 0; j < WIDTH; j++) {
			disp1color_DrawPixel(j, i, k);
		}
	}

}

// функция отрисовки анимации для начала игры (после нажатия кнопки)
void gamebeg() {
	for (uint16_t i = 0; i < HEIGHT; i++) {
		for (uint16_t j = 0; j < WIDTH; j++) {
			disp1color_DrawPixel(j, i, 0);
		}
	}
	for (int i = 0; i < beg; i++) {
		for (uint16_t j = 0; j <= i; j++) {
			disp1color_DrawRectangle(16 - i * 2, 8 - i, 16 + i * 2, 8 + i);
			disp1color_DrawRectangle(16 - i, 8 - i, 16 + i, 8 + i);
			disp1color_DrawRectangle(15 - i, 8 - i, 15 + i, 8 + i);
			disp1color_DrawRectangle(15 - i * 2, 8 - i, 15 + i * 2, 8 + i);
		}
	}

}

// функция хитбоксов моделек. Здесь определяется случай, если динозавр задевает кактусы
void draw_hitbox(int16_t wmin, uint16_t hmax, uint16_t wmax, uint16_t hmin,
		uint16_t cact) {
	for (uint16_t i = 0; i < HEIGHT; i++) {
		for (int16_t j = 0; j < WIDTH + 10; j++) {
			hitbox[i][j] = 'A';
			//disp1color_DrawPixel(j, i, 0);
			if (i > 7 - h && i < 15 - h && j < 7) {
				hitbox[i][j] = 'D';
				//disp1color_DrawPixel(j, i, 1);
			}
			if (wmin < 0) {
				wmin = 0;
			}
			if (i >= hmin && i <= hmin && j <= wmax && j >= wmin && cact < 32) {
				//disp1color_DrawPixel(j, i, 1);
				if (hitbox[i][j] == 'D') {
					game_over = 2;
				}

			}
		}
	}
}

// отрисовка прыжка в зависимости от высоты
void jump_up(uint16_t h) {
	disp1color_DrawPixel(5, 9 - h, 1);
	disp1color_DrawLine(2, 12 - h, 3, 12 - h);
	disp1color_DrawRectangle(4, 10 - h, 6, 8 - h);
	disp1color_DrawRectangle(4, 11 - h, 5, 10 - h);
	disp1color_DrawRectangle(1, 12 - h, 4, 11 - h);
	disp1color_DrawRectangle(0, 12 - h, 1, 10 - h);
	disp1color_DrawPixel(1, 10 - h, 0);

	disp1color_DrawPixel(1, 14 - h, 1);
	disp1color_DrawPixel(1, 13 - h, 1);
	disp1color_DrawPixel(2, 14 - h, 1);
	disp1color_DrawPixel(4, 14 - h, 1);
	disp1color_DrawPixel(4, 13 - h, 1);
	disp1color_DrawPixel(5, 14 - h, 1);
}

// функция отрисовки анимации бега динозавра
void draw_dino() {
	if (move == 1) {
		disp1color_DrawPixel(5, 9, 1);
		disp1color_DrawLine(2, 12, 3, 12);
		disp1color_DrawRectangle(4, 10, 6, 8);
		disp1color_DrawRectangle(4, 11, 5, 10);
		disp1color_DrawRectangle(1, 12, 4, 11);
		disp1color_DrawRectangle(0, 12, 1, 10);
		disp1color_DrawPixel(1, 10, 0);

		disp1color_DrawPixel(1, 13, 1);
		disp1color_DrawPixel(2, 13, 1);

		disp1color_DrawPixel(4, 14, 1);
		disp1color_DrawPixel(4, 13, 1);
		disp1color_DrawPixel(5, 14, 1);
	} else if (move == 2) {
		disp1color_DrawPixel(5, 9, 1);
		disp1color_DrawLine(2, 12, 3, 12);
		disp1color_DrawRectangle(4, 10, 6, 8);
		disp1color_DrawRectangle(4, 11, 5, 10);
		disp1color_DrawRectangle(1, 12, 4, 11);
		disp1color_DrawRectangle(0, 12, 1, 10);
		disp1color_DrawPixel(1, 10, 0);

		disp1color_DrawPixel(1, 14, 1);
		disp1color_DrawPixel(1, 13, 1);
		disp1color_DrawPixel(2, 14, 1);

		disp1color_DrawPixel(4, 13, 1);
		disp1color_DrawPixel(5, 13, 1);
	} else if (move == 3) {
		disp1color_DrawPixel(5, 9, 1);
		disp1color_DrawLine(2, 12, 3, 12);
		disp1color_DrawRectangle(4, 10, 6, 8);
		disp1color_DrawRectangle(4, 11, 5, 10);
		disp1color_DrawRectangle(1, 12, 4, 11);
		disp1color_DrawRectangle(0, 12, 1, 10);
		disp1color_DrawPixel(1, 10, 0);
		disp1color_DrawPixel(1, 14, 1);
		disp1color_DrawPixel(1, 13, 1);
		disp1color_DrawPixel(2, 14, 1);
		disp1color_DrawPixel(4, 14, 1);
		disp1color_DrawPixel(4, 13, 1);
		disp1color_DrawPixel(5, 14, 1);
	}

}

// функция отрисовки кактуса в зависимости от числа numb (от 0 до 3 включительно)
void draw_cactus(uint16_t numb) {
	if (numb == 0) { // узкая длинная
		disp1color_DrawLine(30 - mcact, 11, 30 - mcact, 13);
		disp1color_DrawLine(26 - mcact, 11, 26 - mcact, 13);
		disp1color_DrawLine(28 - mcact, 14, 28 - mcact, 11);
		disp1color_DrawLine(30 - mcact, 13, 26 - mcact, 13);
		draw_hitbox(26 - mcact, 14, 30 - mcact, 11, mcact);
	} else if (numb == 1) { // узкая короткая
		disp1color_DrawLine(30 - mcact, 13, 30 - mcact, 12);
		disp1color_DrawLine(26 - mcact, 13, 26 - mcact, 12);
		disp1color_DrawLine(28 - mcact, 14, 28 - mcact, 12);
		disp1color_DrawLine(30 - mcact, 13, 26 - mcact, 13);
		draw_hitbox(26 - mcact, 14, 30 - mcact, 12, mcact);
	} else if (numb == 2) { // широкая длинная
		disp1color_DrawLine(31 - mcact, 13, 31 - mcact, 11);
		disp1color_DrawLine(25 - mcact, 13, 25 - mcact, 11);
		disp1color_DrawLine(28 - mcact, 14, 28 - mcact, 11);
		disp1color_DrawLine(31 - mcact, 13, 25 - mcact, 13);
		draw_hitbox(25 - mcact, 14, 31 - mcact, 11, mcact);
	} else if (numb == 3) { // широкая короткая
		disp1color_DrawLine(31 - mcact, 13, 31 - mcact, 12);
		disp1color_DrawLine(25 - mcact, 13, 25 - mcact, 12);
		disp1color_DrawLine(28 - mcact, 14, 28 - mcact, 12);
		disp1color_DrawLine(31 - mcact, 13, 25 - mcact, 13);
		draw_hitbox(25 - mcact, 14, 31 - mcact, 12, mcact);
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
  MX_TIM1_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM9_Init();
  MX_TIM12_Init();
  MX_TIM10_Init();
  MX_TIM2_Init();
  MX_ADC3_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim4);
	HAL_TIM_Base_Start_IT(&htim9);
	HAL_TIM_Base_Start_IT(&htim12);
	HAL_TIM_Base_Start_IT(&htim10);

	char *gamebegin = "CLICK"; // слово, которое выводится в начале игры
	char *gameover = "GAMEOVER SCORE IS "; // слово, которое выводится в конце игры
	char *scor[5]; // результат игры (количество очков)

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		// цикл для игрового режима. Здесь происходит анимация экрана, движения динозаврика и кактусов.
		if (!game_over) {
			draw_screen();
			// генерация и движение кактуса по экрану
			if (count == 0) {
				timer = rand() % 4 + 2;
				count++;
			} else if (count == timer - 1) {
				mcact = -5;
				numb = rand() % 4;
			} else if (count <= timer + 4 && count >= timer) {
				draw_cactus(numb);
			}
			// анимация прыжка при нажатии кнопки
			if (poz == 1) {
				if (jump_state == 0) {
					jump_state++;
					jump_time = 0;
					h = 0;
					jump_up(h);
				} else if (jump_state <= 2) {
					h = 1;
					jump_up(h);
				} else if (jump_state <= 3) {
					h = 4;
					jump_up(h);
				} else if (jump_state <= 4) {
					h = 6;
					jump_up(h);
				} else if (jump_state <= 5 && jump_time >= 2) {
					h = 8;
					kof = 1;
					jump_up(h);
				} else if (jump_state <= 6 && kof == 1) {
					h = 8;
					jump_up(h);
				} else if (jump_state <= 8 && kof == 1) {
					h = 6;
					jump_up(h);
				} else if (jump_state <= (10 - kof)) {
					h = 4;
					jump_up(h);
				} else if (jump_state <= (11 - kof)) {
					h = 1;
					jump_time = 0;
					jump_up(h);
				} else if (jump_state <= (12 - kof)) {
					h = 0;
					jump_up(h);
					kof = 5;
				}
			} else if (jump_state == 0) {
				draw_dino();
			}
			// загрузка данных на LED-панель и их отрисовка
			disp1color_UpdateFromBuff();
			prepare_data();
			for (uint8_t i = 0; i < 20; i++) {
				disp_row(0);
				disp_row(1);
				disp_row(2);
				disp_row(3);
			}
		// цикл для конечного экрана. Он активируется при соприкосновении хитбоксов динозавра и кактуса
		} else if (game_over == 2) {
			if (flag == 1) {
				flag = 0;
			}

			sprintf(scor, "%d", score);
			char *pMyStr[1000];
			if (!beg) {
				beg = 1;
			}
			// цикл отвечает за анимацию мерцания
			if (beg % 2 == 0 && beg <= 9 && !flag) {
				gameove(1);
				disp1color_UpdateFromBuff();
				prepare_data();
				for (uint8_t i = 0; i < 20; i++) {
					disp_row(0);
					disp_row(1);
					disp_row(2);
					disp_row(3);
				}
			} else if (beg % 2 == 1 && beg <= 9 && !flag) {
				gameove(0);
				disp1color_UpdateFromBuff();
				prepare_data();
				for (uint8_t i = 0; i < 20; i++) {
					disp_row(0);
					disp_row(1);
					disp_row(2);
					disp_row(3);
				}
			}

			// отвечает за вывод информации на экран
			snprintf(pMyStr, sizeof pMyStr, "%s%s", gameover, scor);
			uint16_t strSize = strlen(pMyStr);
			uint8_t symbolDelay = 2;
			if ((beg > 9 && flag == 0) || flag == 2) {
				flag = 2;
				for (int16_t x = 30;
						x > -((strSize + symbolDelay) * f6x8_MONO_WIDTH); x--) {
					disp1color_printf(x, 4, FONTID_6X8M, pMyStr);
					disp1color_UpdateFromBuff();
					prepare_data();
					for (uint8_t i = 0; i < 20; i++) {
						disp_row(0);
						disp_row(1);
						disp_row(2);
						disp_row(3);
					}
				}
				flag = 3;
			}
			// после вывода информации на конечном экране выводит счёт в центр и зависает
			if (flag == 3) {
				snprintf(pMyStr, sizeof pMyStr, "%s", scor);
				disp1color_printf(11, 4, FONTID_6X8M, pMyStr);
				disp1color_UpdateFromBuff();
				prepare_data();
				for (uint8_t i = 0; i < 20; i++) {
					disp_row(0);
					disp_row(1);
					disp_row(2);
					disp_row(3);
				}
			}
		// цикл для анимации начального экрана
		} else if (game_over == 3) {
			// отвечает за вывод строки переменной gamebegin
			if (!beg) {
				char *pMyStr[100];
				snprintf(pMyStr, sizeof pMyStr, "%s", gamebegin);
				disp1color_printf(1, 4, FONTID_6X8M, pMyStr);
				disp1color_UpdateFromBuff();
				prepare_data();
				for (uint8_t i = 0; i < 20; i++) {
					disp_row(0);
					disp_row(1);
					disp_row(2);
					disp_row(3);
				}
			}

			// цикл для анимации начальной анимации
			if (adc_button || flag) {
				flag = 1;
				if (!beg) {
					beg = 1;
				}
				if (beg <= 8) {
					for (uint16_t i = 0; i <= 2; i++) {
						gamebeg();
						disp1color_UpdateFromBuff();
						prepare_data();
						for (uint8_t i = 0; i < 20; i++) {
							disp_row(0);
							disp_row(1);
							disp_row(2);
							disp_row(3);
						}
					}

				}
				// цикл отрисовки падения динозавра перед началом игры
				if (beg > 8) {
					draw_screen();
					if (jump_state == 0) {
						jump_state++;
						h = 12;
						jump_up(h);
					} else if (jump_state <= 2) {
						h = 10;
						jump_up(h);
					} else if (jump_state <= 3) {
						h = 8;
						jump_up(h);
					} else if (jump_state <= 4) {
						h = 6;
						jump_up(h);
					} else if (jump_state <= 6) {
						h = 4;
						jump_up(h);
					} else if (jump_state <= 8) {
						h = 2;
						jump_up(h);
					} else if (jump_state <= 9) {
						h = 0;
						poz = 2;
						jump_up(h);
					}
					disp1color_UpdateFromBuff();
					prepare_data();
					for (uint8_t i = 0; i < 20; i++) {
						disp_row(0);
						disp_row(1);
						disp_row(2);
						disp_row(3);
					}
				}
				if (jump_state == 11) {
					game_over = 0;
					kof = 5;
				}
			}
		}
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 4000;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 2000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 40-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 50000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 80-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 50000-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 20-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 50000-1;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 150-1;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 50000-1;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief TIM12 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM12_Init(void)
{

  /* USER CODE BEGIN TIM12_Init 0 */

  /* USER CODE END TIM12_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM12_Init 1 */

  /* USER CODE END TIM12_Init 1 */
  htim12.Instance = TIM12;
  htim12.Init.Prescaler = 320-1;
  htim12.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim12.Init.Period = 50000-1;
  htim12.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim12.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim12) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim12, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM12_Init 2 */

  /* USER CODE END TIM12_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, nOE_Pin|SCLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, A_Pin|B_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : nOE_Pin SCLK_Pin */
  GPIO_InitStruct.Pin = nOE_Pin|SCLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : A_Pin B_Pin */
  GPIO_InitStruct.Pin = A_Pin|B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
	while (1) {
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
