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

int16_t flag_volume = 0;
int16_t contador_Printar_Numero = 0;
int16_t flag_estado_abastecimento = 0;
int16_t flag_estado_drenagem = 0;

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
volatile uint8_t flag_atualizar_tela = 1;

float volume_ficticio = 0.0f;
float ultimo_volume_ficticio = 0.0f;
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

/* Função responsável por printar o esquemático de cada tela, tudo aquilo de mais fundamental que uma tela tem e,
 * muito provavelmente terá durante todo o funcionamento da tela. Função garante que seja chamado apenas uma vez
 * toda a escrita da tela, objetos que variam na tela são modificados separadamente em outros lugares */
void starter_Screen (void)
{
	/* Verifica se é a primeira vez que está rodando o código com a verificação ultimo_estado_menu = -1, onde deve
	 * limpar a tela e escrever uma linha horizontal separando o título do corpo da tela */
	if(ultimo_estado_menu == -1)
	{
		TFT_Clear_Screen(VIVID_BLUE);
		TFT_Draw_HLine(30, 45, 740, 3, WHITE);
	}

	/* Verifica se a tela atual é a tela específica de cada opção separadamente */
	if(current_screen == 1)
	{
		/* Verifica se o item selecionado é o item de banho, com isso deve imprimir o SetPoint na parte central da
		 * tela e todos os itens que constituem a parte do banho, esses itens se preciso serão modificados apenas na
		 * opção do banho em si */
		if(item_selected == 0)
		{
			// Printar SetPoint grande na tela
			sprintf(buffer, "%d", temp);
			LCD_Font(160, 280, buffer,_Open_Sans_Bold_128, 1, WHITE);
			LCD_Font(320, 225, "C",_Open_Sans_Bold_48, 1, WHITE);
			TFT_Draw_Circle(315, 185, 4, 0, 1, WHITE);

			// Printar toda a parte de volume na tela
			LCD_Font(705, 90, "Volume",_Open_Sans_Bold_16, 1, GRAYISH_BLUE);
			LCD_Font(660, 115, "Reservatorio",_Open_Sans_Bold_16, 1, GRAYISH_BLUE);
			sprintf(buffer, "%.1f", volume_ficticio);
			LCD_Font(600, 155, buffer,_Open_Sans_Bold_28, 1, WHITE);
			LCD_Font(670, 155, "| 20.0",_Open_Sans_Bold_28, 1, GRAYISH_BLUE);
			LCD_Font(760, 155, "L",_Open_Sans_Bold_16, 1, WHITE);

			if(volume_ficticio >= 10.0f)
			{
				TFT_Draw_Fill_Round_Rect(620, 175, 150, 25, 5, GREEN);
				LCD_Font(658, 191, "nivel bom",_Open_Sans_Bold_14, 1, VIVID_BLUE);
			}
			else if(volume_ficticio > 5.0f && volume_ficticio < 10.0f)
			{
				TFT_Draw_Fill_Round_Rect(620, 175, 150, 25, 5, YELLOW);
				LCD_Font(655, 191, "nivel baixo",_Open_Sans_Bold_14, 1, VIVID_BLUE);
			}
			else
			{
				TFT_Draw_Fill_Round_Rect(620, 175, 150, 25, 5, RED);
				LCD_Font(630, 191, "nivel insuficiente",_Open_Sans_Bold_14, 1, VIVID_BLUE);
			}

			// Printar todos os 3 ícones desativados no início da tela
			TFT_Draw_Bitmap(595, 237, BANHO_PRONTO_WIDTH, BANHO_PRONTO_HEIGHT, banho_pronto_bitmap, GRAYISH_BLUE);
			LCD_Font(635, 257, "Banho Pronto",_Open_Sans_Bold_16, 1, GRAYISH_BLUE);
			sprintf(buffer, "%d|25", temp);
			LCD_Font(551, 310, buffer, _Open_Sans_Bold_24, 1, GRAYISH_BLUE);
			LCD_Font(631, 307, "c", _Open_Sans_Bold_18, 1, GRAYISH_BLUE);
			TFT_Draw_Circle(629, 293, 2, 0, 1, GRAYISH_BLUE);
			LCD_Font(661, 307, "Aquecendo",_Open_Sans_Bold_16, 1, GRAYISH_BLUE);
			TFT_Draw_Bitmap(603, 337, RECIRCULACAO_WIDTH, RECIRCULACAO_HEIGHT, recirculacao_bitmap, GRAYISH_BLUE);
			LCD_Font(643, 357, "Recirculando",_Open_Sans_Bold_16, 1, GRAYISH_BLUE);
		}

		/* Verifica se o item selecionado é o item de abastecimento, com isso deve imprimir o volume atual e o
		 * limite do reservatório do SmartBath, imprimindo um rótulo da opção */
		else if(item_selected == 1)
		{
			LCD_Font(100, 160, "Volume Reservatorio (L)", _Open_Sans_Bold_20, 1, WHITE);
			sprintf(buffer, "%.1f", volume_ficticio);
			LCD_Font(100, 280, buffer, _Open_Sans_Bold_128, 1, WHITE);
			TFT_Draw_VLine(400, 180, 110, 2, WHITE);
			LCD_Font(440, 280, "20.0", _Open_Sans_Bold_128, 1, GRAYISH_BLUE);
		}

		else if(item_selected == 2)
		{
			LCD_Font(100, 160, "Volume Reservatorio (L)", _Open_Sans_Bold_20, 1, WHITE);
			sprintf(buffer, "%.1f", volume_ficticio);
			LCD_Font(100, 280, buffer, _Open_Sans_Bold_128, 1, WHITE);
			TFT_Draw_VLine(400, 180, 110, 2, WHITE);
			LCD_Font(440, 280, "20.0", _Open_Sans_Bold_128, 1, GRAYISH_BLUE);
		}
	}
}

