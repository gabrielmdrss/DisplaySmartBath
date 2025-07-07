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
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim9;

UART_HandleTypeDef huart1;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FSMC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM9_Init(void);
static void MX_ADC1_Init(void);
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
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  Init_SSD1963();
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_TIM_Base_Start_IT(&htim9);
  TFT_Clear_Screen(VIVID_BLUE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

/************************************   MENU PRINCIPAL  *********************************************/
	  if(current_screen == 0){


		// Verifica se houve uma troca de telas, para imprimir apenas uma vez os elementos
		if (current_screen != ultimo_estado_menu)
		{
			TFT_Draw_Fill_Rectangle(0, 75, 800, 323, VIVID_BLUE);	// Limpa a tela
			starter_Screen();	// Imprime o esqueleto do menu
			home_screen();		// Printa os rótulos das opções e elementos individuais
			zeraBotoes();		// Zera os botões
		}

		// Verifica se alguém clicou no botão de troca de tela
		if (flag_botao_on){
			flag_escrita_tela = 1;	// Seta a flag de atualizar o rodapé e cabeçalho
			current_screen = 1;		// Muda o valor da tela atual
			zeraBotoes();			// Zera o valor da flag
		}

		// Verifica se alguém clicou no item onde descerá pelas opções da interface
		if (flag_botao_down)
		{
			zeraBotoes();		// Zera a flag dos botões
			item_selected++;	// Incrementa o item selecionado

			// Verifica se chegou no limite da lista e rezeta
			if (item_selected >= NUM_ITEMS)
				item_selected = 0;
		}

		// Verifica se alguém clicou no item onde subirá pelas opções da interface
		if(flag_botao_up)
		{
			zeraBotoes();		// Zera a flag dos botões
			item_selected--;	// Decrementa o item selecionado

			// Verifica se chegou no limite superior da lista e rezeta
			if (item_selected < 0)
				item_selected = NUM_ITEMS - 1;
		}

		// Função responsável por atualizar o cursor quando houver uma mudança no item selecionado
		atualiza_Cursor();
	  }

/************************************   TELA ESPECÍFICA  *********************************************/
	  else
	  {
			if (current_screen != ultimo_estado_menu)
			{
				TFT_Draw_Fill_Rectangle(0, 75, 800, 323, VIVID_BLUE);
				starter_Screen();
				ultimo_estado_menu = current_screen;
				flag_botao_on = flag_botao_off = flag_botao_up = flag_botao_down = 0;
			}

			/********************* BANHO *************************/
			if(item_selected == 0)
			{
				if(flag_estado_banho)
				{
					if(!flag_desligar_banho)
					{
						if(flag_resistencia_ativa)
						{
							flag_resistencia_ativa = flag_resistencia_ativa;
						}
						else if(!flag_resistencia_ativa && !flag_iniciar_banho)
						{
							sprintf(buffer, "%d|25", temp);
							LCD_Font(551, 310, buffer, _Open_Sans_Bold_24, 1, WHITE);
							LCD_Font(631, 307, "c", _Open_Sans_Bold_18, 1, WHITE);
							TFT_Draw_Circle(629, 293, 2, 0, 1, WHITE);
							LCD_Font(661, 307, "Aquecendo",_Open_Sans_Bold_16, 1, WHITE);
							flag_resistencia_ativa = 1;
						}

						if(flag_botao_on || volume_ficticio <= 1.0f)
						{
							TFT_Draw_Fill_Rectangle(0, 75, 800, 323, VIVID_BLUE);
							LCD_Font(150, 185, "Finalizando...", _Open_Sans_Bold_20, 1, WHITE);
							TFT_Draw_Fill_Round_Rect(150, 200, 500, 30, 10, GRAYISH_BLUE);
							flag_desligar_banho = 1;
							flag_resistencia_ativa = 0;
							zeraBotoes();
							flag_iniciar_banho = 0;
							contador_Iniciar = 0;

							flag_escrita_tela = 1;
						}
					}
					else
					{
						if(ultimo_contador_Desligar != contador_Desligar && (contador_Desligar % 5 == 0))
						{
							ultimo_contador_Desligar = contador_Desligar;
							uint16_t w = (500 * contador_Desligar) / COUNT_LIM;
							TFT_Draw_Fill_Round_Rect(150, 200, w, 30, 10, WHITE);
						}
					}
				}
				else
				{
					if (flag_botao_off)
					{
						flag_escrita_tela = 1;
						zeraBotoes();
						flag_botao_on = flag_botao_off = flag_botao_up = flag_botao_down = 0;
						current_screen = 0;
					}

					if(flag_botao_on && !flag_estado_banho && !flag_desligar_banho)
					{
						zeraBotoes();
						if(volume_ficticio >= 5.0f)
						{
							flag_escrita_tela = 1;
							flag_iniciar_banho = 1;
							flag_estado_banho = 1;
							TFT_Draw_Bitmap(603, 337, RECIRCULACAO_WIDTH, RECIRCULACAO_HEIGHT, recirculacao_bitmap, WHITE);
							LCD_Font(643, 357, "Recirculando",_Open_Sans_Bold_16, 1, WHITE);
						}

					}
				}

				if(volume_ficticio >= 10.0f && ultimo_volume_ficticio < 10.0f)
				{
					TFT_Draw_Fill_Round_Rect(620, 175, 150, 25, 5, GREEN);
					LCD_Font(658, 191, "nivel bom",_Open_Sans_Bold_14, 1, VIVID_BLUE);
					ultimo_volume_ficticio = volume_ficticio;
				}
				else if(volume_ficticio > 5.0f && volume_ficticio < 10.0f &&
						(ultimo_volume_ficticio <= 5.0f || ultimo_volume_ficticio >= 10.0f))
				{
					TFT_Draw_Fill_Round_Rect(620, 175, 150, 25, 5, YELLOW);
					LCD_Font(655, 191, "nivel baixo",_Open_Sans_Bold_14, 1, VIVID_BLUE);
					ultimo_volume_ficticio = volume_ficticio;
				}
				else if(volume_ficticio <= 5.0f && ultimo_volume_ficticio > 5.0f)
				{
					TFT_Draw_Fill_Round_Rect(620, 175, 150, 25, 5, RED);
					LCD_Font(630, 191, "nivel insuficiente",_Open_Sans_Bold_14, 1, VIVID_BLUE);
					ultimo_volume_ficticio = volume_ficticio;
				}

				if(flag_botao_up)
				{
					zeraBotoes();
					temp++;

					if(temp > 42) temp = 42;

					else
					{
						TFT_Draw_Fill_Rectangle(160, 185, 150, 100, VIVID_BLUE);
						sprintf(buffer, "%d", temp);
						LCD_Font(160, 280, buffer,_Open_Sans_Bold_128, 1, WHITE);

						TFT_Draw_Fill_Rectangle(546, 290, 35, 30, VIVID_BLUE);
						sprintf(buffer, "%d", temp);

						if(flag_resistencia_ativa) LCD_Font(551, 310, buffer, _Open_Sans_Bold_24, 1, WHITE);
						else LCD_Font(551, 310, buffer, _Open_Sans_Bold_24, 1, GRAYISH_BLUE);
					}
				}

				if(flag_botao_down)
				{
					zeraBotoes();
					temp--;

					if(temp < 36) temp = 36;

					else
					{
						TFT_Draw_Fill_Rectangle(160, 185, 150, 100, VIVID_BLUE);
						sprintf(buffer, "%d", temp);
						LCD_Font(160, 280, buffer,_Open_Sans_Bold_128, 1, WHITE);

						TFT_Draw_Fill_Rectangle(546, 290, 35, 30, VIVID_BLUE);
						sprintf(buffer, "%d", temp);

						if(flag_resistencia_ativa) LCD_Font(551, 310, buffer, _Open_Sans_Bold_24, 1, WHITE);
						else LCD_Font(551, 310, buffer, _Open_Sans_Bold_24, 1, GRAYISH_BLUE);
					}
				}

				if(flag_atualizar_tela && !flag_desligar_banho)
				{
					TFT_Draw_Fill_Rectangle(595, 130, 65, 28, VIVID_BLUE);
					sprintf(buffer, "%.1f", volume_ficticio);
					LCD_Font(600, 155, buffer,_Open_Sans_Bold_28, 1, WHITE);
					flag_atualizar_tela = 0;
				}
			}

			/********************* ABASTECIMENTO *************************/
			else if(item_selected == 1)
			{
				if(volume_ficticio >= 19.5f)
				{
					if(flag_estado_abastecimento) flag_estado_abastecimento = 0;

					if(!flag_volume)
					{
						TFT_Draw_Bitmap(105, 308, BANHO_PRONTO_WIDTH, BANHO_PRONTO_HEIGHT, banho_pronto_bitmap, GREEN);
						TFT_Draw_Fill_Round_Rect(145, 310, 125, 25, 5, GREEN);
						LCD_Font(163, 326, "Abastecido", _Open_Sans_Bold_14, 1, VIVID_BLUE);
						flag_escrita_tela = 1;
						flag_volume = 1;
					}
					else
					{
						if (flag_botao_off)
						{
							flag_escrita_tela = 1;
							flag_volume = 0;
							zeraBotoes();
							current_screen = 0;
						}
					}
				}
				else
				{
					if(flag_volume)
					{
						flag_volume = 0;
						flag_escrita_tela = 1;
						TFT_Draw_Fill_Round_Rect(105, 305, 165, 30, 5, VIVID_BLUE);
					}

					if(flag_estado_abastecimento)
					{
						if (flag_botao_on)
						{
							flag_escrita_tela = 1;
							zeraBotoes();
							flag_estado_abastecimento = 0;
							TFT_Draw_Bitmap(105, 308, BANHO_PRONTO_WIDTH, BANHO_PRONTO_HEIGHT, banho_pronto_bitmap, VIVID_BLUE);
							TFT_Draw_Fill_Round_Rect(145, 310, 125, 25, 5, VIVID_BLUE);
						}
					}
					else
					{
						if(flag_botao_on && volume_ficticio <= 19.5)
						{
							flag_escrita_tela = 1;
							flag_estado_abastecimento = 1;
							zeraBotoes();
							TFT_Draw_Bitmap(105, 308, BANHO_PRONTO_WIDTH, BANHO_PRONTO_HEIGHT, banho_pronto_bitmap, YELLOW);
							TFT_Draw_Fill_Round_Rect(145, 310, 125, 25, 5, YELLOW);
							LCD_Font(158, 326, "Abastecendo", _Open_Sans_Bold_14, 1, VIVID_BLUE);
						}

						if (flag_botao_off)
						{
							flag_escrita_tela = 1;
							zeraBotoes();
							current_screen = 0;
						}
					}
				}

				if(flag_atualizar_tela)
				{
					TFT_Draw_Fill_Rectangle(100, 180, 280, 105, VIVID_BLUE);
					sprintf(buffer, "%.1f", volume_ficticio);
					LCD_Font(100, 280, buffer, _Open_Sans_Bold_128, 1, WHITE);
					flag_atualizar_tela = 0;
				}
			}

			/********************* DRENAGEM *************************/
			else if(item_selected == 2)
			{
				if(volume_ficticio <= 0.5f)
				{
					if(flag_estado_drenagem) flag_estado_drenagem = 0;

					if(!flag_volume)
					{
						TFT_Draw_Bitmap(105, 308, BANHO_PRONTO_WIDTH, BANHO_PRONTO_HEIGHT, banho_pronto_bitmap, GREEN);
						TFT_Draw_Fill_Round_Rect(145, 310, 125, 25, 5, GREEN);
						LCD_Font(175, 326, "Drenado", _Open_Sans_Bold_14, 1, VIVID_BLUE);
						flag_escrita_tela = 1;
						flag_volume = 1;
					}
					else
					{
						if (flag_botao_off)
						{
							flag_escrita_tela = 1;
							flag_volume = 0;
							zeraBotoes();
							current_screen = 0;
						}
					}
				}
				else
				{
					if(flag_volume)
					{
						flag_volume = 0;
						flag_escrita_tela = 1;
						TFT_Draw_Fill_Round_Rect(105, 305, 165, 30, 5, VIVID_BLUE);
					}

					if(flag_estado_drenagem)
					{
						if (flag_botao_on)
						{
							flag_escrita_tela = 1;
							zeraBotoes();
							flag_estado_drenagem = 0;
							TFT_Draw_Bitmap(105, 308, BANHO_PRONTO_WIDTH, BANHO_PRONTO_HEIGHT, banho_pronto_bitmap, VIVID_BLUE);
							TFT_Draw_Fill_Round_Rect(145, 310, 125, 25, 5, VIVID_BLUE);
						}
					}
					else
					{
						if(flag_botao_on && volume_ficticio >= 0.5f)
						{
							flag_escrita_tela = 1;
							flag_estado_drenagem = 1;
							zeraBotoes();
							TFT_Draw_Bitmap(105, 308, BANHO_PRONTO_WIDTH, BANHO_PRONTO_HEIGHT, banho_pronto_bitmap, YELLOW);
							TFT_Draw_Fill_Round_Rect(145, 310, 125, 25, 5, YELLOW);
							LCD_Font(170, 326, "Drenando", _Open_Sans_Bold_14, 1, VIVID_BLUE);
						}

						if (flag_botao_off)
						{
							flag_escrita_tela = 1;
							zeraBotoes();
							current_screen = 0;
						}
					}
				}

				if(flag_atualizar_tela)
				{
					TFT_Draw_Fill_Rectangle(100, 180, 280, 105, VIVID_BLUE);
					sprintf(buffer, "%.1f", volume_ficticio);
					LCD_Font(100, 280, buffer, _Open_Sans_Bold_128, 1, WHITE);
					flag_atualizar_tela = 0;
				}
			}

			/********************* CONFIGURAÇÃO *************************/
			else if(item_selected == 3)
			{
				if (flag_botao_off)
				{
					flag_escrita_tela = 1;
					zeraBotoes();
					current_screen = 0;
				}
			}

	  }

		header_fotter();

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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

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
			if(contador_Printar_Numero < 5)	contador_Printar_Numero++;
			else
			{
				flag_atualizar_tela = 1;
				contador_Printar_Numero = 0;
				HAL_ADC_Start(&hadc1);
				uint16_t adc_value = HAL_ADC_GetValue(&hadc1);
				volume_ficticio = (float)adc_value * 20.0f / 4095.0f;
				HAL_ADC_Stop(&hadc1);
			}

		if(flag_iniciar_banho)
		{
			if(contador_Iniciar >= COUNT_LIM)
			{
				contador_Iniciar = 0;
				flag_iniciar_banho = 0;
				flag_escrita_tela = 1;
			}
			else
			{
				contador_Iniciar++;
			}

		}

		if(flag_desligar_banho)
		{
			if(contador_Desligar >= COUNT_LIM)
			{
				flag_estado_banho = 0;
				contador_Desligar = 0;
				ultimo_contador_Desligar = 0;
				flag_desligar_banho = 0;
				TFT_Draw_Fill_Rectangle(0, 81, 800, 317, VIVID_BLUE);
				starter_Screen();
				flag_escrita_tela = 1;
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
