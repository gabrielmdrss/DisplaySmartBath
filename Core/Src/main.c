/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "menu.h"
#include "math.h"
#include <stdio.h>

uint8_t flag_botao_k0 = 0;
uint8_t flag_botao_k1 = 0;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define COUNT_LIM 50

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim9;

UART_HandleTypeDef huart1;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

uint16_t temp = 36;

char buffer[36];

int16_t contador_Desligar = 0;
int16_t contador_Iniciar = 0;
int16_t flag_iniciar_banho = 0;
int16_t flag_estado_banho = 0;
int16_t flag_desligar_banho = 0;
int16_t flag_resistencia_ativa = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FSMC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM9_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */
  Init_SSD1963();
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start_IT(&htim9);
  TFT_Clear_Screen(WHITE);
  starter_Screen();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(current_screen == 0){

		if (current_screen != ultimo_estado_menu)
		{
			TFT_Draw_Fill_Rectangle(0, 81, 800, 317, VIVID_BLUE);
			home_screen();
			header();
			fotter();
			flag_botao_on = flag_botao_off = flag_botao_up = flag_botao_down = 0;
		}

		atualiza_Cursor();

		if (flag_botao_on){
			current_screen = 1;
			flag_botao_on = 0;

//			if(item_selected == 0)
//			{
//				sprintf(buffer, "%d", temp);
//				LCD_Font(70, 300, buffer,_Open_Sans_Bold_72, 2, BLACK);
//				LCD_Font(250, 225, "C",_Open_Sans_Bold_48, 1, BLACK);
//				TFT_Draw_Circle(245, 185, 4, 0, 1, BLACK);
//				TFT_Draw_Fill_Triangle_Simple(255, 240, 30, 30, 1, BLACK);
//				TFT_Draw_Fill_Triangle_Inverted_Simple(255, 280, 30, 30, 1, BLACK);
//			}
		}


		if (flag_botao_down)
		{
			flag_botao_down = 0;
			item_selected++;

			if (item_selected >= NUM_ITEMS)
				item_selected = 0;
		}

		if(flag_botao_up)
		{
			flag_botao_up = 0;
			item_selected--;

			if (item_selected < 0)
				item_selected = NUM_ITEMS - 1;
		}

	  }
	  else
	  {
			if (current_screen != ultimo_estado_menu)
			{
				TFT_Draw_Fill_Rectangle(0, 81, 800, 317, VIVID_BLUE);
				ultimo_estado_menu = current_screen;
				header();
				fotter();
				flag_botao_on = flag_botao_off = flag_botao_up = flag_botao_down = 0;
			}

			/********************* BANHO *************************/
			if(item_selected == 0)
			{
//				LCD_Font(40, 120, "Ajuste a temperatura do banho:",_Open_Sans_Bold_28, 1, BLACK);
//				TFT_Draw_Rectangle(55, 160, 250, 160, 4, CYAN);
//
//				if(flag_estado_banho)
//				{
//					if(!flag_resistencia_ativa && !flag_desligar_banho)
//					{
//						flag_resistencia_ativa = 1;
//						LCD_Font(600, 200, "Aquecendo", _Open_Sans_Bold_24, 1, RED);
//						TFT_Draw_Circle(580, 192, 13, 1, 1, RED);
//					}
//					else
//					{
//						if (flag_botao_off)
//						{
//							flag_resistencia_ativa = 0;
//							flag_desligar_banho = 1;
//							flag_botao_on = flag_botao_off = flag_botao_up = flag_botao_down = 0;
//							LCD_Font(600, 200, "Aquecendo", _Open_Sans_Bold_24, 1, WHITE);
//							TFT_Draw_Circle(580, 192, 13, 1, 1, WHITE);
//						}
//					}
//
//
//				}
//				else
//				{
//					if(flag_botao_on && !flag_estado_banho)
//					{
//						flag_botao_on = 0;
//						flag_iniciar_banho = 1;
//						LCD_Font(600, 250, "Recirculando", _Open_Sans_Bold_24, 1, RED);
//						TFT_Draw_Circle(580, 242, 13, 1, 1, RED);
////						TFT_Draw_Rectangle(597, 97, 186, 54, 2, BLACK);
//					}
//
//
//				}

				if (flag_botao_off)
				{
					flag_botao_off = 0;
					flag_botao_on = flag_botao_off = flag_botao_up = flag_botao_down = 0;
					current_screen = 0;
				}

				if(flag_botao_up)
				{
					flag_botao_up = 0;
					temp++;

					if(temp > 42) temp = 42;

					TFT_Draw_Fill_Rectangle(70, 195, 180, 120, WHITE);
					sprintf(buffer, "%d", temp);
					LCD_Font(70, 300, buffer,_Open_Sans_Bold_72, 2, BLACK);
				}

				if(flag_botao_down)
				{
					flag_botao_down = 0;
					temp--;

					if(temp < 36) temp = 36;

					TFT_Draw_Fill_Rectangle(70, 195, 180, 120, WHITE);
					sprintf(buffer, "%d", temp);
					LCD_Font(70, 300, buffer,_Open_Sans_Bold_72, 2, BLACK);
				}

			}
			else if(item_selected == 1)
			{
				if (flag_botao_off)
				{
					flag_botao_off = 0;
					flag_botao_on = flag_botao_off = flag_botao_up = flag_botao_down = 0;
					current_screen = 0;
				}
			}
			else if(item_selected == 2)
			{
				if (flag_botao_off)
				{
					flag_botao_off = 0;
					flag_botao_on = flag_botao_off = flag_botao_up = flag_botao_down = 0;
					current_screen = 0;
				}
			}
			else if(item_selected == 3)
			{
				if (flag_botao_off)
				{
					flag_botao_off = 0;
					flag_botao_on = flag_botao_off = flag_botao_up = flag_botao_down = 0;
					current_screen = 0;
				}
			}
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 8399;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 49;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 16799;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 1999;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 1000000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : PE2 PE3 PE4 PE5 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
  hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 1;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 5;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FSMC_Init 2 */

  /* USER CODE END FSMC_Init 2 */
}

