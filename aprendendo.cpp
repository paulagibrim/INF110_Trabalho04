// Inclui o arquivo de cabeçalho da biblioteca Allegro 5
#include <allegro5/allegro.h>
// Inclui o cabeçalho do add-on para uso de imagens
#include <allegro5/allegro_image.h>
 
// Nossa conhecida função main...
int main(void)
{
    // Variável representando a *janela principal*
    ALLEGRO_DISPLAY *janela = NULL;
    
    // Variável representando a *imagem*
    ALLEGRO_BITMAP *imagem = NULL;

    // Inicializamos a biblioteca
    al_init();
    
    // Inicializa o add-on para utilização de imagens
    al_init_image_addon();
    
    // Criamos a nossa janela - dimensões de 500x550 px
    janela = al_create_display(640, 480);

    // Carrega a imagem
    imagem = al_load_bitmap("hu3.bmp");

 
    /*// Preenchemos a janela de branco
    al_clear_to_color(al_map_rgb(255, 255, 255));*/


    // Desenha a imagem na tela
    al_draw_bitmap(imagem, 0, 0, 0);

 
    // Mostra a janela na tela (atualiza a tela)
    al_flip_display();
 
    // Segura a execução por 3 segundos
    al_rest(3.0);
 
    // Finaliza a janela
    al_destroy_display(janela);
 
    return 0;
}