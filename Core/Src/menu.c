#include "menu.h"
#include <stdio.h>
#include "liquidcrystal_i2c.h"

volatile uint16_t adc_Value = 0;		// Valor cru do adc_Value
volatile uint16_t setpoint = 40;

volatile uint8_t flag_pop_up;
char message_pop_up[64];
volatile uint32_t count_pop_up;
volatile uint8_t pop_up_showed = 0;



volatile uint8_t flag_button_up;        // Flag do botão "up"
volatile uint8_t flag_button_down;      // Flag do botão "down"
volatile uint8_t flag_button_enter;     // Flag do botão "enter"

volatile uint8_t flag_timer_int; 		// Flag do timer TIM4

// Definições das variáveis
volatile uint8_t current_screen = 1;    // Tela atual do menu
volatile uint8_t old_screen = 0;        // Tela anteriormente exibida
volatile int8_t cursor = 0;             // Posição atual do cursor
volatile int16_t old_cursor = 0;        // Posição anterior do cursor
volatile uint8_t window_start = 0;      // Índice do primeiro item exibido na janela
volatile uint8_t menu_needs_update = 1; // Flag que indica se o menu precisa ser atualizado

volatile uint32_t lastPressUp = 0;
volatile uint32_t lastPressDown = 0;
volatile uint32_t lastPressEnter = 0;

// Criação dos caracteres – usamos os índices 2,3,5 e 6 (pode manter o 4, se desejar)
void Menu_CreateChars(void) {
    HD44780_CreateSpecialChar(2, arrow_right);      // Cursor (seta)
    HD44780_CreateSpecialChar(3, scroll_line);        // Fundo do scroll
    HD44780_CreateSpecialChar(5, scroll_block_high);  // Bloco superior (alta)
    HD44780_CreateSpecialChar(6, scroll_block_low);   // Bloco inferior (baixa)
}

/*
   Função responsável por printar os nomes e indices das opções, chamada quando o usuário tentar
   acessar uma opção que ainda não está visível na tela, ou quando a tela é alterada.
   Atualiza window_start conforme o cursor atual, se alcançar o limite superior ou inferior é feito
   o wrap circular.
*/
void Menu_Draw(void) {

    // Se o cursor antigo for o último elemento da lista e o cursor atual for igual a zero = a lista volta para o topo.
    if(old_cursor == NUM_ITEMS - 1 && cursor == 0) {
        window_start = 0;

    // Se o cursor antigo for zero e o cursor atual for o último elemento da lista = a lista vai para o final.
    } else if(old_cursor == 0 && cursor == NUM_ITEMS - 1) {
        window_start = NUM_ITEMS - WINDOW_SIZE;
    
    // Se o cursor ultrapassar o limite da janela para cima, atualiza a posição da janela para a posição do cursor.
    } else if(cursor < window_start) {
        window_start = cursor;
    
    // Se o cursor ultrapassar o limite da janela para baixo, atualiza a posição da janela para a posição do cursor.
    } else if(cursor >= window_start + WINDOW_SIZE) {
        window_start = cursor - WINDOW_SIZE + 1;
    }

    HD44780_Clear();    // Limpa o display

    char temp_str[MAX_ITEM_LENGTH + 10];        // String temporária para armazenar o texto
    for(uint8_t i = 0; i < WINDOW_SIZE; i++){
        int8_t item_index = window_start + i;   // Índice do item a ser impresso

        // Se o índice do item for menor que o número de itens, imprime o item
        if(item_index < NUM_ITEMS){
            sprintf(temp_str, "%d.%s", item_index + 1, menu_items[item_index]); // Formata o texto
            HD44780_SetCursor(2, i);    // Seta o cursor na coluna 2 e na linha i
            HD44780_PrintStr(temp_str); // Imprime o texto
        }
    }

    menu_needs_update = 0; // Menu redesenhado
}

