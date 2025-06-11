#ifndef MENU_H
#define MENU_H

#include <string.h>
#include <stdio.h>
#include "display_ssd1963.h"
#include "008_Open_Sans_Bold.h"
#include "009_Open_Sans_Bold.h"
#include "010_Open_Sans_Bold.h"
#include "012_Open_Sans_Bold.h"
#include "014_Open_Sans_Bold.h"
#include "016_Open_Sans_Bold.h"
#include "018_Open_Sans_Bold.h"
#include "020_Open_Sans_Bold.h"
#include "022_Open_Sans_Bold.h"
#include "024_Open_Sans_Bold.h"
#include "026_Open_Sans_Bold.h"
#include "028_Open_Sans_Bold.h"
#include "036_Open_Sans_Bold.h"
#include "048_Open_Sans_Bold.h"
#include "072_Open_Sans_Bold.h"
#include "096_Open_Sans_Bold.h"
#include "112_Open_Sans_Bold.h"
#include "128_Open_Sans_Bold.h"
#include "FreeSerif12pt7b.h"
#include "png.h"


#define	_Free_Serif_12		   &FreeSerif12pt7b
#define _Open_Sans_Bold_8      &Open_Sans_Bold_8
#define _Open_Sans_Bold_9      &Open_Sans_Bold_9
#define _Open_Sans_Bold_10     &Open_Sans_Bold_10
#define _Open_Sans_Bold_11     &Open_Sans_Bold_11
#define _Open_Sans_Bold_12      &Open_Sans_Bold_12
#define _Open_Sans_Bold_14      &Open_Sans_Bold_14
#define _Open_Sans_Bold_16      &Open_Sans_Bold_16
#define _Open_Sans_Bold_18      &Open_Sans_Bold_18
#define _Open_Sans_Bold_20      &Open_Sans_Bold_20
#define _Open_Sans_Bold_22      &Open_Sans_Bold_22
#define _Open_Sans_Bold_24      &Open_Sans_Bold_24
#define _Open_Sans_Bold_26      &Open_Sans_Bold_26
#define _Open_Sans_Bold_28      &Open_Sans_Bold_28
#define _Open_Sans_Bold_36      &Open_Sans_Bold_36
#define _Open_Sans_Bold_48      &Open_Sans_Bold_48
#define _Open_Sans_Bold_72      &Open_Sans_Bold_72
#define _Open_Sans_Bold_96      &Open_Sans_Bold_96
#define _Open_Sans_Bold_112      &Open_Sans_Bold_112
#define _Open_Sans_Bold_128      &Open_Sans_Bold_128

int16_t current_screen;
int16_t item_selected = 0;         // Item atual selecionado no menu.
int16_t item_sel_previous;         // Índice do item anterior, usado para exibir o item antes do selecionado.
int16_t item_sel_next;             // Índice do próximo item, usado para exibir o item após o selecionado.
int16_t current_screen = 0;        // Indica o índice da tela atual sendo exibida.
int16_t cursor = 0;                // Posição do cursor no menu para navegação.
int16_t button_enter_clicked = 0;  // só executa ação quando o botão ENTER é clicado, e espera até outro clique
int16_t up_clicked = 0;            // só executa ação quando o botão é clicado, e espera até outro clique
int16_t down_clicked = 0;          // mesmo que acima

int16_t ultimo_estado_menu = -1;
int16_t ultimo_item_selecionado = -1;

int16_t contador_Desligar = 0;
int16_t ultimo_contador_Desligar = 0;
int16_t contador_Iniciar = 0;
int16_t flag_iniciar_banho = 0;
int16_t flag_estado_banho = 0;
int16_t flag_desligar_banho = 0;
int16_t flag_resistencia_ativa = 0;

static const int8_t mask_keyboard[4][4] =
{
		{ 1, 2, 3, 10 },
		{ 4, 5, 6, 11 },
		{ 7, 8, 9, 12 },
		{ 14, 0, 15, 13}
};

static uint8_t    key_pressed[NUM_ROWS][NUM_COLS]   = {0};
static uint8_t debounce_counter[NUM_ROWS][NUM_COLS] = {0};

