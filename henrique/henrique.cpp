#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>

using namespace std;

const float FPS = 100;
const int SCREEN_W = 500;
const int SCREEN_H = 520;

enum MYKEYS
{
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

//matriz definindo mapa do jogo: 1 representa paredes, 0 representa corredor
char MAPA[26][26] =
{
	"1111111111111111111111111",
	"1000000001111111000000001",
	"1011111101111111011111101",
	"1011111100000000011111101",
	"1000000001111111000000001",
	"1110111101111111011110111",
	"1000111100001000011110001",
	"1010111111101011111110101",
	"1010000111001001110000101",
	"1011110111011101110111101",
	"1011110100000000010111101",
	"1000110001110111000110001",
	"1110111101110111011110111",
	"1000000001110111000000001",
	"1011111101110111011111101",
	"1011100000000000000011101",
	"1000001111110111111000001",
	"1111101110000000111011111",
	"1111101110111110111011111",
	"1000000000000000000000001",
	"1011101111110111111011101",
	"1000001111110111111000001",
	"1011100000010100000011101",
	"1000001111000001111000001",
	"1011100000011100000011101",
	"1111111111111111111111111",
};


ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* event_queue = NULL;
ALLEGRO_TIMER* timer = NULL;
ALLEGRO_BITMAP* mapa = NULL;
ALLEGRO_BITMAP* pacmanright = NULL;
ALLEGRO_BITMAP* pacmanup = NULL;
ALLEGRO_BITMAP* pacmandown = NULL;
ALLEGRO_BITMAP* pacmanleft = NULL;
ALLEGRO_BITMAP* parede = NULL;
ALLEGRO_BITMAP* pacmanfechadoup = NULL;
ALLEGRO_BITMAP* pacmanfechadoleft = NULL;
ALLEGRO_BITMAP* pacmanfechadoright = NULL;
ALLEGRO_BITMAP* pacmanfechadodown = NULL;
ALLEGRO_BITMAP* bolinha = NULL;

int i = 15, j = 12; //posi��o inicial do Pacman na matriz
int q = 20; //tamanho de cada c�lula no mapa
int posy = i * q;
int posx = j * q;
int parede_x;
int parede_y;
int sentido;
int andando;
int bolinha_x;
int bolinha_y;
bool key[4] = { false, false, false, false };
bool redraw = true;
bool sair = false;
const int PAREDE_SIZE = 19;
int cont1 = 19, cont2 = 19, cont3 = 19, cont4 = 19;
int contposicao = 10;
int inicializa() {
	if (!al_init())
	{
		cout << "Falha ao carregar Allegro" << endl;
		return 0;
	}

	if (!al_install_keyboard())
	{
		cout << "Falha ao inicializar o teclado" << endl;
		return 0;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer)
	{
		cout << "Falha ao inicializar o temporizador" << endl;
		return 0;
	}

	if (!al_init_image_addon())
	{
		cout << "Falha ao iniciar al_init_image_addon!" << endl;
		return 0;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display)
	{
		cout << "Falha ao inicializar a tela" << endl;
		al_destroy_timer(timer);
		return 0;
	}
	
	parede = al_create_bitmap(PAREDE_SIZE, PAREDE_SIZE);		//cria um bitmap 20 x 20
	

	pacmanright = al_load_bitmap("pacmanright.tga");
	if (!pacmanright)
	{
		cout << "Falha ao carregar o pacman!" << endl;
		al_destroy_display(display);
		return 0;
	}


	pacmanup = al_load_bitmap("pacmanup.tga");
	if (!pacmanup)
	{
		cout << "Falha ao carregar o pacman!" << endl;
		al_destroy_display(display);
		return 0;
	}
	

	pacmandown = al_load_bitmap("pacmandown.tga");
	if (!pacmandown)
	{
		cout << "Falha ao carregar o pacman!" << endl;
		al_destroy_display(display);
		return 0;
	}
	
	pacmanleft = al_load_bitmap("pacmanleft.tga");
	if (!pacmanleft)
	{
		cout << "Falha ao carregar o pacman!" << endl;
		al_destroy_display(display);
		return 0;
	}

	pacmanfechadoright = al_load_bitmap("pacmanfechadoright.tga");
	if (!pacmanfechadoright)
	{
		cout << "Falha ao carregar o pacman!" << endl;
		al_destroy_display(display);
		return 0;
	}

	pacmanfechadoleft = al_load_bitmap("pacmanfechadoleft.tga");
	if (!pacmanfechadoleft)
	{
		cout << "Falha ao carregar o pacman!" << endl;
		al_destroy_display(display);
		return 0;
	}
	
	pacmanfechadoup = al_load_bitmap("pacmanfechadoup.tga");
	if (!pacmanfechadoup)
	{
		cout << "Falha ao carregar o pacman!" << endl;
		al_destroy_display(display);
		return 0;
	}

	pacmanfechadodown = al_load_bitmap("pacmanfechadodown.tga");
	if (!pacmanfechadodown)
	{
		cout << "Falha ao carregar o pacman!" << endl;
		al_destroy_display(display);
		return 0;
	}

	bolinha = al_load_bitmap("bolinha.tga");
	if (!bolinha)
	{
		cout << "Falha ao carregar o pacman!" << endl;
		al_destroy_display(display);
		return 0;
	}




	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		cout << "Falha ao criar a fila de eventos" << endl;
		al_destroy_display(display);
		al_destroy_timer(timer);
		return 0;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	al_start_timer(timer);

	return 1;
}

int main(int argc, char** argv)
{
	if (!inicializa()) return -1;

	while (!sair)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		bool foi = false;
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (key[KEY_UP])
				sentido = 1;
			if (MAPA[i - 1][j] != '1' and sentido == 1 and posy%20==0 and posx%20==0)
				andando = sentido;
			if (andando == 1 and foi == false and i == -1 and cont1 == 0)
				i = 26;
			if (andando == 1 and MAPA[i - 1][j] != '1' and foi == false)
			{
				if (cont1 == 19)
					i--;
				foi = true;
				posy = i * q + cont1;
				cont1 -= 1;
				if (cont1 == -1)
					cont1 = 19;
			}
			else if (cont1 > 0 and andando == 1 and foi == false)
			{
				cont1 -= 1;
				posy = i * q + cont1;
				foi = true;
			}
		
			cout << cont1 << " ";


			if (key[KEY_DOWN])
				sentido = 2;
			if (MAPA[i + 1][j] != '1' and sentido == 2 and posy % 20 == 0 and posx % 20 == 0)
				andando = sentido;
			if (andando == 2 and foi == false and i == 26 and cont2 == 0)
				i = -1;
			if (MAPA[i + 1][j] != '1' and andando == 2 and foi == false)
			{
				if (cont2 == 19)
					i++;
				foi = true;
				posy = i * q - cont2;
				cont2 -= 1;
				if (cont2 == -1)
					cont2 = 19;
			}
			else if (cont2 > 0 and andando == 2 and foi == false)
			{
				cont2 -= 1;
				posy = i * q - cont2;
				foi = true;
			}

			cout << cont2 << " ";
			if (key[KEY_LEFT])
				sentido = 3;
			if (MAPA[i][j - 1] != '1' and sentido == 3 and posy % 20 == 0 and posx % 20 == 0)
				andando = sentido;
			if (andando == 3 and foi == false and j == -1 and cont3 == 0)
				j = 25;
			if (MAPA[i][j - 1] != '1' and andando == 3 and foi == false)
			{
				if (cont3 == 19)
					j--;
				foi = true;
				posx = j * q + cont3;
				cont3 -= 1;
				if (cont3 == -1)
					cont3 = 19;
			}
			else if (cont3 > 0 and andando == 3 and foi == false )
			{
				cont3 -= 1;
				posx = j * q + cont3;
				foi = true;
			}

			cout << cont3 << " ";

			if (key[KEY_RIGHT])
				sentido = 4;
			if (MAPA[i][j + 1] != '1' and sentido == 4 and posy%20==0 and posx%20==0)
				andando = sentido;
			if (andando == 4 and foi == false and j == 25 and cont4==0)
				j = 0;

			if (MAPA[i][j + 1] != '1' and andando == 4 and foi == false)
			{
				if (cont4 == 19)
					j++;
				
				foi = true;
				posx = j * q - cont4;
				cont4 -= 1;
				if (cont4 == -1)
					cont4 = 19;
			}
			else if (cont4 > 0 and andando == 4 and foi == false )
			{
				cont4 -= 1;
				posx = j * q - cont4;
				foi = true;
			}
			
			cout << cont4 << endl;
			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
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
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
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

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			
	
			al_clear_to_color(al_map_rgb(0, 0, 0));
			

			for (int a = 0; a < 26; a++)
				for (int b = 0; b < 26; b++)
				{
					if (MAPA[a][b] == '1')
					{

						al_set_target_bitmap(parede);						//muda destino dos desenhos para o bitmap bouncer
						al_clear_to_color(al_map_rgb(255, 255, 255));			//limpa e colore de magenta
						al_set_target_bitmap(al_get_backbuffer(display));	//muda de volta o destino dos desenhos para o display
						parede_x = b * q;
						parede_y = a * q;
						al_draw_bitmap(parede, parede_x, parede_y, 0);

					}
					if (MAPA[a][b] == '0')
					{
						bolinha_x = b * q;
						bolinha_y = a * q;
						al_draw_bitmap(bolinha, bolinha_x, bolinha_y, 0);
					}
				}
			if (contposicao % 20 >= 10 and contposicao % 20 < 20)
			{
				if (andando == 1)
					al_draw_bitmap(pacmanfechadoup, posx, posy, 0);
				else if (andando == 2)
					al_draw_bitmap(pacmanfechadodown, posx, posy, 0);
				else if (andando == 3)
					al_draw_bitmap(pacmanfechadoleft, posx, posy, 0);
				else
					al_draw_bitmap(pacmanfechadoright, posx, posy, 0);
				contposicao += 1;
			}
			else
			{
				if (andando == 1)
					al_draw_bitmap(pacmanup, posx, posy, 0);
				else if (andando == 2)
					al_draw_bitmap(pacmandown, posx, posy, 0);
				else if (andando == 3)
					al_draw_bitmap(pacmanleft, posx, posy, 0);
				else
					al_draw_bitmap(pacmanright, posx, posy, 0);
				contposicao += 1;
			}
			al_flip_display();
		}
	}

	al_destroy_bitmap(mapa);
	al_destroy_bitmap(pacmandown);
	al_destroy_bitmap(pacmanright);
	al_destroy_bitmap(pacmanup);
	al_destroy_bitmap(pacmanleft);
	al_destroy_bitmap(pacmanfechadoright);
	al_destroy_bitmap(pacmanfechadoleft);
	al_destroy_bitmap(pacmanfechadoup);
	al_destroy_bitmap(pacmanfechadodown);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}