/* USER CODE BEGIN 4 */

int __io_putchar(int ch) {
	USART1->DR = (ch & (uint16_t) 0x01FF);
	while (!(USART1->SR & USART_SR_TXE))
		; //espera pelo fim da transmissão do caractere para evitar a segunda transmissão antes da primeira ser concluída
	return ch;
}

int __io_getchar(void) {
	return (uint16_t) (USART1->DR & (uint16_t) 0x01FF);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if(htim->Instance == TIM7)
	{
		int8_t botao_selecionado = scan_membrana();
		if(botao_selecionado != -1){

			switch(botao_selecionado){

			case 10:
				flag_botao_on = 1;
				break;

			case 1:
				flag_botao_off = 1;
				break;

			case 2:
				flag_botao_up = 1;
				break;

			case 3:
				flag_botao_down = 1;
				break;
			}
		}
	}
	if(htim->Instance == TIM9)
	{
		if(flag_iniciar_banho)
		{
			if(contador_Iniciar >= COUNT_LIM)
			{
				contador_Iniciar = 0;
				flag_estado_banho = 1;
				flag_iniciar_banho = 0;
//				TFT_Draw_Fill_Rectangle(597, 97, 186, 54, WHITE);
			}
			else
			{
				contador_Iniciar++;
//				uint16_t w = (180 * contador_Iniciar) / COUNT_LIM;
//				TFT_Draw_Fill_Rectangle(600, 100, w, 50, BLACK);
			}
		}

		if(flag_desligar_banho)
		{
			if(contador_Desligar >= COUNT_LIM)
			{
				flag_estado_banho = 0;
				contador_Desligar = 0;
				flag_desligar_banho = 0;
				LCD_Font(600, 250, "Recirculando", _Open_Sans_Bold_24, 1, WHITE);
				TFT_Draw_Circle(580, 242, 13, 1, 1, WHITE);
			}
			else contador_Desligar++;
		}
	}
}

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
