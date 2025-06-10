#ifndef MENU_H
#define MENU_H

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
#include "FreeSerif18pt7b.h"


#define	_Free_Serif_24		   &FreeSerif18pt7b
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

#define NUM_ITEMS  4           // Número total de itens no menu, também usado para o número de capturas de tela e códigos QR.
#define MAX_ITEM_LENGTH  20    // Comprimento máximo de caracteres permitido para cada nome de item.

#define NUM_ROWS 4
#define NUM_COLS 4
#define DEBOUNCE_THRESHOLD 5

char menu_items [NUM_ITEMS] [MAX_ITEM_LENGTH] = {  // array with item names
  {"Banho"},
  {"Abastecimento"},
  {"Drenagem"},
  {"Configuracao"}
 };

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

void starter_Screen (void)
{
	TFT_Clear_Screen(VIVID_BLUE);

//	TFT_Draw_Round_Rect(40, 420, 140, 40, 10, 1, WHITE);
//	TFT_Draw_Round_Rect(230, 420, 140, 40, 10, 1, WHITE);
//	TFT_Draw_Round_Rect(420, 420, 140, 40, 10, 1, WHITE);
//	TFT_Draw_Round_Rect(620, 420, 140, 40, 10, 1, WHITE);

	TFT_Draw_HLine(30, 45, 740, 3, WHITE);
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
}

void atualiza_Cursor (void)
{
	// Inserir o Cursor na primeira iteração
	if(ultimo_item_selecionado == -1)
	{
		TFT_Draw_Fill_Round_Rect(31, 91, 738, 63, 6, WHITE);
		LCD_Font(140, 130, menu_items[0], _Open_Sans_Bold_24, 1, VIVID_BLUE);
		ultimo_item_selecionado = 0;

	}

	// Inserir o cursor após a troca de tela
	if(ultimo_estado_menu != current_screen)
	{
		ultimo_estado_menu = current_screen;
		TFT_Draw_Fill_Round_Rect(31, (item_selected * (65 + 10)) + 91, 738, 63, 8, WHITE);
		LCD_Font(140, (item_selected * (65 + 10)) + 131, menu_items[item_selected], _Open_Sans_Bold_24, 1, VIVID_BLUE);
	}

	// Verifica se a variável item_selecionado mudou = a posição do cursor deve mudar para a nova posição
	if (ultimo_item_selecionado != item_selected)
	{
		// Escreve os antigos cursores com branco para não precisar apagar a tela totalmente
		TFT_Draw_Fill_Round_Rect(31, (ultimo_item_selecionado * (65 + 10)) + 91, 738, 63, 8, VIVID_BLUE);
		LCD_Font(140, (ultimo_item_selecionado * (65 + 10)) + 131, menu_items[ultimo_item_selecionado], _Open_Sans_Bold_24, 1, WHITE);

		ultimo_item_selecionado = item_selected; // Muda o item selecionado

		TFT_Draw_Fill_Round_Rect(31, (item_selected * (65 + 10)) + 91, 738, 63, 8, WHITE);
		LCD_Font(140, (item_selected * (65 + 10)) + 131, menu_items[item_selected], _Open_Sans_Bold_24, 1, VIVID_BLUE);
	}
}

void cabecalho(char *titulo, int x)
{
	LCD_Font(x, 50, titulo, _Open_Sans_Bold_48, 1, BLACK);
	LCD_Font(x, 50, titulo, _Open_Sans_Bold_48, 1, BLACK);
	TFT_Draw_HLine(5, 100, 795, 2, BLACK);
}

void header (void)
{
	TFT_Draw_Fill_Rectangle(30, 0, 600, 35, VIVID_BLUE);

	if(current_screen == 0)
		LCD_Font(33, 30, "Menu", _Open_Sans_Bold_18, 1, WHITE);
	else
	{
		if(item_selected == 0)	LCD_Font(33, 30, "Banho", _Open_Sans_Bold_18, 1, WHITE);
		else if(item_selected == 1)	LCD_Font(33, 30, "Abastecimento", _Open_Sans_Bold_18, 1, WHITE);
		else if(item_selected == 2)	LCD_Font(33, 30, "Drenagem", _Open_Sans_Bold_18, 1, WHITE);
		else if(item_selected == 3)	LCD_Font(33, 30, "Configuracao", _Open_Sans_Bold_18, 1, WHITE);
	}
}

void fotter (void)
{
	if(current_screen == 0)
	{
		TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, VIVID_BLUE);
		TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, WHITE);
		TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, WHITE);
		TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

		TFT_Draw_Fill_Triangle_Simple(285, 427, 30, 18, 1, VIVID_BLUE);
		TFT_Draw_Fill_Triangle_Inverted_Simple(475, 432, 30, 18, 1, VIVID_BLUE);
		LCD_Font(660, 446, "Entrar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
	}
	else
	{

		//if(flag_escrita_tela){}
		if(item_selected == 0)
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
		else if(item_selected == 1)
		{
			TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
			TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, VIVID_BLUE);
			TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, VIVID_BLUE);
			TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

			LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
			LCD_Font(655, 446, "Iniciar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
		}
		else if(item_selected == 2)
		{
			TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
			TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, VIVID_BLUE);
			TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, VIVID_BLUE);
			TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

			LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
			LCD_Font(655, 446, "Iniciar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
		}
		else if(item_selected == 3)
		{
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
