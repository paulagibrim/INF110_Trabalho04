//***************************QUARTO TRABALHO DE INF110****************************//
/*
    TRABALHO ELABORADO PELOS ALUNOS DA DISCIPLINA INF 110 DA UNIVERSIDADE FEDERAL DE
    VIÇOSA EM 2019/I.

    GRUPO:  PAULA GIBRIM - ES90366
            RENAN LOPES  - ES97370
            GABRIEL XXXX - ESXXXXXX

    MODIFICAÇÃO: 20/06/2019

	ATENÇÃO EU, RENAN VOU REFAZER O MAPA ATUAL;



	CORES
	AZUL = al_map_rgb(15, 174, 191)
	VERDE = al_map_rgb(65, 166, 50)


*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <iostream>

using namespace std;

const float FPS = 10;
const int SCREEN_W = 500;
const int SCREEN_H = 550;

bool DEBUG_MODE = true;

enum MYKEYS{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER
};

//matriz definindo mapa do jogo: 1 representa paredes, 0 representa corredor

//ATENÇÃO! precisa atualizar a matriz

	// Numeros na Matriz
	// 0 = Vazio
	// 1 = Parede
	// 2 = Bolinha
	// 3 = Pipula
	// 4 = Portal

char MAPA[26][26] =
{
    "1111111111111111111111111",
    "1222222221111111222222221",
    "1211111121111111211111121",
    "1211111122222222211111121",
    "1222222221111111222232221",
    "1112111121111111211112111",
    "1222111122221222211112221",
    "1212111111121211111112121",
    "1212222111221221112222121",
    "1211112111211121112111121",
    "1211112122222222212111121",
    "1222112221112111222112221",
    "1112111121112111211112111",
    "1222222221112111222222221",
    "1211111121112111211111121",
    "1211122222222222222211121",
    "1322221111112111111222221",
    "1111121112222222111211111",
    "1111121112111112111211111",
    "1222222222222222222222221",
    "1211121111112111111211121",
    "1222221111112111111222221",
    "1211132222212122222211121",
    "1222221111222221111222221",
    "1111111111111111111111111",
};


ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *mapa = NULL;
ALLEGRO_BITMAP *pacman = NULL;
ALLEGRO_BITMAP *ball = NULL;
ALLEGRO_BITMAP *pipula = NULL;


ALLEGRO_BITMAP *splash_Screen = NULL;
ALLEGRO_FONT *fonte_Misfits = NULL;
ALLEGRO_FONT* fonte_Misfits_2 = NULL;


int i = 15, j = 12;  //posisao inicial do Pacman na matriz
int q = 20;         //tamanho de cada celula no mapa
int posy = i*q;
int posx = j*q;

//Variável de direção
//se dir = -1 -> esquerda
//se dir = 1  -> direita
//se dir = 2  -> cima
//se dir = -2 -> baixo
//se dir = 0  -> parado
int dir = 0;

bool key[5] = { false, false, false, false, false };
bool redraw = true;
bool sair = false;
bool inicial = true;
int inicializa() {

    if(!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
    }

    if(!al_install_keyboard())
    {
        cout << "Falha ao inicializar o teclado" << endl;
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }

    if(!al_init_image_addon())
    {
        cout <<"Falha ao iniciar al_init_image_addon!" << endl;
        return 0;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        cout << "Falha ao inicializar a tela" << endl;
        al_destroy_timer(timer);
        return 0;
    }


	// Fontes

	al_init_font_addon();
	al_init_ttf_addon();

	fonte_Misfits = al_load_font("fontes/MISFITS_.TTF", 20, 0);
	fonte_Misfits_2 = al_load_font("fontes/MISFITS_.TTF", 60, 0);
	if (!fonte_Misfits || !fonte_Misfits_2) {
		cout << "Falha ao carregar fonte MISFITS_" << endl;
		al_destroy_display(display);
		al_destroy_font(fonte_Misfits_2);
		return 0;
	}

	// Carregar BITMAPS e Imagens
	
	splash_Screen = al_load_bitmap("Splash.tga");
	if (!splash_Screen) {
		cout << "Falha ao carregar tela inicial" << endl;
		al_destroy_display(display);
		return 0;
	}



    if (key[KEY_ENTER]){
        al_destroy_bitmap(splash_Screen); // Destruir Splash Screen
    } 
    

    mapa = al_load_bitmap("map.tga");
    if(!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mapa,0,0,0);

    pacman = al_load_bitmap("imagens/pac.tga");
    if(!pacman)
    {
        cout << "Falha ao carregar o pacman!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(pacman,posx,posy,0);

    ball = al_load_bitmap("imagens/bolinha.tga");
    if(!ball){
        cout << "Falha ao carregar as bolinhas!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(ball,posx,posy,0);

	pipula = al_load_bitmap("imagens/pipula.tga");
	if (!pipula) {
		cout << "Falha ao carregar as pilulas!" << endl;
		al_destroy_display(display);
		return 0;
	}
	al_draw_bitmap(pipula, posx, posy, 0);

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        cout << "Falha ao criar a  fila de eventos" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return 0;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_start_timer(timer);

    return 1;
}

int main(int argc, char **argv)
{
    if(!inicializa()) return -1;

	int pontos = 0;


    while(!sair)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {   
            if(key[KEY_ENTER]) {
                inicial = false;
            }
            
            if(key[KEY_UP] && MAPA[i-1][j] != '1' and inicial == false) {
                i--;
                posy = i*q;
            }

            if(key[KEY_DOWN] && MAPA[i+1][j] != '1' and inicial == false) {
                i++;
                posy = i*q;
            }

            if(key[KEY_LEFT] && MAPA[i][j-1] != '1' and inicial == false) {
                j--;
                posx = j*q;
            }

            if(key[KEY_RIGHT] && MAPA[i][j+1] != '1' and inicial == false) {
                j++;
                posx = j*q;
            }

            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ENTER:
                key[KEY_ENTER] = true;
                break;

            case ALLEGRO_KEY_UP:
                key[KEY_UP] = true;
                break;

            case ALLEGRO_KEY_DOWN:
                key[KEY_DOWN] = true;
                break;

            case ALLEGRO_KEY_LEFT:
                key[KEY_LEFT] = true;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[KEY_RIGHT] = true;
                break;
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ENTER:
                key[KEY_ENTER] = false;
                break;

            case ALLEGRO_KEY_UP:
                key[KEY_UP] = false;
                break;

            case ALLEGRO_KEY_DOWN:
                key[KEY_DOWN] = false;
                break;

            case ALLEGRO_KEY_LEFT:
                key[KEY_LEFT] = false;
                break;

            case ALLEGRO_KEY_RIGHT:
                key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_ESCAPE:
                sair = true;
                break;
            }
        }

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(0,0,0));
            if (inicial == true){
				al_draw_bitmap(splash_Screen, 0, 0, 0);
				al_draw_textf(fonte_Misfits_2, al_map_rgb(65, 166, 50), SCREEN_W/2, SCREEN_H/2, ALLEGRO_ALIGN_CENTER, "APERTA ENTER AI");
				if (DEBUG_MODE == true) cout << "1" << endl;
            }
            else {
				al_draw_bitmap(mapa,0,0,0);

				// Carregar itens do mapa/ bolinha e pilula 
				for (int a = 0; a < 26; a++)
					for (int b = 0; b < 26; b++) {
						if (MAPA[a][b] == '2') al_draw_bitmap(ball, b * q, a * q, 0);
						if (MAPA[a][b] == '3') al_draw_bitmap(pipula, b * q, a * q, 0);
					}
				al_draw_bitmap(pacman,posx,posy,0);
				if (DEBUG_MODE == true) {
					cout << "2" << endl;
					cout << "x = " << posx << " y = " << posy << endl;
				}

				for (int i = 0; i < 26; i++) {
					for (int j = 0; j < 26; j++) {
						// Remover do mapa itens
						if (i == (posy / 20) && j == (posx / 20) && MAPA[i][j] == '3') {
							MAPA[i][j] = '0';	//Tirar Pilula do mapa
							pontos += 33;
						}
						if (i == (posy / 20) && j == (posx / 20) && MAPA[i][j] == '2') {
							MAPA[i][j] = '0';	// Tirar Bolinha do mapa
							pontos += 10;	// Aumentar pontuacao
							if (DEBUG_MODE == true) cout << pontos << endl;
						}
					}
				}
				al_draw_textf(fonte_Misfits, al_map_rgb(65, 166, 50), 200, 510, ALLEGRO_ALIGN_CENTER, "%d PONTOS", pontos );
            }

            al_flip_display();
        }
    }

	// Destruir BITMAPS
    al_destroy_bitmap(mapa);
    al_destroy_bitmap(pacman);
	al_destroy_bitmap(ball);
	al_destroy_bitmap(pipula);

	// Destruir Fontes
	al_destroy_font(fonte_Misfits);


	// Destruir
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
