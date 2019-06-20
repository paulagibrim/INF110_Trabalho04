//***************************QUARTO TRABALHO DE INF110****************************//
/*
    TRABALHO ELABORADO PELOS ALUNOS DA DISCIPLINA INF 110 DA UNIVERSIDADE FEDERAL DE
    VIÇOSA EM 2019/I.

    GRUPO:  PAULA GIBRIM - ES90366
            RENAN LOPES  - ES97370
            GABRIEL XXXX - ESXXXXXX

    MODIFICAÇÃO: 12/06/2019
*/

#include <iostream>
using namespace std;

// Inclui o arquivo de cabeçalho da biblioteca Allegro 5
#include <allegro5/allegro.h>
 
// Função main
int main(void)
{
    // Variável representando a janela principal
    ALLEGRO_DISPLAY *window = NULL;
 
    // Inicializamos a biblioteca
    al_init();
 
    // Criamos a nossa janela - dimensões de 640x480 px
    window = al_create_display(640, 480);
 
    // Preenchemos a janela de branco
    al_clear_to_color(al_map_rgb(255, 255, 255));
 
    // Atualiza a tela
    al_flip_display();
 
    // Segura a execução por 10 segundos
    al_rest(10.0);
 
    // Finaliza a janela
    al_destroy_display(window);
 
    return 0;
}