/* Função responsável por imprimir o nome das opções na tela e todos os ícones responsáveis pela
 * visualização do usuário. Função só será chamada no acionamento do sistema e quando ocorrer uma
 * troca de uma tela específica para o menu */
void home_screen(void) {

	// Desenha os 4 retângulos das opções
	TFT_Draw_Round_Rect(30, 90, 740, 65, 10, 1, WHITE);
	TFT_Draw_Round_Rect(30, 165, 740, 65, 10, 1, WHITE);
	TFT_Draw_Round_Rect(30, 240, 740, 65, 10, 1, WHITE);
	TFT_Draw_Round_Rect(30, 315, 740, 65, 10, 1, WHITE);

	// Escreve o nome dos 4 modos de operação
	LCD_Font(140, 127, menu_items[0], _Open_Sans_Bold_24, 1, WHITE);
	LCD_Font(140, 202, menu_items[1], _Open_Sans_Bold_24, 1, WHITE);
	LCD_Font(140, 277, menu_items[2], _Open_Sans_Bold_24, 1, WHITE);
	LCD_Font(140, 352, menu_items[3], _Open_Sans_Bold_24, 1, WHITE);

	// Desenha os 4 ícones das opções na tela
	TFT_Draw_Bitmap(70, 105, BANHO_BITMAP_WIDTH, BANHO_BITMAP_HEIGHT, banho_bitmap, WHITE);
	TFT_Draw_Bitmap(70, 180, ABASTECIMENTO_BITMAP_WIDTH, ABASTECIMENTO_BITMAP_HEIGHT, abastecimento_bitmap, WHITE);
	TFT_Draw_Bitmap(70, 255, DRENAGEM_BITMAP_WIDTH, DRENAGEM_BITMAP_HEIGHT, drenagem_bitmap, WHITE);
	TFT_Draw_Bitmap(70, 330, CONFIGURACAO_BITMAP_WIDTH, CONFIGURACAO_BITMAP_HEIGHT, configuracao_bitmap, WHITE);
}

/* Função responsável por atualizar o cursor. Só será chamado quando o sistema estiver no menu da solução,
 * fica constantemente comparando o cursor atual com o último, quando houver uma diferença entre os dois significa
 * que o usuário pressionou o botão que indica o movimento do cursor. Com isso, o cursor antigo deve ser apagado
 * e o novo cursor deve ser impresso na tela */
