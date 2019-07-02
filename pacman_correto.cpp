//***************************QUARTO TRABALHO DE INF110****************************//
//**********************************CABEÇALHO*************************************//
/*
    TRABALHO ELABORADO PELOS ALUNOS DA DISCIPLINA INF 110 DA UNIVERSIDADE FEDERAL DE
    VIÇOSA EM 2019/I.

    GRUPO:  PAULA GIBRIM - ES90366
            RENAN LOPES  - ES97370
            GABRIEL XXXX - ESXXXXXX
            PEDRO HOTE   - ESXXXXXX

    MODIFICAÇÃO: 20/06/2019

	ATENÇÃO EU, RENAN VOU REFAZER O MAPA ATUAL;



	CORES
	AZUL = al_map_rgb(15, 174, 191)
	VERDE = al_map_rgb(65, 166, 50)


*/

//* BIBLIOTECAS E VARIÁVEIS GLOBAIS *//

//BIBLIOTECAS
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <cstring>

using namespace std;

//VARIÁVEIS
const float FPS = 10;               //DEFINIÇÃO DE FPS
const int SCREEN_W = 500;           //TAMANHO DA TELA
const int SCREEN_H = 550;           //TAMANHO DA TELA
bool movimento = false;             //VARIÁVEL DE MOVIMENTO (ESTÁ MOVENDO OU NÃO)
string direcao;                     //DIREÇÃO QUE VAI - OPÇÕES: "up", "down", "right", "left"
string indo;                        //DIREÇÃO QUE TÁ INDO - OPÇÕES: "up", "down", "right", "left"

// LEGENDA: MATRIZ DO MAPA
// 0 = VAZIO
// 1 = PAREDES
// 2 = BOLINHAS
// 3 = PILULA
// 4 = PORTAL
// 5 = CASINHA DOS FANTASMAS *****IMPLEMENTAR*****

//MATRIZ DO MAPA
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

bool DEBUG_MODE = true;                                     //DEBUG_MODE
enum MYKEYS{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER        //TECLAS QUE SERÃO USADAS
};

//DEFINIÇÕES DO ALLEGRO
ALLEGRO_DISPLAY *display = NULL;                            //DISPLAY (TELA)
ALLEGRO_EVENT_QUEUE *event_queue = NULL;                    //FILA DE EVENTOS

ALLEGRO_TIMER *timer = NULL;                                //TEMPORIZADOR
ALLEGRO_BITMAP *mapa = NULL;                                //MAPA
ALLEGRO_BITMAP *pacman = NULL;                              //PACMAN (MORTY)
ALLEGRO_BITMAP *ball = NULL;                                //BOLINHA (COMÍVEL)
ALLEGRO_BITMAP *pilula = NULL;                              //PILULA QUE DÁ MAIS PONTOS
ALLEGRO_BITMAP *barra = NULL;                               //ESPAÇO DAS PONTUAÇÕES ????????
ALLEGRO_BITMAP* portal = NULL;                              //PORTAIS
ALLEGRO_BITMAP *splash_Screen = NULL;                       //TELA INICIAL (SPLASH)

ALLEGRO_FONT *fonte_Misfits = NULL;                         //FONTE1 USADA
ALLEGRO_FONT *fonte_Misfits_2 = NULL;                       //FONTE2 USADA
ALLEGRO_FONT *fonte_Misfits_3 = NULL;                       //FONTE3 USADA

ALLEGRO_SAMPLE *musica_Background = NULL;                   //MUSICA DE FUNDO
ALLEGRO_SAMPLE_INSTANCE *backgroundMusica_instance = NULL;  //INSTANCIA DA MUSICA DE FUNDO


int i = 15, j = 12;                             //POSIÇÃO INICIAL DO PACMAN NA MATRIZ
int q = 20;                                     //TAMANHO DE CADA CÉLULA DO MAPA
int posy = i*q;                                 //POSIÇÃO EM Y
int posx = j*q;                                 //POSIÇÃO EM X
int portalH, portalW;                           //POSIÇÃO DO PORTAL