volatile uint8_t flag_botao_off = 0; 			// Flag para indicar que o botão de desligar o banho ta ativo
volatile uint8_t flag_botao_up = 0; 			// Flag para indicar que o botão UP foi presscionado
volatile uint8_t flag_botao_down = 0; 			// Flag para indicar que o botão DOWN foi presscionado
volatile uint8_t flag_botao_on = 0; 			// Flag para indicar que o botão ON foi presscionado

volatile uint8_t flag_escrita_tela = 1;

float volume_ficticio = 20.0f;
uint16_t temp = 36;
char buffer[36];

static const uint16_t *bitmap_icons[NUM_ITEMS] = {
  banho_bitmap,
  abastecimento_bitmap,
  drenagem_bitmap,
  configuracao_bitmap
};

static const uint16_t bitmap_w[NUM_ITEMS] = {
  BANHO_BITMAP_WIDTH,
  ABASTECIMENTO_BITMAP_WIDTH,
  DRENAGEM_BITMAP_WIDTH,
  CONFIGURACAO_BITMAP_WIDTH
};
static const uint16_t bitmap_h[NUM_ITEMS] = {
  BANHO_BITMAP_HEIGHT,
  ABASTECIMENTO_BITMAP_HEIGHT,
  DRENAGEM_BITMAP_HEIGHT,
  CONFIGURACAO_BITMAP_HEIGHT
};


void starter_Screen (void)
{
	if(ultimo_estado_menu == -1)
	{
		TFT_Clear_Screen(VIVID_BLUE);
		TFT_Draw_HLine(30, 45, 740, 3, WHITE);
	}

	if(current_screen == 1)
	{
		if(item_selected == 0)
		{
			sprintf(buffer, "%d", temp);
			LCD_Font(120, 280, buffer,_Open_Sans_Bold_128, 1, WHITE);
			LCD_Font(280, 225, "C",_Open_Sans_Bold_48, 1, WHITE);
			TFT_Draw_Circle(275, 185, 4, 0, 1, WHITE);

			TFT_Draw_Bitmap(690, 237, BANHO_PRONTO_WIDTH, BANHO_PRONTO_HEIGHT, banho_pronto_bitmap, GRAYISH_BLUE);
			TFT_Draw_Circle(750, 250, 10, 1, 1, GRAYISH_BLUE);
			LCD_Font(685, 310, "36", _Open_Sans_Bold_24, 1, GRAYISH_BLUE);
			TFT_Draw_Circle(750, 300, 10, 1, 1, GRAYISH_BLUE);
			TFT_Draw_Bitmap(690, 337, RECIRCULACAO_WIDTH, RECIRCULACAO_HEIGHT, recirculacao_bitmap, GRAYISH_BLUE);
			TFT_Draw_Circle(750, 350, 10, 1, 1, GRAYISH_BLUE);
		}
	}
}

void home_screen(void) {
	TFT_Draw_Round_Rect(30, 90, 740, 65, 10, 1, WHITE);
	TFT_Draw_Round_Rect(30, 165, 740, 65, 10, 1, WHITE);
	TFT_Draw_Round_Rect(30, 240, 740, 65, 10, 1, WHITE);
	TFT_Draw_Round_Rect(30, 315, 740, 65, 10, 1, WHITE);

	// Escreve os 4 modos nas suas devidas posições
	LCD_Font(140, 130, menu_items[0], _Open_Sans_Bold_24, 1, WHITE);
	LCD_Font(140, 205, menu_items[1], _Open_Sans_Bold_24, 1, WHITE);
	LCD_Font(140, 280, menu_items[2], _Open_Sans_Bold_24, 1, WHITE);
	LCD_Font(140, 355, menu_items[3], _Open_Sans_Bold_24, 1, WHITE);

	TFT_Draw_Bitmap(70, 105, BANHO_BITMAP_WIDTH, BANHO_BITMAP_HEIGHT, banho_bitmap, WHITE);
	TFT_Draw_Bitmap(70, 180, ABASTECIMENTO_BITMAP_WIDTH, ABASTECIMENTO_BITMAP_HEIGHT, abastecimento_bitmap, WHITE);
	TFT_Draw_Bitmap(70, 255, DRENAGEM_BITMAP_WIDTH, DRENAGEM_BITMAP_HEIGHT, drenagem_bitmap, WHITE);
	TFT_Draw_Bitmap(70, 330, CONFIGURACAO_BITMAP_WIDTH, CONFIGURACAO_BITMAP_HEIGHT, configuracao_bitmap, WHITE);
}

