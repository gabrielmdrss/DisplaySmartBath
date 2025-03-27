/******************************************************************************
 * @file    menu.h
 * @brief   Arquivo de cabeçalho contendo definições e protótipos de funções para o menu.
 *
 * @authors Gabriel Domingos
 * @date    Março 2025
 * @version 1.0.0
 *
 * @note    Este arquivo inclui as definições necessárias, constantes e os
 *          protótipos de funções para manipulação do menu.
 *
 * @copyright (c) 2025 Assert IFPB. Todos os direitos reservados.
 ******************************************************************************/

 #ifndef MENU_H
 #define MENU_H
 
 #include "liquidcrystal_i2c.h"     // Biblioteca para manipulação de LCD via I2C.
 #include <string.h>
 
 // ------------------------------ Defines ------------------------------
 
 #define NUM_ITEMS       6            	// Número de itens do menu
 #define MAX_ITEM_LENGTH 16           	// Comprimento máximo de cada item (em caracteres)
 #define WINDOW_SIZE     4            	// Quantidade de itens exibidos na tela
 #define DEBOUNCE_DELAY  100	      	// Delay de debounce (em milissegundos)

 
 extern volatile uint16_t adc_Value;	// Valor cru do adc_Value
 extern volatile uint16_t setpoint;		// Valor escolhido de temperatura
 extern volatile uint8_t flag_pop_up;	// Flag de pop-up
 extern char message_pop_up[64];
 extern volatile uint32_t count_pop_up;
 extern volatile uint8_t pop_up_showed;

 // ------------------------ Variáveis dos Botões ------------------------
 
 /**
  * @brief Flags dos botões.
  *
  * Estas flags indicam quando os botões "up", "down" e "enter" são acionados.
  */
 extern volatile uint8_t flag_button_up;    // Flag do botão "up"
 extern volatile uint8_t flag_button_down;  // Flag do botão "down"
 extern volatile uint8_t flag_button_enter; // Flag do botão "enter"
 
 /**
  * @brief Flags do timer.
  *
  * Esta flag indica a flag de estouro do timer, que acontece a cada 1s.
  */
extern volatile uint8_t flag_timer_int; 	// Flag do timer TIM4

 /**
  * @brief Variáveis de controle dos botões.
  *
  * Estas variáveis armazenam os tempos dos últimos eventos de subida.
  */
 extern volatile uint32_t lastPressUp;
 extern volatile uint32_t lastPressDown;
 extern volatile uint32_t lastPressEnter;
 
 // --------------------- Variáveis de Controle do Menu ---------------------
 
 /**
  * @brief Variáveis de controle do menu.
  *
  * Estas variáveis armazenam o estado atual e anterior do menu, posição do cursor
  * e índice do primeiro item a ser exibido na janela do menu.
  */
 extern volatile uint8_t current_screen;    // Tela atual do menu
 extern volatile uint8_t old_screen;        // Tela anteriormente exibida
 extern volatile int8_t cursor;             // Posição atual do cursor
 extern volatile int16_t old_cursor;        // Posição anterior do cursor
 extern volatile uint8_t window_start;      // Índice do primeiro item exibido na janela
 extern volatile uint8_t menu_needs_update; // Flag que indica se o menu precisa ser atualizado
 
 // ----------------------- Caracteres Customizados -----------------------
 
 static const uint8_t arrow_right[8] = {
     0b00000,
     0b00000,
     0b00100,
     0b00010,
     0b11111,
     0b00010,
     0b00100,
     0b00000
 };  // Símbolo de seta para a direita utilizado no menu
 
 static const uint8_t scroll_line[8] = {
     0b00010,
     0b00010,
     0b00010,
     0b00010,
     0b00010,
     0b00010,
     0b00010,
     0b00010
 };  // Linha de rolagem vertical para a interface do menu
 
 // Dividindo o bloco inteiro em dois blocos para as metades da barra de rolagem:
 static const uint8_t scroll_block_high[8] = {
     0b00111, // Top preenchido
     0b00111,
     0b00010,
     0b00010,
     0b00010,
     0b00010,
     0b00010,
     0b00010,
 };  // Bloco superior da barra de rolagem (parcialmente preenchido)
 
 static const uint8_t scroll_block_low[8] = {
     0b00010,
     0b00010,
     0b00010,
     0b00010,
     0b00010,
     0b00111, // Base preenchida
     0b00111,
     0b00111
 };  // Bloco inferior da barra de rolagem (parcialmente preenchido)
 
 // --------------------------- Itens do Menu ---------------------------
 
 /**
  * @brief Itens do menu.
  *
  * Lista de itens que serão apresentados na interface do menu.
  */
 static const char menu_items[NUM_ITEMS][MAX_ITEM_LENGTH] = {
     "Banho",
     "Temperatura",
     "Volume",
     "Conexao",
     "Configuracao",
     "Calibracao"
 };

// -------------------------- Funções do Menu --------------------------
/**
 * @brief Cria os caracteres customizados para a interface do menu.
 *
 * Esta função registra caracteres especiais em índices específicos para:
 * - O cursor (seta) no índice 2.
 * - O fundo da linha de rolagem no índice 3.
 * - A parte superior da barra de rolagem no índice 5.
 * - A parte inferior da barra de rolagem no índice 6.
 */
void Menu_CreateChars(void);

/**
 * @brief Redesenha toda a interface do menu.
 *
 * Esta função atualiza o menu visível com base na posição atual do cursor.
 * Ela ajusta o índice inicial da janela quando ocorre wrap, limpa o LCD,
 * e imprime os itens do menu com seus respectivos números de índice.
 */
void Menu_Draw(void);

/**
 * @brief Atualiza a barra de rolagem vertical no display LCD.
 *
 * Esta função calcula a posição atual do cursor em unidades virtuais,
 * determina a célula e a fração correspondentes, e então atualiza cada linha
 * da área da barra de rolagem para refletir a posição do cursor.
 */
void Menu_UpdateScrollbar(void);

/**
 * @brief Atualiza a exibição do cursor no menu.
 *
 * Esta função apaga o cursor anteriormente desenhado, atualiza a barra de rolagem,
 * e então desenha o novo cursor na posição atual com base na janela visível.
 */
void Menu_UpdateCursor(void);

/**
 * @brief Verifica e processa as entradas dos botões para navegação no menu.
 *
 * Esta função avalia os estados das flags dos botões "up", "down" e "enter",
 * atualiza a posição do cursor ou alterna o estado da tela conforme necessário,
 * e então reseta as respectivas flags.
 */
void Menu_CheckButtons(void);

/**
 * @brief Atualiza a tela principal do menu.
 *
 * Esta função redesenha o menu se houver mudança de tela, se a flag de atualização
 * estiver ativa, ou se o cursor se mover para fora da janela visível. Em seguida,
 * atualiza a exibição do cursor.
 */
void Menu_HomeScreen(void);

/**
 * @brief Exibe uma tela específica com base na seleção atual do menu.
 *
 * Esta função limpa o display LCD quando ocorre mudança de estado da tela e,
 * em seguida, exibe o texto correspondente ao item selecionado em coordenadas predefinidas.
 */
void Menu_SpecificScreen(void);

void Menu_ShowPopup();

/**
 * @brief Gerencia a lógica geral da interface do menu.
 *
 * Esta função determina qual tela deve ser exibida com base no estado atual.
 * Ela chama a função de atualização da tela principal ou a de exibição de uma tela específica,
 * e processa as entradas dos botões para atualizar o menu conforme necessário.
 */
void Menu_Interface(void);

#endif /* MENU_H */
