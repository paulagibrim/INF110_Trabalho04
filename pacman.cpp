//***************************QUARTO TRABALHO DE INF110****************************//
/*                                                                                */
/*TRABALHO ELABORADO PELOS ALUNOS DA DISCIPLINA INF 110 DA UNIVERSIDADE FEDERAL DE*/
/*VIÇOSA EM 2019/I.                                                               */
/*                                                                                */
/*    GRUPO:  PAULA GIBRIM    - ES90366                                           */
/*            RENAN LOPES     - ES97370                                           */
/*            GABRIEL BATISTA - ES98882                                           */
/*            PEDRO HOTE      - ESXXXXX                                           */
/*                                                                                */
/*   MODIFICAÇÃO: 20/06/2019                                                      */
/*                                                                                */
//********************************************************************************//

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
#include <stdlib.h>

using namespace std;

//MAPA A SER PERCORRIDO
char MAPA[26][26] ={
    /*LEGENDA: 
    0 = VAZIO
    1 = PAREDES
    2 = BOLINHAS
    3 = PILULA
    4 E 5 = PORTAL*/

    "1111111111111111111111111",
    "1222222221111111222222221",
    "1211111121111111211111121",
    "1211111122224222211111121",
    "1222222221111111222232221",
    "1112111121111111211112111",
    "1222111122221222211112221",
    "1212111111121211111112121",
    "1212222111221221112222121",
    "1211112111211121112111121",
    "1211112122222222212111121",
    "1222112221112111222112221",
    "1112111121000001211112111",
    "1222222221000001222222221",
    "1211111121111111211111121",
    "1211122222222222222211121",
    "1322221111112111111222221",
    "1111121112222222111211111",
    "1111121112111112111211111",
    "1222222222222222222222221",
    "1211121111112111111211121",
    "1222221111112111111222221",
    "1211132222212122222211121",
    "1222221111225221111222221",
    "1111111111111111111111111",
};

//TECLAS QUE SERÃO USADAS
enum MYKEYS{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER
};

//DEBUG MODE
bool DEBUG_MODE = true;

//VARIÁVEIS
const float FPS = 6;                        //DEFINIÇÃO DE FPS
const int SCREEN_W = 500;                   //TAMANHO DA TELA
const int SCREEN_H = 550;                   //TAMANHO DA TELA
int q = 20;                                 //TAMANHO DE CADA CÉLULA DO MAPA
int portalH, portalW;                       //POSIÇÃO DO PORTAL
int pontos = 0, graus = 0;                  //ANIMAÇÃO DA TELA INICIAL
int cont = 0;
bool key[5] = { false, false, false, false, false };    //VARIÁVEL DE USO DE CADA TECLA DEFINIDA ANTERIORMENTE
bool redraw = true;                                     //VARIÁVEL PARA REDESENHAR A TELA
bool sair = false;                                      //VARIÁVEL PARA SAIR
bool inicial = true;                                    //VARIÁVEL QUE INFORMA SE ESTÁ NA TELA INICIAL (TRUE) OU NÃO (FALSE)
bool andou = false;                                     ////////////////////////
bool win = false, lose = false, final = false;


//DEFINIÇÕES DO ALLEGRO
ALLEGRO_DISPLAY *display = NULL;                            //DISPLAY (TELA)
ALLEGRO_EVENT_QUEUE *event_queue = NULL;                    //FILA DE EVENTOS
//
ALLEGRO_TIMER *timer = NULL;                                //TEMPORIZADOR
ALLEGRO_BITMAP *mapa = NULL;                                //MAPA
ALLEGRO_BITMAP *pacman = NULL;                              //PACMAN (MORTY)
ALLEGRO_BITMAP *ball = NULL;                                //BOLINHA (COMÍVEL)
ALLEGRO_BITMAP *pilula = NULL;                              //PILULA QUE DÁ MAIS PONTOS
ALLEGRO_BITMAP *barra = NULL;                               //ESPAÇO DAS PONTUAÇÕES ????????
ALLEGRO_BITMAP *portal = NULL;                              //PORTAL NA TELA INICIAL
ALLEGRO_BITMAP *splash_Screen = NULL;                       //TELA INICIAL (SPLASH)
ALLEGRO_BITMAP *pportal = NULL;                             //PORTAL IN GAME
ALLEGRO_BITMAP *phantom1 = NULL;                            //FANTASMA1
ALLEGRO_BITMAP *phantom2 = NULL;                            //FANTASMA2
ALLEGRO_BITMAP *phantom3 = NULL;                            //FANTASMA3
ALLEGRO_BITMAP *phantom4 = NULL;                            //FANTASMA4
ALLEGRO_BITMAP *ganhou = NULL;							    //TELA QUANDO GANHA
ALLEGRO_BITMAP *perdeu = NULL;							    //TELA QUANDO PERDE
//
ALLEGRO_FONT *fonte_Misfits = NULL;                         //FONTE1 USADA
ALLEGRO_FONT *fonte_Misfits_2 = NULL;                       //FONTE2 USADA
ALLEGRO_FONT *fonte_Misfits_3 = NULL;                       //FONTE3 USADA
//
ALLEGRO_SAMPLE *musica_Background = NULL;                   //MUSICA DE FUNDO
ALLEGRO_SAMPLE_INSTANCE *backgroundMusica_instance = NULL;  //INSTANCIA DA MUSICA DE FUNDO