void atualiza_Cursor (void)
{
	// Inserir o Cursor na primeira iteração
	if(ultimo_item_selecionado == -1)
	{
		TFT_Draw_Fill_Round_Rect(31, 91, 738, 63, 6, WHITE);
		LCD_Font(140, 130, menu_items[0], _Open_Sans_Bold_24, 1, VIVID_BLUE);
		TFT_Draw_Bitmap(70, 105, BANHO_BITMAP_WIDTH, BANHO_BITMAP_HEIGHT, banho_bitmap, VIVID_BLUE);
		ultimo_item_selecionado = 0;

	}

	// Inserir o cursor após a troca de tela
	if(ultimo_estado_menu != current_screen && ultimo_estado_menu != 0)
	{
		ultimo_estado_menu = current_screen;
		TFT_Draw_Fill_Round_Rect(31, (item_selected * (65 + 10)) + 91, 738, 63, 8, WHITE);
		LCD_Font(140, (item_selected * (65 + 10)) + 131, menu_items[item_selected], _Open_Sans_Bold_24, 1, VIVID_BLUE);
		TFT_Draw_Bitmap(70, (item_selected * (65 + 10)) + 105, bitmap_w[item_selected], bitmap_h[item_selected], bitmap_icons[item_selected], VIVID_BLUE);
	}

	// Verifica se a variável item_selecionado mudou = a posição do cursor deve mudar para a nova posição
	if (ultimo_item_selecionado != item_selected)
	{
		uint16_t y_old = (ultimo_item_selecionado*(65+10)) + 110;
		uint16_t y_new = (item_selected*(65+10)) + 110;

		// Escreve os antigos cursores com branco para não precisar apagar a tela totalmente
		TFT_Draw_Fill_Round_Rect(31, y_old-19, 738, 63, 8, VIVID_BLUE);
		LCD_Font(140, y_old+21, menu_items[ultimo_item_selecionado], _Open_Sans_Bold_24, 1, WHITE);
		TFT_Draw_Bitmap(70, y_old, bitmap_w[ultimo_item_selecionado], bitmap_h[ultimo_item_selecionado], bitmap_icons[ultimo_item_selecionado], WHITE);

		ultimo_item_selecionado = item_selected; // Muda o item selecionado

		TFT_Draw_Fill_Round_Rect(31, y_new-19, 738, 63, 8, WHITE);
		LCD_Font(140, y_new+21, menu_items[item_selected], _Open_Sans_Bold_24, 1, VIVID_BLUE);
		TFT_Draw_Bitmap(70, y_new, bitmap_w[item_selected], bitmap_h[item_selected], bitmap_icons[item_selected], VIVID_BLUE);
	}
}

void cabecalho(char *titulo, int x)
{
	LCD_Font(x, 50, titulo, _Open_Sans_Bold_48, 1, BLACK);
	LCD_Font(x, 50, titulo, _Open_Sans_Bold_48, 1, BLACK);
	TFT_Draw_HLine(5, 100, 795, 2, BLACK);
}