/*
   Atualiza a barra de scroll com resolução para até 8 elementos.
   Cada linha do display passa a ter 2 “unidades virtuais”, totalizando WINDOW_SIZE*2.
   Visto que existem 2 possíveis caractéres para 4 linhas. É calculado a posição do cursor
   em unidades (0 a (WINDOW_SIZE*2)-1) e determina:
     – cell: a linha escolhida para representar aquele elemento.
     – fraction: Se será representado com a parte inferior (0) ou superior (1), de acordo com
        os dois possíveis caracteres disponíveis.
*/
void Menu_UpdateScrollbar(void) {

    // Total de unidades é o dobro de WINDOW_SIZE
    uint8_t total_res = WINDOW_SIZE * 2; // para WINDOW_SIZE = 4, total_res = 8
    
    // Calcula a posição (em unidades) do cursor, escalando de 0 a total_res-1
    uint16_t pos_units = (cursor * (total_res - 1)) / (NUM_ITEMS - 1);

    uint8_t cell = pos_units / 2;       // Linha escolhida para simbolizar a barra do scroll (0 a 3)
    uint8_t fraction = pos_units % 2;   // Quais dos dois caracteres foi escolhido = 0: “inferior”, 1: “superior”

    for(uint8_t i = 0; i < WINDOW_SIZE; i++){
        // Seta o curso na última coluna e na linha i
        HD44780_SetCursor(19, i);

        // Se o indice da linha for igual ao cell, imprime o caractere correspondente
        if(i == cell) {

            // Escolhe qual metade imprimir conforme o nível
            if(fraction == 0)
                HD44780_PrintSpecialChar(5); // imprime o bloco "baixo"
            else
                HD44780_PrintSpecialChar(6); // imprime o bloco "alto"

        // Se o indice não for igual ao cell, imprime a barra horizontal vazia
        } else {
            HD44780_PrintSpecialChar(3);
        }
    }
}

/*
   Atualiza apenas o cursor: Sobreescreve o anterior e desenha o novo sem apagar a tela.
   O cursor é impresso na coluna 0 na linha relativa (cursor - window_start)
*/
void Menu_UpdateCursor(void) {

    // Se ocorreu uma mudança no cursor, atualiza a posição do cursor.
    if(old_cursor != cursor){
        HD44780_SetCursor(0, old_cursor - window_start);
        HD44780_PrintStr(" ");  // Apaga o cursor antigo
        old_cursor = cursor;    // Atualiza a posição do cursor
    }

    // Atualiza a barra de rolagem
    Menu_UpdateScrollbar();

    // Imprime o cursor na nova posição
    HD44780_SetCursor(0, cursor - window_start);
    HD44780_PrintSpecialChar(2);
}

void Menu_CheckButtons(void){

	if((flag_button_up || flag_button_down) && flag_pop_up){
		flag_pop_up = 0;
		count_pop_up = 0;
		pop_up_showed = 1;
		old_screen = !old_screen;
	}

	if(flag_button_up  && !flag_pop_up)
	{
		if(!current_screen){
			// Por exemplo: atualiza a posição do cursor
			cursor--;
			if(cursor < 0)
				cursor = (NUM_ITEMS - 1);

		} else {
			setpoint++;
			if(setpoint > 45)
				setpoint = 45;
		}

        flag_button_up = 0;
	}
	else if(flag_button_down && !flag_pop_up)
	{
		if(!current_screen){
			// Por exemplo: atualiza a posição do cursor
			cursor++;
			if(cursor > (NUM_ITEMS - 1))
				cursor = 0;

		} else {
			setpoint--;
			if(setpoint < 25)
				setpoint = 25;
		}

        flag_button_down = 0;
	}
	else if(flag_button_enter)
	{
		current_screen = !current_screen;
		pop_up_showed = 0;
		HD44780_Clear();
        flag_button_enter = 0;
	}
}

/*
   Função que é chamada no loop principal para atualizar o menu.
   O menu só é redesenhado quando for necessário mostrar uma opção fora das 4 presentes na tela.
*/
void Menu_HomeScreen(void) {

    // Condição para redesenhar o menu:
    // - Mudança de tela
    // - Flag de atualização acionada (menu_needs_update)
    // - O cursor ultrapassou o limite da janela: se estiver no primeiro e o usuário pressionar cima
    //   ou no último e o usuário pressionar para baixo (wrap circular).
    if(old_screen != current_screen || menu_needs_update ||
       (cursor < window_start) || (cursor >= window_start + WINDOW_SIZE)) {
    	Menu_Draw();
        old_screen = current_screen;
    }

    // Função que atualiza o cursor
    Menu_UpdateCursor();
}