//STRUCT DOS PERSONAGENS(FANTASMAS E PACMAN)
struct personagem{
    string direcao, indo;   //direção que quer ir - direção que está indo
    bool andou = false;     //andou = true - não andou = false
    int x,y;                //ponto em x e em y (pela matriz)
    int posicaox = x*q;
    int posicaoy = y*q;     //posiçao x e y em pixels
};

//VETOR DE PERSONAGENS
personagem todos[5];
void def_personagens(personagem todos[5]){
    //PACMAN
    todos[0].x = 12;
    todos[0].y = 15;

    //PHANTOM1
    todos[1].x = 1;
    todos[1].y = 1;

    //PHANTOM2
    todos[2].x = 1;
    todos[2].y = 1;

    //PHANTOM3
    todos[3].x = 1;
    todos[3].y = 1;

    //PHANTOM4
    todos[4].x = 1;
    todos[4].y = 1;
}

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
	ganhou = al_load_bitmap("imagens/ganhou.bmp");
	if (!ganhou) {
		cout << "Falha ao carregar ganhou.bmp";
		al_destroy_display(display);
	}
	perdeu = al_load_bitmap("imagens/perdeu.bmp");
	if (!perdeu) {
		cout << "Falha ao carregar perdeu.bmp";
		al_destroy_display(display);
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
    al_draw_bitmap(pacman,todos[0].posicaox,todos[0].posicaoy,0);

    ball = al_load_bitmap("imagens/bolinha.tga");   //CARREGAR A BOLINHA
    if(!ball){
        cout << "Falha ao carregar as bolinhas!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(ball,q,q,0);

	pilula = al_load_bitmap("imagens/pipula.tga");  //CARREGAR A PÍLULA (MAIS PONTOS)
	if (!pilula) {
		cout << "Falha ao carregar as pilulas!" << endl;
		al_destroy_display(display);
		return 0;
	}
	al_draw_bitmap(pilula, 3*q, 3*q, 0);

    pportal = al_load_bitmap("imagens/portalpqn.png");
    if (!pportal){
        cout << "Falha ao carregar os portais." << endl;
        al_destroy_display(display);
        return 0;
    }

    phantom1 = al_load_bitmap("imagens/Fantasminhas/pha01.tga");
    if (!phantom1){
        cout << "Falha ao carregar fantasma 1." << endl;
        al_destroy_display(display);
        return 0;
    }
    phantom2 = al_load_bitmap("imagens/Fantasminhas/pha02.tga");
    if (!phantom2){
        cout << "Falha ao carregar fantasma 2." << endl;
        al_destroy_display(display);
        return 0;
    }
    phantom3 = al_load_bitmap("imagens/Fantasminhas/pha03.tga");
    if (!phantom3){
        cout << "Falha ao carregar fantasma 3." << endl;
        al_destroy_display(display);
        return 0;
    }
    phantom4 = al_load_bitmap("imagens/Fantasminhas/pha04.tga");
    if (!phantom4){
        cout << "Falha ao carregar fantasma 4." << endl;
        al_destroy_display(display);
        return 0;
    }
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

//**FUNÇÕES DE VERIFICAÇÃO DE VITORIA/DERROTA**// ok
void phant_derrota(personagem todos[5], int n){
    if (todos[0].posicaox == todos[n].posicaox
    and todos[0].posicaoy == todos[n].posicaoy){
        lose = true;
    }

    if (todos[0].direcao == "right" and todos[n].direcao == "left"){
        if (todos[0].posicaox == (todos[n].posicaox + q) 
        and todos[0].posicaoy == todos[n].posicaoy){
            lose = true;
        }
    }
    
    if (todos[n].direcao == "right" and todos[0].direcao == "left"){
        if (todos[0].posicaox == (todos[n].posicaox - q) 
        and todos[0].posicaoy == todos[n].posicaoy){
            lose = true;
        }
    }
    
    if (todos[0].direcao == "up" and todos[n].direcao == "down"){
        if (todos[0].posicaox == todos[n].posicaox 
        and todos[0].posicaoy == (todos[n].posicaoy - q)){
            lose = true;
        }
    }

    if (todos[n].direcao == "up" and todos[0].direcao == "down"){
        if (todos[0].posicaox == todos[n].posicaox 
        and todos[0].posicaoy == (todos[n].posicaoy + q)){
            lose = true;
        }
    }
}
void verifica(){
    if (pontos == 2640){
        win = true;
    }
    phant_derrota(todos, 1); 
    phant_derrota(todos, 2);
    phant_derrota(todos, 3);
    phant_derrota(todos, 4);
}

//**FUNÇÃO MOVIMENTO**//
void movimento(personagem todos[],int n){
    //SE A DIREÇÃO FOR XX E TIVER ESPAÇO PARA ANDAR NESSA DIREÇÃO E NÃO ESTIVER NA TELA INICIAL
    //ELE ANDA NAQUELA DIREÇÃO
    if (todos[n].direcao == "up" and MAPA[todos[n].x][todos[n].y - 1] != '1' and !inicial)
        todos[n].indo = "up";
    if (todos[n].indo == "up" and MAPA[todos[n].x][todos[n].y - 1] != '1' and !inicial and !todos[n].andou){
        todos[n].y--;
        todos[n].posicaoy = todos[n].y * q;
        todos[n].andou = true;
    }

    if (todos[n].direcao == "down" and MAPA[todos[n].x][todos[n].y + 1] != '1' and !inicial)
        todos[n].indo = "down";
    if (todos[n].indo == "down" and MAPA[todos[n].x][todos[n].y + 1] != '1' and !inicial and !todos[n].andou){
        todos[n].y--;
        todos[n].posicaoy = todos[n].y * q;
        todos[n].andou = true;
    }
    
    if (todos[n].direcao == "left" and MAPA[todos[n].x - 1][todos[n].y] != '1' and !inicial)
        todos[n].indo = "letf";
    if (todos[n].indo == "left" and MAPA[todos[n].x - 1][todos[n].y] != '1' and !inicial and !todos[n].andou){
        todos[n].x--;
        todos[n].posicaox = todos[n].x * q;
        todos[n].andou = true;
    }
    
    if (direcao == "left" and MAPA_PACMAN[i][j-1] != '1' and MAPA_PACMAN[i-1][j] != '6' and !inicial)
        indo = "left";
    if (indo == "left" and MAPA_PACMAN[i][j-1] != '1' and MAPA_PACMAN[i-1][j] != '6' and !inicial and !andou){
        j--;
        posx = j*q;
        andou = true;/////////////////
    }


    if (direcao == "right" and MAPA_PACMAN[i][j+1] != '1' and MAPA_PACMAN[i-1][j] != '6' and !inicial)
        indo = "right";
    if (indo == "right" and MAPA_PACMAN[i][j+1] != '1' and MAPA_PACMAN[i-1][j] != '6' and !inicial and !andou){
        j++;
        posx = j*q;
        andou = true;///////////////////
    }
}
//**FUNÇÃO TECLADO**//
void teclado(personagem todos[]){
    verifica();
    if(key[KEY_ENTER]) inicial = false;                    //DEFINE O APERTO DA TECLA ENTER (INICIALIZAÇÃO)
    if (key[KEY_UP]) todos[1].direcao = "up";              //DEFINE A DIREÇÃO PARA CIMA
    if (key[KEY_DOWN]) todos[1].direcao = "down";          //DEFINE A DIREÇÃO PARA BAIXO
    if (key[KEY_LEFT]) todos[1].direcao = "left";          //DEFINE A DIREÇÃO PARA A ESQUERDA
    if (key[KEY_RIGHT]) todos[1].direcao = "right";        //DEFINE A DIREÇÃO PARA A DIREITA
    

    //CASO DE PORTAL
    if (MAPA_PACMAN[i][j] == '4' and !inicial and indo == "right"){
        j = 13;
        i = 23;
        posx = j*q;
        posy = i*q;
    }
    if (MAPA_PACMAN[i][j] == '4' and !inicial and indo == "left"){
        j = 11;
        i = 23;
        posx = j*q;
        posy = i*q;
    }
    if (MAPA_PACMAN[i][j] == '5' and !inicial and indo == "right"){
        j = 13;
        i = 3;
        posx = j*q;
        posy = i*q;
    }
    if (MAPA_PACMAN[i][j] == '5' and !inicial and indo == "left"){
        j = 11;
        i = 3;
        posx = j*q;
        posy = i*q;
    }
    if (MAPA_PACMAN[i][j] == '5' and !inicial and indo == "down"){
        j = 12;
        i = 3;
        posx = j*q;
        posy = i*q;
    }
    
    redraw = true;
}