void atualiza_Cursor (void)
{
	/* Se o ultimo_item_selecionado = -1 quer dizer que é a primeira vez que o sistema está passando por essa
	 * função. Com isso, é necessário que o cursor seja impresso na tela*/
	if(ultimo_item_selecionado == -1)
	{
		TFT_Draw_Fill_Round_Rect(31, 91, 738, 63, 6, WHITE);
		LCD_Font(140, 130, menu_items[0], _Open_Sans_Bold_24, 1, VIVID_BLUE);
		TFT_Draw_Bitmap(70, 105, BANHO_BITMAP_WIDTH, BANHO_BITMAP_HEIGHT, banho_bitmap, VIVID_BLUE);
		ultimo_item_selecionado = 0;

	}

	/* Se ocorreu uma troca de tela é necessário que o cursor seja desenhado exatamente na função que ele estava
	 * anteriormente. Além disso a variável ultimo_estado_menu recebe a tela atual para garantir que essa condição
	 * seja comprida apenas uma vez. */
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

/* Função responsável por atualizar o cabeçalho e rodapé da tela. Esses 2 fatores irão atualizar o tempo todo.
 * Já que irão atualizar o tempo inteiro, foi necessário jogá-los no loop principal e usar um fator bloqueante,
 * uma flag que garantisse que o sistema entrasse apenas uma vez para atualizar o que for necessário quando
 * necessário */
void header_fotter (void)
{

	/* Verifica se a flag de escrita na tela foi setada em alguma parte do código, se sim, irá ramificar qual
	 * parte do código que solicitou e irá imprimir o que é necessário no cabeçalho e rodapé*/
	if(flag_escrita_tela)
	{

		/* Verifica se a tela atual é o menu. Deve apagar o cabeçalho, escrever menu e as opções necessárias no rodapé*/
		if(current_screen == 0)
		{
			// Apaga o cabeçalho e escreve "menu"
			TFT_Draw_Fill_Rectangle(30, 0, 600, 42, VIVID_BLUE);
			LCD_Font(33, 30, "Menu", _Open_Sans_Bold_24, 1, WHITE);

			// Imprime as três opções da direita com branco
			TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, VIVID_BLUE);
			TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, WHITE);
			TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, WHITE);
			TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

			// Desenha dois triangulos referentes ao deslocamento no menu e um indicativo da opção Entrar
			TFT_Draw_Fill_Triangle_Simple(285, 427, 30, 18, 1, VIVID_BLUE);
			TFT_Draw_Fill_Triangle_Inverted_Simple(475, 432, 30, 18, 1, VIVID_BLUE);
			LCD_Font(660, 446, "Entrar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
		}
		else // current_screen == 1
		{
			/* Verifica se a opção atual é banho, se sim, irá identificar em qual etapa do banho o sistema está
			 * e após isso imprimir na tela as funções disponíveis no rodapé*/
			if(item_selected == 0)
			{

				// Limpa o cabeçalho
				TFT_Draw_Fill_Rectangle(30, 0, 600, 42, VIVID_BLUE);

				/* Verifica se o banho está em processo de desligamento */
				if(flag_desligar_banho)
				{
					// Imprime no cabeçalho o nome da opção
					LCD_Font(33, 30, "Banho / Finalizando", _Open_Sans_Bold_24, 1, WHITE);

					// Todos os botões "desativados" por não conseguirem bloquear o processo de desligamento
					TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, VIVID_BLUE);
					TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, GRAYISH_BLUE);
					TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, GRAYISH_BLUE);
					TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, GRAYISH_BLUE);

					// Impressão dos sinais de subir, descer e parar banho, todos desativados indicando o desligamento
					TFT_Draw_Fill_Triangle_Simple(285, 427, 30, 18, 1, VIVID_BLUE);
					TFT_Draw_Fill_Triangle_Inverted_Simple(475, 432, 30, 18, 1, VIVID_BLUE);
					LCD_Font(665, 446, "Parar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
				}

				/* Se o banho não está em desligamento então pode imprimir no cabeçalho a mensagem de Banho */
				else
				{

					// Cabeçalho
					LCD_Font(33, 30, "Banho / Ajuste a Temperatura do Banho", _Open_Sans_Bold_24, 1, WHITE);

					/* Verifica se o banho está em andamento ou se o banho está iniciando */
					if(flag_estado_banho || flag_iniciar_banho)
					{
						// Imprime os 4 botões, com os 3 da direita possuindo funcionalidades
						TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, VIVID_BLUE);
						TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

						// Aumentar e diminuir temperatura e ao invés de "iniciar" agora possui "parar" indicando o banho
						TFT_Draw_Fill_Triangle_Simple(285, 427, 30, 18, 1, VIVID_BLUE);
						TFT_Draw_Fill_Triangle_Inverted_Simple(475, 432, 30, 18, 1, VIVID_BLUE);
						LCD_Font(665, 446, "Parar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
					}

					/* Se o banho não estiver em andamento, ao invés da opção de "parar", o rodapé deve mostrar a opção
					 * de iniciar o banho ou até de voltar para o menu, já que o processo ainda não se iniciou.*/
					else
					{
						// Todos os botões do rodapé são setados com branco
						TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);

						// São impressos o que cada um dos botões faz individualmente
						LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
						TFT_Draw_Fill_Triangle_Simple(285, 427, 30, 18, 1, VIVID_BLUE);
						TFT_Draw_Fill_Triangle_Inverted_Simple(475, 432, 30, 18, 1, VIVID_BLUE);
						LCD_Font(660, 446, "Iniciar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
					}
				}
			}

			/* Verifica se a opção atual é abastecimento, se sim, irá identificar em qual etapa do abastecimento o sistema está
			 * e após isso imprimir na tela as funções disponíveis no rodapé*/
			else if(item_selected == 1)
			{
				// Limpar o cabeçalho e imprimir o rótulo da opçãol
				TFT_Draw_Fill_Rectangle(30, 0, 600, 35, VIVID_BLUE);
				LCD_Font(33, 30, "Abastecimento", _Open_Sans_Bold_24, 1, WHITE);

				/* Verifica se a flag_volume = 1(Reservatório cheio), se sim deve impedir o abastecimento e permitir
				 * apenas a volta para o menu */
				if(flag_volume)
				{
					TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
					TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, VIVID_BLUE);
					TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, VIVID_BLUE);
					TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, GRAYISH_BLUE);
					LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
					LCD_Font(660, 446, "Iniciar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
				}

				/* Se não está cheio, então devo verificar se está abastecendo ou não. Para saber se imprimirei
				 * a mensagem de parar ou iniciar, respectivamente */
				else
				{
					/* Se está abastecendo no momento */
					if(flag_estado_abastecimento)
					{
						TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, GRAYISH_BLUE);
						TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, VIVID_BLUE);
						TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, VIVID_BLUE);
						TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);
						LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
						LCD_Font(665, 446, "Parar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
					}

					/* Se está apenas na tela do abastecimento sem ter iniciado o processo */
					else
					{
						TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, VIVID_BLUE);
						TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, VIVID_BLUE);
						TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);
						LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
						LCD_Font(660, 446, "Iniciar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
					}
				}
			}

			/* Verifica se essa é a opção da Drenagem */
			else if(item_selected == 2)
			{
				TFT_Draw_Fill_Rectangle(30, 0, 600, 35, VIVID_BLUE);
				LCD_Font(33, 30, "Drenagem", _Open_Sans_Bold_24, 1, WHITE);

				/* Verifica se a flag_volume = 1(Reservatório vazio), se sim deve impedir a drenagem e permitir
				 * apenas a volta para o menu */
				if(flag_volume)
				{
					TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
					TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, VIVID_BLUE);
					TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, VIVID_BLUE);
					TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, GRAYISH_BLUE);
					LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
					LCD_Font(660, 446, "Iniciar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
				}

				/* Se não está vazio, então devo verificar se está drenando ou não. Para saber se imprimirei
				 * a mensagem de parar ou iniciar, respectivamente */
				else
				{
					/* Se está drenando no momento */
					if(flag_estado_drenagem)
					{
						TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, GRAYISH_BLUE);
						TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, VIVID_BLUE);
						TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, VIVID_BLUE);
						TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);
						LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
						LCD_Font(665, 446, "Parar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
					}

					/* Se está apenas na tela da drenagem sem ter iniciado o processo */
					else
					{
						TFT_Draw_Fill_Round_Rect(41, 421, 138, 38, 8, WHITE);
						TFT_Draw_Fill_Round_Rect(231, 421, 138, 38, 8, VIVID_BLUE);
						TFT_Draw_Fill_Round_Rect(421, 421, 138, 38, 8, VIVID_BLUE);
						TFT_Draw_Fill_Round_Rect(621, 421, 138, 38, 8, WHITE);
						LCD_Font(80, 446, "Voltar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
						LCD_Font(660, 446, "Iniciar", _Open_Sans_Bold_18, 1, VIVID_BLUE);
					}
				}
			}

			/* Verifica se essa é a opção da Configuração */
			else if(item_selected == 3)
			{
				TFT_Draw_Fill_Rectangle(30, 0, 600, 35, VIVID_BLUE);
				LCD_Font(33, 30, "Configuracao", _Open_Sans_Bold_24, 1, WHITE);

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

void zeraBotoes(void) {flag_botao_on = flag_botao_off = flag_botao_up = flag_botao_down = 0;}
#endif /*MENU_H*/