/* Função que armazena a lógica individual de cada tela específica do sistema */
void Menu_SpecificScreen(void){

    // Verifica se a variável de tela anterior é diferente da atual, indicando a necessidade de limpar a tela
	if(old_screen != current_screen){
		HD44780_Clear();                // Limpa a tela
		old_screen = current_screen;    // Atualiza a tela anterior
	}

	if(flag_pop_up){
		Menu_ShowPopup();

	// Se a opção selecionada for "Banho"
	} else if(cursor == 0){
		uint16_t temp;
		char buffer_Str[10];
		temp = (uint16_t)(((adc_Value * 20) / 4095) + 25);

		HD44780_SetCursor(0, 0);
		HD44780_PrintStr("Temperatura|SetPoint");

		sprintf(buffer_Str, "%d", temp);
		HD44780_SetCursor(2, 2);
		HD44780_PrintStr(" ");
		HD44780_PrintStr(buffer_Str);

		sprintf(buffer_Str, "%d", setpoint);
		HD44780_SetCursor(13, 2);
		HD44780_PrintStr(" ");
		HD44780_PrintStr(buffer_Str);

        // Se a temperatura real chegou ao setpoint, exibe o pop-up desejado
        if(temp == setpoint && !pop_up_showed){
            HD44780_Clear();
            flag_pop_up = 1;
            strcpy(message_pop_up, "Temperatura pronta!");

        }
                
		if(flag_timer_int){
//		HD44780_SetCursor(5, 2);
//		HD44780_PrintStr(buffer_Str);
		flag_timer_int = 0;
		}

    // Se a opção selecionada for "Abastecer"
	} else if(cursor == 1){

		HD44780_SetCursor(4, 1);
		HD44780_PrintStr("Abastecer");

    // Se a opção selecionada for "Ajustes"
	} else if(cursor == 2){
		HD44780_SetCursor(0, 0);
		HD44780_PrintStr("Ajustes");

    // Se a opção selecionada for "Manuntenção"
	} else if(cursor == 3){
		HD44780_SetCursor(0, 0);
		HD44780_PrintStr("Manuntencao");

    // Se a opção selecionada for "Conexão"
	} else if(cursor == 4){
		HD44780_SetCursor(0, 0);
		HD44780_PrintStr("Conexao");
	}

}

void Menu_ShowPopup()
{
    char line[21];
    char msg[19];
    char middle[21];

    // Desenha a borda superior na linha 0, coluna 0
    sprintf(line, "|------------------|");
    HD44780_SetCursor(0, 0);
    HD44780_PrintStr(line);

    // Prepara a mensagem com no máximo 18 caracteres
    strncpy(msg, message_pop_up, 18);
    msg[18] = '\0';

    // Desenha a linha do meio com a mensagem (linha 1, coluna 0)
    sprintf(middle, "|%-18s|", msg);
    HD44780_SetCursor(0, 1);
    HD44780_PrintStr(middle);

    HD44780_SetCursor(0, 2);
    HD44780_PrintStr("|");

    HD44780_SetCursor(19, 2);
    HD44780_PrintStr("|");

    // Desenha a borda inferior (linha 2, coluna 0)
    sprintf(line, "|------------------|");
    HD44780_SetCursor(0, 3);
    HD44780_PrintStr(line);
    
    if(count_pop_up < 150){
    	count_pop_up++;
    	HAL_Delay(1);
    	//printf("CONTADOR= %d\n", count_pop_up);

    } else {
    	flag_pop_up = 0;
    	count_pop_up = 0;
    	pop_up_showed = 1;
    	old_screen = !old_screen;
    }
}

/* Função que gerencia a lógica geral da interface do menu */
void Menu_Interface(void){

    // Verifica se a tela atual é a tela principal ou uma tela específica
	if(!current_screen)
		Menu_HomeScreen();
	else
		Menu_SpecificScreen();

    // Verifica as flags dos botões
	Menu_CheckButtons();

}