void header_fotter (void)
{
	if(flag_escrita_tela)
	{
		if(current_screen == 0)
		{
			TFT_Draw_Fill_Rectangle(30, 0, 600, 35, VIVID_BLUE);
			LCD_Font(33, 30, "Menu", _Open_Sans_Bold_18, 1, WHITE);
			TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, VIVID_BLUE);
			TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, WHITE);
			TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, WHITE);
			TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

			TFT_Draw_Fill_Triangle_Simple(285, 427, 30, 18, 1, VIVID_BLUE);
			TFT_Draw_Fill_Triangle_Inverted_Simple(475, 432, 30, 18, 1, VIVID_BLUE);
			LCD_Font(660, 446, "Entrar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
		}
		else // current_screen == 1
		{
			if(item_selected == 0)
			{
				TFT_Draw_Fill_Rectangle(30, 0, 600, 35, VIVID_BLUE);

				if(flag_desligar_banho)
				{
					LCD_Font(33, 30, "Banho / Finalizando", _Open_Sans_Bold_18, 1, WHITE);

					TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, VIVID_BLUE);
					TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, GRAYISH_BLUE);
					TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, GRAYISH_BLUE);
					TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, GRAYISH_BLUE);

					TFT_Draw_Fill_Triangle_Simple(285, 427, 30, 18, 1, VIVID_BLUE);
					TFT_Draw_Fill_Triangle_Inverted_Simple(475, 432, 30, 18, 1, VIVID_BLUE);
					LCD_Font(665, 446, "Parar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
				}
				else
				{
					LCD_Font(33, 30, "Banho / Ajuste a Temperatura do Banho", _Open_Sans_Bold_18, 1, WHITE);

					if(flag_estado_banho || flag_iniciar_banho)
					{
						TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, VIVID_BLUE);
						TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

						TFT_Draw_Fill_Triangle_Simple(285, 427, 30, 18, 1, VIVID_BLUE);
						TFT_Draw_Fill_Triangle_Inverted_Simple(475, 432, 30, 18, 1, VIVID_BLUE);
						LCD_Font(665, 446, "Parar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
					}
					else
					{

						TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

						LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
						TFT_Draw_Fill_Triangle_Simple(285, 427, 30, 18, 1, VIVID_BLUE);
						TFT_Draw_Fill_Triangle_Inverted_Simple(475, 432, 30, 18, 1, VIVID_BLUE);
						LCD_Font(655, 446, "Iniciar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
					}
				}
			}
			else if(item_selected == 1)
			{
				TFT_Draw_Fill_Rectangle(30, 0, 600, 35, VIVID_BLUE);
				LCD_Font(33, 30, "Abastecimento", _Open_Sans_Bold_18, 1, WHITE);

				TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
				TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, VIVID_BLUE);
				TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, VIVID_BLUE);
				TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

				LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
				LCD_Font(655, 446, "Iniciar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
			}
			else if(item_selected == 2)
			{
				TFT_Draw_Fill_Rectangle(30, 0, 600, 35, VIVID_BLUE);
				LCD_Font(33, 30, "Drenagem", _Open_Sans_Bold_18, 1, WHITE);

				TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
				TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, VIVID_BLUE);
				TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, VIVID_BLUE);
				TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

				LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
				LCD_Font(655, 446, "Iniciar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
			}
			else if(item_selected == 3)
			{
				TFT_Draw_Fill_Rectangle(30, 0, 600, 35, VIVID_BLUE);
				LCD_Font(33, 30, "Configuracao", _Open_Sans_Bold_18, 1, WHITE);

				TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
				TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, WHITE);
				TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, WHITE);
				TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

				LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
				TFT_Draw_Fill_Triangle_Simple(285, 427, 30, 18, 1, VIVID_BLUE);
				TFT_Draw_Fill_Triangle_Inverted_Simple(475, 432, 30, 18, 1, VIVID_BLUE);
				LCD_Font(660, 446, "Entrar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
			}
		}

		flag_escrita_tela = 0;
	}
}

void cabecalhos(void)
{

	if (item_selected == 0)
		cabecalho("Banho", 110);
	else if (item_selected == 1)
		cabecalho("Abastecimento", 60);
	else if (item_selected == 2)
		cabecalho("Manutencao", 75);
	else if (item_selected == 3)
		cabecalho("Configuracao", 75);
}

int8_t scan_membrana(void)
{

		for(int col = 0; col < 4; col++ )
		{

			uint32_t botao_mask = (GPIOE->IDR & (0x0F << 2)) >> 2;
			uint8_t botao = (botao_mask >> col) & 1;


			if(botao == key_pressed[0][col])
			{
				debounce_counter[0][col] = 0;
			}else if(++debounce_counter[0][col] >= DEBOUNCE_THRESHOLD)
			{
				key_pressed[0][col] = botao;
				debounce_counter[0][col] = 0;

				if(botao)
				{
					return mask_keyboard[0][col];
				}
			}
	}

	return -1;
}

#endif /*MENU_H*/