bool key[5] = { false, false, false, false, false };    //VARIÁVEL DE USO DE CADA TECLA DEFINIDA ANTERIORMENTE
bool redraw = true;                                     //VARIÁVEL PARA REDESENHAR A TELA
bool sair = false;                                      //VARIÁVEL PARA SAIR
bool inicial = true;                                    //VARIÁVEL QUE INFORMA SE ESTÁ NA TELA INICIAL (TRUE) OU NÃO (FALSE)
bool andou = false;////////////////////////
//**FUNÇÃO PARA INICIALIZAR O JOGO**//
int inicializa() {
    //CARREGAR O ALLEGRO
    if(!al_init()){
        cout << "Falha ao carregar Allegro." << endl;
        return 0;
    }
    //CARREGAR O TECLADO
    if(!al_install_keyboard()){
        cout << "Falha ao inicializar o teclado." << endl;
        return 0;
    }
    //CARREGAR O TEMPORIZADOR
    timer = al_create_timer(1.0 / FPS);
    if(!timer){
        cout << "Falha ao inicializar o temporizador." << endl;
        return 0;
    }
    //ADDON DE IMAGEM
    if(!al_init_image_addon()){
        cout <<"Falha ao iniciar al_init_image_addon." << endl;
        return 0;
    }
    //CRIAR DISPLAY
    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display){
        cout << "Falha ao inicializar a tela." << endl;
        al_destroy_timer(timer);
        return 0;
    }

	//ADDON DE ÁUDIO
	al_install_audio(); 
	if (!al_install_audio()) {
		cout << "Falha ao inicializar o audio." << endl;
		return 0;
	}

	//ADDON QUE DÁ SUPORTE ÀS EXTENSÕES DE ÁUDIO
	al_init_acodec_addon();
	if (!al_init_acodec_addon()) {
		cout << "Falha ao inicializar o codec de audio." << endl;
		return 0;
	}

	//CARREGAR A STREAM
	musica_Background = al_load_sample("musicas/BitofRickandMorty.ogg");
	if (!musica_Background) {
		cout << "Audio nao carregado" << endl;
		al_destroy_sample(musica_Background);
		return 0;
	}
	al_reserve_samples(1);

	backgroundMusica_instance = al_create_sample_instance(musica_Background);
	al_set_sample_instance_playmode(backgroundMusica_instance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(backgroundMusica_instance, al_get_default_mixer());

	//CARREGAR AS FONTES
	al_init_font_addon();
	al_init_ttf_addon();

	fonte_Misfits = al_load_font("fontes/MISFITS_.TTF", 20, 0);
	fonte_Misfits_2 = al_load_font("fontes/MISFITS_.TTF", 30, 0);
	fonte_Misfits_3 = al_load_font("fontes/MISFITS_.TTF", 20, 0);

    //CARREGAR FONTE3
	if (!fonte_Misfits_3) {
		cout << "Falha ao carregar fonte MISFITS_ 3" << endl;
		al_destroy_display(display);
		return 0;
	}
    //CARREGAR FONTE2 E FONTE1
	if (!fonte_Misfits || !fonte_Misfits_2) {
		cout << "Falha ao carregar fonte MISFITS_" << endl;
		al_destroy_display(display);
		return 0;
	}



	//CARREGAR BITMAPS E IMAGENS
	
	splash_Screen = al_load_bitmap("imagens/splashScreen.bmp");
	if (!splash_Screen) {
		cout << "Falha ao carregar tela inicial." << endl;
		al_destroy_display(display);
		return 0;
	}

	portal = al_load_bitmap("imagens/portal.tga");
	if (!portal) {
		cout << "Falha ao carregar portal." << endl;
		al_destroy_display(display);
		return 0;
	}
	portalW = al_get_bitmap_width(portal);
	portalH = al_get_bitmap_height(portal);



    if (key[KEY_ENTER]){
        al_destroy_bitmap(splash_Screen);           //SE APERTAR ENTER, DESTROI SPLASHSCREEN
    } 
    

    mapa = al_load_bitmap("map.bmp");               //CARREGAR O MAPA
    if(!mapa){
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mapa,0,0,0);


    pacman = al_load_bitmap("imagens/pac.tga");     //CARREGAR O PACMAN (MORTY)
    if(!pacman){
        cout << "Falha ao carregar o pacman!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(pacman,posx,posy,0);

    ball = al_load_bitmap("imagens/bolinha.tga");   //CARREGAR A BOLINHA
    if(!ball){
        cout << "Falha ao carregar as bolinhas!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(ball,posx,posy,0);

	pilula = al_load_bitmap("imagens/pipula.tga");  //CARREGAR A PÍLULA (MAIS PONTOS)
	if (!pilula) {
		cout << "Falha ao carregar as pilulas!" << endl;
		al_destroy_display(display);
		return 0;
	}
	al_draw_bitmap(pilula, posx, posy, 0);

    event_queue = al_create_event_queue();          //CRIAR A FILA DE EVENTOS
    if(!event_queue){
        cout << "Falha ao criar a  fila de eventos." << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return 0;
    }

    //REGISTRANDO OS EVENTOS DA FILA DE EVENTOS
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_flip_display();
    al_start_timer(timer);

    return 1;
}
//**FUNÇÃO TECLADO**//
void teclado(){

    if(key[KEY_ENTER]) inicial = false;                             //DEFINE O APERTO DA TECLA ENTER (INICIALIZAÇÃO)
    if (key[KEY_UP]) direcao = "up";                                //DEFINE A DIREÇÃO PARA CIMA
    if (key[KEY_DOWN]) direcao = "down";                            //DEFINE A DIREÇÃO PARA BAIXO
    if (key[KEY_LEFT]) direcao = "left";                            //DEFINE A DIREÇÃO PARA A ESQUERDA
    if (key[KEY_RIGHT]) direcao = "right";                          //DEFINE A DIREÇÃO PARA A DIREITA
    

    //SE A DIREÇÃO FOR XX E TIVER ESPAÇO PARA ANDAR NESSA DIREÇÃO E NÃO ESTIVER NA TELA INICIAL
    //ELE ANDA NAQUELA DIREÇÃO
    if (direcao == "up" and MAPA[i-1][j] != '1' and !inicial)
        indo = "up";
    if (indo == "up" and MAPA[i-1][j] != '1' and !inicial and !andou){
        i--;
        posy = i*q;
        andou = true; ////////
    }

    if (direcao == "down" and MAPA[i+1][j] != '1' and !inicial)
        indo = "down";
    if (indo == "down" and MAPA[i+1][j] != '1' and !inicial and !andou){
        i++;
        posy = i*q;
        andou = true;/////////
    }
    
    if (direcao == "left" and MAPA[i][j-1] != '1' and !inicial)
        indo = "left";
    if (indo == "left" and MAPA[i][j-1] != '1' and !inicial and !andou){
        j--;
        posx = j*q;
        andou = true;/////////////////
    }

    if (direcao == "right" and MAPA[i][j+1] != '1' and !inicial)
        indo = "right";
    if (indo == "right" and MAPA[i][j+1] != '1' and !inicial and !andou){
        j++;
        posx = j*q;
        andou = true;///////////////////
    }

    redraw = true;
}


//FUNÇÃO PRINCIPAL
int main(int argc, char **argv)
{
    if(!inicializa()) return -1; //SE NÃO INICIALIZAR, RETORNA -1 (ERRO)

	int pontos = 0, graus = 0;

	al_play_sample_instance(backgroundMusica_instance); // MÚSICA EM LOOP

    while(!sair){
        andou = false;
        
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER){   
            teclado();
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            break;
        }else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
            //CASOS
            switch(ev.keyboard.keycode){
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
        else if(ev.type == ALLEGRO_EVENT_KEY_UP){
            //CASOS
            switch(ev.keyboard.keycode){
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

        if(redraw && al_is_event_queue_empty(event_queue)){
            redraw = false;

            al_clear_to_color(al_map_rgb(0,0,0));
            if (inicial == true){
				al_draw_bitmap(splash_Screen, 0, 0, 0);
				graus += 5;
				if (graus > 360) graus = 0;
				al_draw_rotated_bitmap(portal, portalW / 2, portalH / 2, 250, 350, graus * 3.1415 / 180, 0);
				al_draw_textf(fonte_Misfits_2, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 40, ALLEGRO_ALIGN_CENTER, "APERTE ENTER");
				al_draw_textf(fonte_Misfits_3, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 80, ALLEGRO_ALIGN_CENTER, "PARA INICIAR");
            }else {
				al_draw_bitmap(mapa,0,0,0);

				// CARREGAR ITENS DO MAPA
				for (int a = 0; a < 26; a++)
					for (int b = 0; b < 26; b++) {
						if (MAPA[a][b] == '2') al_draw_bitmap(ball, (b * q) + 4, (a * q) + 6, 50);  //CARREGA BOLA
						if (MAPA[a][b] == '3') al_draw_bitmap(pilula, (b * q) + 6, (a * q), 0);     //CARREGA PILULA
					}
				al_draw_bitmap(pacman,posx,posy,0);///////////
				if (DEBUG_MODE == true) {
					cout << "2" << endl;
					cout << "x = " << posx << " y = " << posy << endl;
				}

				for (int i = 0; i < 26; i++) {
					for (int j = 0; j < 26; j++) {
						// REMOVER ITENS DO MAPA
						if (i == (posy / 20) && j == (posx / 20) && MAPA[i][j] == '3') {
							MAPA[i][j] = '0';	//TIRA A PILULA DO MAPA
							pontos += 33;
						}
						if (i == (posy / 20) && j == (posx / 20) && MAPA[i][j] == '2') {
							MAPA[i][j] = '0';	//TIRA BOLINHA DO MAPA
							pontos += 10;	    //AUMENTA A PONTUAÇÃO
							if (DEBUG_MODE == true) cout << pontos << endl;
						}
					}
				}
				al_draw_textf(fonte_Misfits, al_map_rgb(65, 166, 50), 80, 515, ALLEGRO_ALIGN_CENTER, "%d PONTOS", pontos );
            }

            al_flip_display();
        }
    }

	// DESTRUIR BITMAP
    al_destroy_bitmap(mapa);
	al_destroy_bitmap(splash_Screen);
    al_destroy_bitmap(pacman);
	al_destroy_bitmap(ball);

	al_destroy_bitmap(pilula);
	al_destroy_bitmap(portal);

	// DESTRUIR FONTES
	al_destroy_font(fonte_Misfits);


	// DESTRUIR O RESTO
	al_destroy_display(display);
	al_destroy_sample_instance(backgroundMusica_instance);
	al_destroy_sample(musica_Background);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);


    return 0;
}