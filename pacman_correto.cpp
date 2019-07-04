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
const float FPS = 8;                //DEFINIÇÃO DE FPS
const int SCREEN_W = 500;           //TAMANHO DA TELA
const int SCREEN_H = 550;           //TAMANHO DA TELA
bool movimento = false;             //VARIÁVEL DE MOVIMENTO (ESTÁ MOVENDO OU NÃO)
string direcao;                     //DIREÇÃO QUE VAI - OPÇÕES: "up", "down", "right", "left"
string indo;                        //DIREÇÃO QUE TÁ INDO - OPÇÕES: "up", "down", "right", "left"
string direcaop, indop;

// LEGENDA: MATRIZ DO MAPA
// 0 = VAZIO
// 1 = PAREDES
// 2 = BOLINHAS
// 3 = PILULA
// 4 E 5 = PORTAL
// 6 = CASINHA DOS FANTASMAS

//MATRIZ DO MAPA
char MAPA_PACMAN[26][26] =
{
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
    "1222112221111111222112221",
    "1112111121111111211112111",
    "1222222221111111222222221",
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


char MAPA_PHANTOM[26][26] =
{
    "1111111111111111111111111",
    "1000000001111111000000001",
    "1011111101111111011111101",
    "1011111100004000011111101",
    "1000000001111111000000001",
    "1110111101111111011110111",
    "1000111100001000011110001",
    "1010111111101011111110101",
    "1010000111001001110000101",
    "1011110111011101110111101",
    "1011110100000000010111101",
    "1000110001110111000110001",
    "1110111101000001011110111",
    "1000000001001001000000001",
    "1011111101110111011111101",
    "1011100000000000000011101",
    "1000001111110111111000001",
    "1111101110000000111011111",
    "1111101110111110111011111",
    "1000000000000000000000001",
    "1011101111110111111011101",
    "1000001111110111111000001",
    "1011100000010100000011101",
    "1000001111005001111000001",
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
ALLEGRO_BITMAP *pacman_up = NULL;                           //PACMAN (MORTY)
ALLEGRO_BITMAP *pacman_down = NULL;
ALLEGRO_BITMAP *pacman_left = NULL;
ALLEGRO_BITMAP *pacman_right = NULL;

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

ALLEGRO_FONT *fonte_Misfits = NULL;                         //FONTE1 USADA
ALLEGRO_FONT *fonte_Misfits_2 = NULL;                       //FONTE2 USADA
ALLEGRO_FONT *fonte_Misfits_3 = NULL;                       //FONTE3 USADA

ALLEGRO_SAMPLE *musica_Background = NULL;                   //MUSICA DE FUNDO
ALLEGRO_SAMPLE_INSTANCE *backgroundMusica_instance = NULL;  //INSTANCIA DA MUSICA DE FUNDO


int i = 15, j = 12;                             //POSIÇÃO INICIAL DO PACMAN NA MATRIZ
int k = 13, l = 13;                             //POSIÇÃO INICIAL DO FANTASMA
int q = 20;                                     //TAMANHO DE CADA CÉLULA DO MAPA
int posy = i*q;                                 //POSIÇÃO EM Y
int posx = j*q;                                 //POSIÇÃO EM X
int portalH, portalW;                           //POSIÇÃO DO PORTAL
int pposx1 = k*q, pposy1 = l*q;
string direcaop2, indop2;
bool andoup2;
int k2 = 13 , l2 = 13;
int pposy2 = l2*q, pposx2 = k2*q;
string direcaop3, indop3;
bool andoup3;
int k3 = 10, l3 = 13;
int pposy3 = l3*q, pposx3 = k3*q;
string direcaop4, indop4;
bool andoup4;
int k4 = 10, l4 = 12;
int pposy4 = l4*q, pposx4 = k4*q;

//*//
string ultimodir, ultimodir2, ultimodir3, ultimodir4;
//*//

int pontos = 0, graus = 0, contFim = 0;

bool key[5] = { false, false, false, false, false };    //VARIÁVEL DE USO DE CADA TECLA DEFINIDA ANTERIORMENTE
bool redraw = true;                                     //VARIÁVEL PARA REDESENHAR A TELA
bool sair = false;                                      //VARIÁVEL PARA SAIR
bool inicial = true;                                    //VARIÁVEL QUE INFORMA SE ESTÁ NA TELA INICIAL (TRUE) OU NÃO (FALSE)
bool andou = false;////////////////////////
bool andoup = false;
bool win = false, lose = false, fim = false, final = false;
int cont = 0;

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
    
    mapa = al_load_bitmap("map.bmp");               //CARREGAR O MAPA
    if(!mapa){
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
    }
    al_draw_bitmap(mapa,0,0,0);

	pacman_up = al_load_bitmap("imagens/Pacman/MortyBigUp.tga");     //CARREGAR O PACMAN (MORTY)
    if(!pacman_up){
        cout << "Falha ao carregar o pacman_up!" << endl;
        al_destroy_display(display);
        return 0;
    }
    //al_draw_bitmap(pacman_up,posx,posy,0);
	pacman_down = al_load_bitmap("imagens/Pacman/MortyBigDown.tga");     //CARREGAR O PACMAN (MORTY)
	if (!pacman_up) {
		cout << "Falha ao carregar o pacman_down!" << endl;
		al_destroy_display(display);
		return 0;
	}
	pacman_left = al_load_bitmap("imagens/Pacman/MortyBigLeft.tga");     //CARREGAR O PACMAN (MORTY)
	if (!pacman_up) {
		cout << "Falha ao carregar o pacman_left!" << endl;
		al_destroy_display(display);
		return 0;
	}
	pacman_right = al_load_bitmap("imagens/Pacman/MortyBigRight.tga");     //CARREGAR O PACMAN (MORTY)
	if (!pacman_up) {
		cout << "Falha ao carregar o pacman_right!" << endl;
		al_destroy_display(display);
		return 0;
	}

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

//**FUNÇÃO TECLADO**//
void teclado(){
    if (pontos == 2640){
        win = true;
    }
    if ((posx == pposx1 and posy == pposy1) or (posx == pposx2 and posy == pposy2)
    or (posx == pposx3 and posy == pposy3) or (posx == pposx4 and posy == pposy4)){
        lose = true;
    }

    if(key[KEY_ENTER]) inicial = false;                             //DEFINE O APERTO DA TECLA ENTER (INICIALIZAÇÃO)
    if (key[KEY_UP]) direcao = "up";                                //DEFINE A DIREÇÃO PARA CIMA
    if (key[KEY_DOWN]) direcao = "down";                            //DEFINE A DIREÇÃO PARA BAIXO
    if (key[KEY_LEFT]) direcao = "left";                            //DEFINE A DIREÇÃO PARA A ESQUERDA
    if (key[KEY_RIGHT]) direcao = "right";                          //DEFINE A DIREÇÃO PARA A DIREITA
    

    //SE A DIREÇÃO FOR XX E TIVER ESPAÇO PARA ANDAR NESSA DIREÇÃO E NÃO ESTIVER NA TELA INICIAL
    //ELE ANDA NAQUELA DIREÇÃO
    if (direcao == "up" and MAPA_PACMAN[i-1][j] != '1' and MAPA_PACMAN[i-1][j] != '6' and !inicial)
        indo = "up";
    if (indo == "up" and MAPA_PACMAN[i-1][j] != '1' and MAPA_PACMAN[i-1][j] != '6' and !inicial and !andou){
        i--;
        posy = i*q;
        andou = true; ////////
    }

    if (direcao == "down" and MAPA_PACMAN[i+1][j] != '1' and MAPA_PACMAN[i-1][j] != '6' and !inicial)
        indo = "down";
    if (indo == "down" and MAPA_PACMAN[i+1][j] != '1' and MAPA_PACMAN[i-1][j] != '6' and !inicial and !andou){
        i++;
        posy = i*q;
        andou = true;/////////
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


//**FUNÇÃO FANTASMAS**//
void fantasma1(){
    int movimento = rand();
    if (movimento%4 == 1) direcaop = "right";
    if (movimento%4 == 2) direcaop = "left";
    if (movimento%4 == 3) direcaop = "up";
    if (movimento%4 == 0) direcaop = "down";
	
    
    //SE A DIREÇÃO FOR XX E TIVER ESPAÇO PARA ANDAR NESSA DIREÇÃO E NÃO ESTIVER NA TELA INICIAL
    //ELE ANDA NAQUELA DIREÇÃO
	
	if (direcaop == "up" and MAPA_PHANTOM[k - 1][l] != '1' and !inicial and ultimodir != "down") { //*// COLOQUEI UMA variavel para guardar qual foi a ultima direção, então o fatasminha so vai ir a direção x se a ultmia não for contraria a que ele  esta tentando ir
		indop = "up";
		ultimodir = indop;
	}
    if (indop == "up" and MAPA_PHANTOM[k-1][l] != '1' and !inicial and !andoup){
        k--;
        pposy1 = k*q;
        andoup = true; ////////
    }

	if (direcaop == "down" and MAPA_PHANTOM[k + 1][l] != '1' and !inicial and ultimodir != "up") {
		indop = "down";
		ultimodir = indop;
	}
    if (indop == "down" and MAPA_PHANTOM[k+1][l] != '1' and !inicial and !andoup){
        k++;
        pposy1 = k*q;
        andoup = true;/////////
    }
    
	if (direcaop == "left" and MAPA_PHANTOM[k][l - 1] != '1' and !inicial and ultimodir != "right") {
		indop = "left";
		ultimodir = indop;
	}
    if (indop == "left" and MAPA_PHANTOM[k][l-1] != '1' and !inicial and !andoup){
        l--;
        pposx1 = l*q;
        andoup = true;/////////////////
    }


	if (direcaop == "right" and MAPA_PHANTOM[k][l + 1] != '1' and !inicial and ultimodir != "left") {
		indop = "right";
		ultimodir = indop;
	}
    if (indop == "right" and MAPA_PHANTOM[k][l+1] != '1' and !inicial and !andoup){
        l++;
        pposx1 = l*q;
        andoup = true;///////////////////
    }

    //CASO DE PORTAL
    if (MAPA_PHANTOM[k][l] == '4' and !inicial and indo == "right"){
        l = 13;
        k = 23;
        pposx1 = l*q;
        pposy1 = k*q;
    }
    if (MAPA_PHANTOM[k][l] == '4' and !inicial and indo == "left"){
        l = 11;
        k = 23;
        pposx1 = l*q;
        pposy1 = k*q;
    }
    if (MAPA_PHANTOM[k][l] == '5' and !inicial and indo == "right"){
        l = 13;
        k = 3;
        pposx1 = l*q;
        pposy1 = k*q;
    }
    if (MAPA_PHANTOM[k][l] == '5' and !inicial and indo == "left"){
        l = 11;
        k = 3;
        pposx1 = l*q;
        pposy1 = k*q;
    }
    if (MAPA_PHANTOM[k][l] == '5' and !inicial and indo == "down"){
        l = 12;
        k = 3;
        pposx1 = l*q;
        pposy1 = k*q;
    } 
}
void fantasma2(){
    int movimento = rand();
    if (movimento%4 == 1) direcaop2 = "right";
    if (movimento%4 == 2) direcaop2 = "left";
    if (movimento%4 == 3) direcaop2 = "up";
    if (movimento%4 == 0) direcaop2 = "down";
    
    //SE A DIREÇÃO FOR XX E TIVER ESPAÇO PARA ANDAR NESSA DIREÇÃO E NÃO ESTIVER NA TELA INICIAL
    //ELE ANDA NAQUELA DIREÇÃO

	//*//
	if (direcaop2 == "up" and MAPA_PHANTOM[k2 - 1][l2] != '1' and !inicial and ultimodir2 != "down") {
		indop2 = "up";
		ultimodir2 = indop2;
	}
    if (indop2 == "up" and MAPA_PHANTOM[k2-1][l2] != '1' and !inicial and !andoup2){
        k2--;
        pposy2 = k2*q;
        andoup2 = true; ////////
    }

	if (direcaop2 == "down" and MAPA_PHANTOM[k2 + 1][l2] != '1' and !inicial and ultimodir2 != "up") {
		indop2 = "down";
		ultimodir2 = indop2;
	}
    if (indop2 == "down" and MAPA_PHANTOM[k2+1][l2] != '1' and !inicial and !andoup2){
        k2++;
        pposy2 = k2*q;
        andoup2 = true;/////////
    }
    
	if (direcaop2 == "left" and MAPA_PHANTOM[k2][l2 - 1] != '1' and !inicial and ultimodir2 != "right") {
		indop2 = "left";
		ultimodir2 = indop2;
	}
    if (indop2 == "left" and MAPA_PHANTOM[k2][l2-1] != '1' and !inicial and !andoup2){
        l2--;
        pposx2 = l2*q;
        andoup2 = true;/////////////////
    }


	if (direcaop2 == "right" and MAPA_PHANTOM[k2][l2 + 1] != '1' and !inicial and ultimodir2 != "left") {
		indop2 = "right";
		ultimodir2 = indop2;
	}
    if (indop2 == "right" and MAPA_PHANTOM[k2][l2+1] != '1' and !inicial and !andoup2){
        l2++;
        pposx2 = l2*q;
        andoup2 = true;///////////////////
    }

    //CASO DE PORTAL
    if (MAPA_PHANTOM[k2][l2] == '4' and !inicial and indop2 == "right"){
        l2 = 13;
        k2 = 23;
        pposx2 = l2*q;
        pposy2 = k2*q;
    }
    if (MAPA_PHANTOM[k2][l2] == '4' and !inicial and indop2 == "left"){
        l2 = 11;
        k2 = 23;
        pposx2 = l2*q;
        pposy2 = k2*q;
    }
    if (MAPA_PHANTOM[k2][l2] == '5' and !inicial and indop2 == "right"){
        l2 = 13;
        k2 = 3;
        pposx2 = l2*q;
        pposy2 = k2*q;
    }
    if (MAPA_PHANTOM[k2][l2] == '5' and !inicial and indop2 == "left"){
        l2 = 11;
        k2 = 3;
        pposx2 = l2*q;
        pposy2 = k2*q;
    }
    if (MAPA_PHANTOM[k2][l2] == '5' and !inicial and indop2 == "down"){
        l2 = 12;
        k2 = 3;
        pposx2 = l2*q;
        pposy2 = k2*q;
    } 
}
void fantasma3(){
    int movimento = rand();
    if (movimento%4 == 1) direcaop3 = "right";
    if (movimento%4 == 2) direcaop3 = "left";
    if (movimento%4 == 3) direcaop3 = "up";
    if (movimento%4 == 0) direcaop3 = "down";
    
    //SE A DIREÇÃO FOR XX E TIVER ESPAÇO PARA ANDAR NESSA DIREÇÃO E NÃO ESTIVER NA TELA INICIAL
    //ELE ANDA NAQUELA DIREÇÃO

	//*//
	if (direcaop3 == "up" and MAPA_PHANTOM[k3 - 1][l3] != '1' and !inicial and ultimodir3 != "down") {
		indop3 = "up";
		ultimodir3 = indop3;
	}
    if (indop3 == "up" and MAPA_PHANTOM[k3-1][l3] != '1' and !inicial and !andoup3){
        k3--;
        pposy3 = k3*q;
        andoup3 = true; ////////
    }

    if (direcaop3 == "down" and MAPA_PHANTOM[k3+1][l3] != '1' and !inicial and ultimodir3 != "up"){
        indop3 = "down";
		ultimodir3 = indop3;
	}
    if (indop3 == "down" and MAPA_PHANTOM[k3+1][l3] != '1' and !inicial and !andoup3){
        k3++;
        pposy3 = k3*q;
        andoup3 = true;/////////
    }
    
    if (direcaop3 == "left" and MAPA_PHANTOM[k3][l3-1] != '1' and !inicial and ultimodir3 != "right"){
        indop3 = "left";
		ultimodir3 = indop3;
	}
    if (indop3 == "left" and MAPA_PHANTOM[k3][l3-1] != '1' and !inicial and !andoup3){
        l3--;
        pposx3 = l3*q;
        andoup3 = true;/////////////////
    }


    if (direcaop3 == "right" and MAPA_PHANTOM[k3][l3+1] != '1' and !inicial and ultimodir3 != "left"){
        indop3 = "right";
		ultimodir3 = indop3;
	}
    if (indop3 == "right" and MAPA_PHANTOM[k3][l3+1] != '1' and !inicial and !andoup3){
        l3++;
        pposx3 = l3*q;
        andoup3 = true;///////////////////
    }

    //CASO DE PORTAL
    if (MAPA_PHANTOM[k3][l3] == '4' and !inicial and indop3 == "right"){
        l3 = 13;
        k3 = 23;
        pposx3 = l3*q;
        pposy3 = k3*q;
    }
    if (MAPA_PHANTOM[k3][l3] == '4' and !inicial and indop3 == "left"){
        l3 = 11;
        k3 = 23;
        pposx3 = l3*q;
        pposy3 = k3*q;
    }
    if (MAPA_PHANTOM[k3][l3] == '5' and !inicial and indop3 == "right"){
        l3 = 13;
        k3 = 3;
        pposx3 = l3*q;
        pposy3 = k3*q;
    }
    if (MAPA_PHANTOM[k3][l3] == '5' and !inicial and indop3 == "left"){
        l3 = 11;
        k3 = 3;
        pposx3 = l3*q;
        pposy3 = k3*q;
    }
    if (MAPA_PHANTOM[k3][l3] == '5' and !inicial and indop3 == "down"){
        l3 = 12;
        k3 = 3;
        pposx3 = l3*q;
        pposy3 = k3*q;
    } 
}
void fantasma4(){
    int movimento = rand();
    if (movimento%4 == 1) direcaop4 = "right";
    if (movimento%4 == 2) direcaop4 = "left";
    if (movimento%4 == 3) direcaop4 = "up";
    if (movimento%4 == 0) direcaop4 = "down";
    
    //SE A DIREÇÃO FOR XX E TIVER ESPAÇO PARA ANDAR NESSA DIREÇÃO E NÃO ESTIVER NA TELA INICIAL
    //ELE ANDA NAQUELA DIREÇÃO

	//*//
	if (direcaop4 == "up" and MAPA_PHANTOM[k4 - 1][l4] != '1' and !inicial and ultimodir4 != "down") {
		indop4 = "up";
		ultimodir4 = indop4;
	}
    if (indop4 == "up" and MAPA_PHANTOM[k4-1][l4] != '1' and !inicial and !andoup4){
        k4--;
        pposy4 = k4*q;
        andoup4 = true; ////////
    }

    if (direcaop4 == "down" and MAPA_PHANTOM[k4+1][l4] != '1' and !inicial and ultimodir4 != "up"){
        indop4 = "down";
		ultimodir4 = indop4;
	}

    if (indop4 == "down" and MAPA_PHANTOM[k4+1][l4] != '1' and !inicial and !andoup4){
        k4++;
        pposy4 = k4*q;
        andoup4 = true;/////////
    }
    
    if (direcaop4 == "left" and MAPA_PHANTOM[k4][l4-1] != '1' and !inicial and ultimodir4 != "right"){
        indop4 = "left";
		ultimodir4 = indop4;
	}
    if (indop4 == "left" and MAPA_PHANTOM[k4][l4-1] != '1' and !inicial and !andoup4){
        l4--;
        pposx4 = l4*q;
        andoup4 = true;/////////////////
    }


    if (direcaop4 == "right" and MAPA_PHANTOM[k4][l4+1] != '1' and !inicial and ultimodir4 != "left"){
        indop4 = "right";
		ultimodir4 = indop4;
	}
    if (indop4 == "right" and MAPA_PHANTOM[k4][l4+1] != '1' and !inicial and !andoup4){
        l4++;
        pposx4 = l4*q;
        andoup4 = true;///////////////////
    }

    //CASO DE PORTAL
    if (MAPA_PHANTOM[k4][l4] == '4' and !inicial and indop4 == "right"){
        l4 = 13;
        k4 = 23;
        pposx4 = l4*q;
        pposy4 = k4*q;
    }
    if (MAPA_PHANTOM[k4][l4] == '4' and !inicial and indop4 == "left"){
        l4 = 11;
        k4 = 23;
        pposx4 = l4*q;
        pposy4 = k4*q;
    }
    if (MAPA_PHANTOM[k4][l4] == '5' and !inicial and indop4 == "right"){
        l4 = 13;
        k4 = 3;
        pposx4 = l4*q;
        pposy4 = k4*q;
    }
    if (MAPA_PHANTOM[k4][l4] == '5' and !inicial and indop4 == "left"){
        l4 = 11;
        k4 = 3;
        pposx4 = l4*q;
        pposy4 = k4*q;
    }
    if (MAPA_PHANTOM[k4][l4] == '5' and !inicial and indop4 == "down"){
        l4 = 12;
        k4 = 3;
        pposx4 = l4*q;
        pposy4 = k4*q;
    } 
}

//**FUNÇÃO DO INÍCIO**//
void inicio(){
    al_draw_bitmap(splash_Screen, 0, 0, 0);
    graus += 5;
    if (graus > 360) graus = 0;
    al_draw_rotated_bitmap(portal, portalW / 2, portalH / 2, 250, 350, graus * 3.1415 / 180, 0);
    al_draw_textf(fonte_Misfits_2, al_map_rgb(255, 255, 255), SCREEN_W/2, SCREEN_H/2 + 40, ALLEGRO_ALIGN_CENTER, "APERTE ENTER");
    al_draw_textf(fonte_Misfits_3, al_map_rgb(255, 255, 255), SCREEN_W / 2, SCREEN_H / 2 + 80, ALLEGRO_ALIGN_CENTER, "PARA INICIAR");
}

//**FUNÇÃO DE CARREGAR E REMOVER ITENS**//
void itens() {

    al_draw_bitmap(mapa,0,0,0);

    // CARREGAR ITENS DO MAPA
    for (int a = 0; a < 26; a++)
        for (int b = 0; b < 26; b++) {
            if (MAPA_PACMAN[a][b] == '2') al_draw_bitmap(ball, (b * q) + 4, (a * q) + 6, 50);                  //CARREGA BOLA
            if (MAPA_PACMAN[a][b] == '3') al_draw_bitmap(pilula, (b * q) + 6, (a * q), 0);                     //CARREGA PILULA
            if (MAPA_PACMAN[a][b] == '4' or MAPA_PACMAN[a][b] == '5') al_draw_bitmap(pportal, (b * q), (a * q), 0);   //CARREGA OS PORTAIS
        }
    
	//DESENHA O PACMAN(MORTY)
	al_draw_bitmap(pacman_up, posx, posy, 0);
	if (indo == "up") al_draw_bitmap(pacman_up, posx, posy, 0);
    if (indo == "down") al_draw_bitmap(pacman_down,posx,posy,0);             
	if (indo == "left") al_draw_bitmap(pacman_left, posx, posy, 0);
	if (indo == "right") al_draw_bitmap(pacman_right, posx, posy, 0);


	//DESENHA OS FANTASMAS
    al_draw_bitmap(phantom1, pposx1, pposy1, 0);  
    al_draw_bitmap(phantom2, pposx2, pposy2, 0);
    al_draw_bitmap(phantom3, pposx3, pposy3, 0);
    al_draw_bitmap(phantom4, pposx4, pposy4, 0);

    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            // REMOVER ITENS DO MAPA
            if (i == (posy / 20) && j == (posx / 20) && MAPA_PACMAN[i][j] == '3') {
                MAPA_PACMAN[i][j] = '0';	//TIRA A PILULA DO MAPA
                pontos += 50;
            }
            if (i == (posy / 20) && j == (posx / 20) && MAPA_PACMAN[i][j] == '2') {
                MAPA_PACMAN[i][j] = '0';	//TIRA BOLINHA DO MAPA
                pontos += 10;	    //AUMENTA A PONTUAÇÃO
            }
        }
    }
    al_draw_textf(fonte_Misfits, al_map_rgb(65, 166, 50), 80, 515, ALLEGRO_ALIGN_CENTER, "%d PONTOS", pontos );
}

//**FUNÇÃO DE VERIFICAÇÃO DE VITORIA/DERROTA**//

void phant_derrota(string direcaop,int k,int l, int pposx1, int pposy1){
    if (posx == pposx1 and posy == pposy1){
        lose = true;
    }
    if (direcao == "right" and direcaop == "left"){
        if (posx == (pposx1+q) and posy == pposy1){
            lose = true;
        }
    }
    if (direcaop == "right" and direcao == "left"){
        if (posx == (pposx1-q) and posy == pposy1){
            lose = true;
        }
    }
    if (direcao == "up" and direcaop == "down"){
        if (posx == pposx1 and posy == (pposy1-q)){
            lose = true;
        }
    }
    if (direcaop == "up" and direcao == "down"){
        if (posx == pposx1 and posy == (pposy1+q)){
            lose = true;
        }
    }
}

void verifica(){
    if (pontos == 2640){
        win = true;
    }
    phant_derrota(direcaop,k,l,pposx1,pposy1);
    phant_derrota(direcaop2, k2, l2, pposx2, pposy2);
    phant_derrota(direcaop3, k3, l3, pposx3, pposy3);
    phant_derrota(direcaop4, k4, l4, pposx4, pposy4);
    
}

void fimdejogo(){
    if (win){
		final = true;
        al_draw_bitmap(ganhou, 0, 0, 0); //TROCAR PARA TELA DE VITÓRIA

    }

    if (lose){
		final = true;
        al_draw_bitmap(perdeu, 0, 0, 0); //TROCAR PARA TELA DE DERROTA
    }

}

//FUNÇÃO PRINCIPAL
int main(int argc, char **argv){
    if(!inicializa()) return -1; //SE NÃO INICIALIZAR, RETORNA -1 (ERRO)

	al_play_sample_instance(backgroundMusica_instance); // MÚSICA EM LOOP

    while(!sair){

        andou = false;
        andoup = false;
        andoup2 = false;
        andoup3 = false;
        andoup4 = false;
        
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_TIMER){   
            fimdejogo();
            teclado();
            verifica();
            fantasma1();
            if (cont>=20) fantasma2();
            if (cont >=50) fantasma3();
            if (cont >=70) fantasma4();
            cont++;
        }if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
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

			al_clear_to_color(al_map_rgb(0, 0, 0));
			if (fim == true && contFim < 30) {
				contFim++;
				//cout << contFim << endl;
				fimdejogo();
			}
			if (contFim >= 30) {
				inicial = true;
				contFim = 0;
			}
   
            if (inicial == true){
				inicio();
				fim = false;
            }else {
                itens();
                fimdejogo();
            }
			if (final == true) fim = true;

            al_flip_display();
        }
    }

	// DESTRUIR BITMAP
    al_destroy_bitmap(mapa);
	al_destroy_bitmap(splash_Screen);
    al_destroy_bitmap(pacman_up);
	al_destroy_bitmap(pacman_down);
	al_destroy_bitmap(pacman_left);
	al_destroy_bitmap(pacman_right);
	al_destroy_bitmap(ball);

	al_destroy_bitmap(pilula);
	al_destroy_bitmap(portal);

	al_destroy_bitmap(perdeu);
	al_destroy_bitmap(ganhou);

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