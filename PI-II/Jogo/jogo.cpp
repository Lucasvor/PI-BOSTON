//Inicialização das bibliotecas
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

//Definição de constantes (Largura, Altura, FPS e Quantidade de Números)
#define LARG 1280
#define ALT 720
#define FPS 60
#define QUANT_NUMEROS 50
#define QUANT_PALAVRAS 300

using namespace std;

typedef struct Caixa{
    int ID;
    int x;
    int y;
    int vidas;
    int velocidade;
    int limitex;
    int limitey;
    int pontuacao;
}CaixaC;

typedef struct Numero{
    int ID;
    int x;
    int y;
    bool vivo;
    int velocidade;
    int limitey;
    int limitex;
    int valor;
}Numeros;

typedef struct Palavra{
    int x;
    int y;
    bool vivo;
    int velocidade;
    int limitey;
    int limitex;
    int ID;
    string palavra;
    int classificacao;
}Palavras;

enum TECLAS{ESQUERDA,DIREITA,ESQUERDAP,DIREITAP};
bool teclas[4] = {false, false, false, false};

void PaineldeInfo(int acertos, int nivel, CaixaC caixa, ALLEGRO_FONT *fonte);

void IniciarCaixa(CaixaC *caixa,int nivel);
void DesenharCaixa(CaixaC caixa, ALLEGRO_FONT *fonte, int nivel);
void MoverCaixaEsquerda(CaixaC *caixa,int nivel);
void MoverCaixaDireita(CaixaC *caixa,int nivel);

void IniciarCaixaP(CaixaC *caixa,int nivel);
void DesenharCaixaP(CaixaC caixa, ALLEGRO_FONT *fonte, int nivel);
void MoverCaixaEsquerdaP(CaixaC *caixa,int nivel);
void MoverCaixaDireitaP(CaixaC *caixa,int nivel);

void ColisaoNumeros(Numeros numero[], int tamanhoN, CaixaC *caixa, int nivel, int *acertos);
void IniciarNumero(Numeros numero[], int tamanho);
void DesenharNumero(Numeros numero[], int tamanho, ALLEGRO_FONT *fonte);
void ComecarNumero(Numeros numero[], int tamanho, int n, int nivel);
void AtualizarNumero(Numeros numero[], int tamanho);

void DefinirPalavras(Palavras palavra[]);
void ColisaoPalavras(Palavras palavra[], int tamanhoN, CaixaC *caixa,CaixaC *caixan, int nivel, int *acertos);
void IniciarPalavra(Palavras palavra[], int tamanho);
void DesenharPalavra(Palavras palavra[], int tamanho, ALLEGRO_FONT *fonte);
void ComecarPalavra(Palavras palavra[], int tamanho, int n, int nivel);
void AtualizarPalavra(Palavras palavra[], int tamanho);

bool primos(Numeros numero[], int i);
bool quadperfeito(Numeros numero[], int i);

int main(int argc, char **argv){
    bool fim_do_jogo = false;
    int nivel = 1;
    int n = 0;
	int acertos=0;
	int Jogo = 0;
    bool isGameOver = false;
    int regiao = 0;

    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
    ALLEGRO_FONT *fonte = NULL;

    CaixaC caixa;
    CaixaC caixa2;
    Numeros numero[QUANT_NUMEROS];
    Palavras palavra[QUANT_PALAVRAS];
    DefinirPalavras(palavra);

    if(!al_init()){
        fprintf(stderr, "Falha ao iniciar Allegro5!\n");
        return -1;
    }

    janela = al_create_display(LARG,ALT);
    if(!janela){
        fprintf(stderr, "Falha ao criar janela!\n");
        return -1;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer){
        fprintf(stderr, "Falha ao criar timer!\n");
        return -1;
    }

    al_init_primitives_addon();
    al_init_image_addon();
    imagem = al_load_bitmap("fase1.png");
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_mouse();
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

    srand(time(NULL));

    fila_de_eventos = al_create_event_queue();
    if(!fila_de_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos!\n");
        return -1;
    }

    fonte = al_load_font("BEBAS.ttf", 18, 0);
    if(!fonte){
        fprintf(stderr, "Falha ao carregar fonte!\n");
        return -1;
    }
    al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_de_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_de_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_de_eventos, al_get_mouse_event_source());

    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

    al_start_timer(timer);
    ALLEGRO_EVENT evento;
    while (!fim_do_jogo){
    	if(Jogo == 0){
    		ALLEGRO_BITMAP *Menu = NULL, *Menu1 = NULL,*Menu2 = NULL,*Menu3 = NULL,*Sair = NULL;
    		Menu = al_load_bitmap("Menu.jpg");
    		Menu1= al_load_bitmap("Menu1.jpg");
    		Menu2= al_load_bitmap("Menu2.jpg");
   			Menu3= al_load_bitmap("Menu3.jpg");
   		 	Sair = al_load_bitmap("Sair.jpg");
   		 	while(Jogo == 0){
                switch(regiao){
                case 4:
                    al_draw_bitmap(Sair,0,0,0);
                    break;
                case 3:
                    al_draw_bitmap(Menu3,0,0,0);
                    break;
                case 2:
                    al_draw_bitmap(Menu2,0,0,0);
                    break;
                case 1:
                    al_draw_bitmap(Menu1,0,0,0);
                    break;
                case 0:
                    al_draw_bitmap(Menu,0,0,0);
                    break;
                }

                al_flip_display();
                al_wait_for_event(fila_de_eventos, &evento);
                if(evento.type == ALLEGRO_EVENT_MOUSE_AXES){
                    if(evento.mouse.x >= 500 && evento.mouse.x <= 830 &&
                       evento.mouse.y >= 220 && evento.mouse.y <= 370)
                        regiao = 1;
                    else if(evento.mouse.x >= 370 && evento.mouse.x <= 920 &&
                            evento.mouse.y >= 370 && evento.mouse.y <= 460)
                        regiao = 2;
                    else if(evento.mouse.x >= 390 && evento.mouse.x <= 920 &&
                            evento.mouse.y >= 460 && evento.mouse.y <= 590)
                        regiao = 3;
                    else if(evento.mouse.x >= 1070 && evento.mouse.x <= 1270 &&
                            evento.mouse.y >= 10 && evento.mouse.y <= 150)
                        regiao = 4;
                    else
                        regiao = 0;

                }
                if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if(regiao == 4){
                        fim_do_jogo = true;
                        break;
                    }
                    if (regiao == 1){
                        nivel = 1;
                        acertos = 0;
                        Jogo = 2;
                        IniciarCaixa(&caixa,nivel);
                        IniciarCaixaP(&caixa2,nivel);
                    }
                }
                if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                    fim_do_jogo = true;
                    break;
                }
            }
            al_destroy_bitmap(Menu);
            al_destroy_bitmap(Menu1);
            al_destroy_bitmap(Menu2);
            al_destroy_bitmap(Menu3);
            al_destroy_bitmap(Sair);
            al_clear_to_color(al_map_rgb(0,0,0));
            al_flip_display();
    	}
    	if(Jogo == 1){
            ALLEGRO_EVENT evento;
        al_wait_for_event(fila_de_eventos, &evento);
        if(!isGameOver){
            if(caixa.vidas <= 0){
                isGameOver = true;
            }
        }
        if(evento.type == ALLEGRO_EVENT_TIMER){
			if(acertos == 10){
				nivel++;
				acertos = 0;
				Jogo = 2;
                switch(nivel){
                case 2:
                    imagem = al_load_bitmap("fase2.jpg");
                    break;
                case 3:
                    imagem = al_load_bitmap("fase3.jpg");
                    break;
                case 4:
                    imagem = al_load_bitmap("fase4.jpg");
                    break;
                case 5:
                    imagem = al_load_bitmap("fase5.jpg");
                    break;
                case 6:
                    imagem = al_load_bitmap("fase6.jpg");
                    break;
                case 7:
                    imagem = al_load_bitmap("fase7.jpg");
                    break;
                }
			}
			if(!isGameOver){
				al_draw_bitmap(imagem, 0,0,0);
                PaineldeInfo(acertos, nivel, caixa, fonte);

				if(nivel!=2){
                    DesenharCaixa(caixa, fonte, nivel);
                    ComecarNumero(numero, QUANT_NUMEROS, n, nivel);
                    AtualizarNumero(numero, QUANT_NUMEROS);
                    ColisaoNumeros(numero, QUANT_NUMEROS, &caixa, nivel, &acertos);
                    DesenharNumero(numero, QUANT_NUMEROS, fonte);
                }

                if(nivel!=1){
                    DesenharCaixaP(caixa2, fonte, nivel);
                    ComecarPalavra(palavra, QUANT_PALAVRAS, n, nivel);
                    AtualizarPalavra(palavra, QUANT_PALAVRAS);
                    ColisaoPalavras(palavra, QUANT_PALAVRAS, &caixa2, &caixa, nivel, &acertos);
                    DesenharPalavra(palavra, QUANT_PALAVRAS, fonte);
                }

                if(nivel>=3) al_draw_filled_rectangle((LARG/2)-5,0, (LARG/2)+5, ALT, al_map_rgb(255,0,0));

                al_flip_display();
			}
            if(teclas[ESQUERDA] && !isGameOver){
                MoverCaixaEsquerda(&caixa,nivel);
            }
            if(teclas[DIREITA] && !isGameOver){
                MoverCaixaDireita(&caixa,nivel);
            }
            if(teclas[ESQUERDAP] && !isGameOver){
                MoverCaixaEsquerdaP(&caixa2,nivel);
            }
            if(teclas[DIREITAP] && !isGameOver){
                MoverCaixaDireitaP(&caixa2,nivel);
            }
        }else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            fim_do_jogo = true;
        }else if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(evento.keyboard.keycode){
            case ALLEGRO_KEY_ESCAPE:
                fim_do_jogo = true;
                break;
            case ALLEGRO_KEY_LEFT:
                if(!isGameOver){
                teclas[ESQUERDA] = true;
                }
                break;
            case ALLEGRO_KEY_RIGHT:
                if(!isGameOver){
                teclas[DIREITA] = true;
                }
                break;
            case ALLEGRO_KEY_A:
                if(!isGameOver){
                teclas[ESQUERDAP] = true;
                }
                break;
            case ALLEGRO_KEY_D:
                if(!isGameOver){
                teclas[DIREITAP] = true;
                }
                break;
            case ALLEGRO_KEY_K:
                if(!isGameOver){
                acertos=10;
                }
                break;
            }
        }else if(evento.type == ALLEGRO_EVENT_KEY_UP){
            switch(evento.keyboard.keycode){
            case ALLEGRO_KEY_LEFT:
                teclas[ESQUERDA] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                teclas[DIREITA] = false;
                break;
            case ALLEGRO_KEY_A:
                teclas[ESQUERDAP] = false;
                break;
            case ALLEGRO_KEY_D:
                teclas[DIREITAP] = false;
                break;
            }
        }
        if(isGameOver && al_is_event_queue_empty(fila_de_eventos)){
            if(isGameOver){
                al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+80, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Game  Over!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pontos:  %i", caixa.pontuacao);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ESC  para  sair.");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+50, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  voltar  ao  menu.");
                if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
                    switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_PAD_ENTER:
                        Jogo = 0;
                        isGameOver = false;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        Jogo = 0;
                        isGameOver = false;
                        break;
                    }
                }
                if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                    fim_do_jogo = true;
                    break;
                }
                al_flip_display();
            }
        }
        n++;
        }
        if(Jogo == 2){
            switch(nivel){
            case 1:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+50, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Numeros Pares", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  NUMEROS  PARES!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 2:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+50, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Substantivos", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  SUBSTANTIVOS!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 3:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+50, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Substantivos", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  SUBSTANTIVOS!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 4:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+50, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Numeros Pares", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  NUMEROS  PARES!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 5:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+50, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Substantivos", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  SUBSTANTIVOS!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 6:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+50, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Substantivos", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  SUBSTANTIVOS!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            }
            al_wait_for_event(fila_de_eventos, &evento);
            if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(evento.keyboard.keycode){
                case ALLEGRO_KEY_PAD_ENTER:
                    Jogo = 1;
                    break;
                case ALLEGRO_KEY_ENTER:
                    Jogo = 1;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    fim_do_jogo = true;
                    break;
                }
            }
            if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                fim_do_jogo = true;
                break;
            }
            IniciarNumero(numero,QUANT_NUMEROS);
            IniciarPalavra(palavra,QUANT_PALAVRAS);
            if(nivel>2){
                caixa.x = (LARG / 4) * 3;
                caixa2.x = LARG / 4;
            }
        }
    }
    al_destroy_display(janela);
    al_destroy_bitmap(imagem);
    return 0;
}

//Parâmetros da Caixa e CaixaP(alavra)
void IniciarCaixa(CaixaC *caixa, int nivel){
    caixa->x = LARG / 2;
    caixa->y = ALT - 10;
    caixa->ID = 12;
    caixa->vidas = 3;
    caixa->velocidade = 9;
    caixa->limitex = 45;
    caixa->limitey = 50;
    caixa->pontuacao = 0;
}
void IniciarCaixaP(CaixaC *caixa, int nivel){
    caixa->x = LARG / 2;
    caixa->y = ALT - 10;
    caixa->ID = 12;
    caixa->vidas = 3;
    caixa->velocidade = 9;
    caixa->limitex = 45;
    caixa->limitey = 50;
    caixa->pontuacao = 0;
}
void DesenharCaixa(CaixaC caixa, ALLEGRO_FONT *fonte, int nivel){

    al_draw_filled_rectangle(caixa.x-50,caixa.y -50, caixa.x + 50, caixa.y, al_map_rgb(142,107,35));
    al_draw_filled_rectangle(caixa.x-70,caixa.y -50, caixa.x - 40, caixa.y -55, al_map_rgb(142,107,35));
    al_draw_filled_rectangle(caixa.x+70,caixa.y -50, caixa.x + 40, caixa.y -55, al_map_rgb(142,107,35));
    al_draw_rectangle(caixa.x-50,caixa.y -50, caixa.x + 50, caixa.y, al_map_rgb(0,0,0),1);
    al_draw_rectangle(caixa.x-70,caixa.y -50, caixa.x - 40, caixa.y -55, al_map_rgb(0,0,0),1);
    al_draw_rectangle(caixa.x+70,caixa.y -50, caixa.x + 40, caixa.y -55, al_map_rgb(0,0,0),1);

    //PS: Nivel 1 - Pares
        //    Nivel 2 - Substantivos
        //    Nivel 3 - Numerais/Divisiveis por 5
        //    Nivel 4 - Adjetivos/Divisiveis por 3
        //    Nivel 5 - Preposições/Primos
        //    Nivel 6 - Verbos/Divisiveis por 7
        //    Nivel 7 - Adverbios/Quadrados Perfeitos
    switch(nivel){
    case 1:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "PARES");
        break;
    case 3:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "/ POR 5");
        break;
    case 4:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "/ POR 3");
        break;
    case 5:
		al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "PRIMOS");
        break;
    case 6:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "/ POR 7");
        break;
	case 7:
		al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-50, ALLEGRO_ALIGN_CENTRE, "QUADRADOS");
		al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-25, ALLEGRO_ALIGN_CENTRE, "PERFEITOS");
        break;
    }
}
void DesenharCaixaP(CaixaC caixa, ALLEGRO_FONT *fonte, int nivel){
    al_draw_filled_rectangle(caixa.x-50,caixa.y -50, caixa.x + 50, caixa.y, al_map_rgb(142,107,35));
    al_draw_filled_rectangle(caixa.x-70,caixa.y -50, caixa.x - 40, caixa.y -55, al_map_rgb(142,107,35));
    al_draw_filled_rectangle(caixa.x+70,caixa.y -50, caixa.x + 40, caixa.y -55, al_map_rgb(142,107,35));
    al_draw_rectangle(caixa.x-50,caixa.y -50, caixa.x + 50, caixa.y, al_map_rgb(0,0,0),1);
    al_draw_rectangle(caixa.x-70,caixa.y -50, caixa.x - 40, caixa.y -55, al_map_rgb(0,0,0),1);
    al_draw_rectangle(caixa.x+70,caixa.y -50, caixa.x + 40, caixa.y -55, al_map_rgb(0,0,0),1);

        //PS: Nivel 1 - Pares
        //    Nivel 2 - Substantivos
        //    Nivel 3 - Numerais/Divisiveis por 5
        //    Nivel 4 - Adjetivos/Divisiveis por 3
        //    Nivel 5 - Preposições/Primos
        //    Nivel 6 - Verbos/Divisiveis por 7
        //    Nivel 7 - Adverbios/Quadrados Perfeitos
    switch(nivel){
    case 2:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "SUBSTANTIVOS");
        break;
    case 3:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "NUMERAIS");
        break;
    case 4:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "ADJETIVOS");
        break;
    case 5:
		al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "PREPOSICOES");
        break;
    case 6:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "VERBOS");
        break;
	case 7:
		al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-38, ALLEGRO_ALIGN_CENTRE, "ADVERBIOS");
        break;
    }
}
void MoverCaixaEsquerda(CaixaC *caixa,int nivel){
    caixa->x -= caixa->velocidade;
    if(nivel==1){
        if(caixa->x<50){
            caixa->x = 50;
        }
    }else{
        if(caixa->x<(LARG/2)+60){
            caixa->x = (LARG/2)+60;
        }
    }
}
void MoverCaixaDireita(CaixaC *caixa,int nivel){
    caixa->x += caixa->velocidade;
    if(caixa->x>LARG-50){
        caixa->x = LARG-50;
    }
}
void MoverCaixaEsquerdaP(CaixaC *caixa,int nivel){
    caixa->x -= caixa->velocidade;
    if(caixa->x<50){
        caixa->x = 50;
    }
}
void MoverCaixaDireitaP(CaixaC *caixa,int nivel){
    caixa->x += caixa->velocidade;
    if(nivel==2){
        if(caixa->x>LARG-50){
            caixa->x = LARG-50;
        }
    }else{
        if(caixa->x>(LARG/2)-60){
            caixa->x = (LARG/2)-60;
        }
    }
}

//Parâmetros dos Números
void IniciarNumero(Numeros numero[], int tamanho){
    int i;
    for(i = 0; i < tamanho; i++){
        numero[i].ID = rand();
        numero[i].vivo = false;
        numero[i].velocidade = 4;
        numero[i].limitex = 18;
        numero[i].limitey = 18;
    }
}
void DesenharNumero(Numeros numero[], int tamanho, ALLEGRO_FONT *fonte){
    int i;
    for(i = 0; i < tamanho; i++){
        if(numero[i].vivo){
            al_draw_filled_circle(numero[i].x, numero[i].y, 20, al_map_rgb(255,0,0));
            al_draw_textf(fonte, al_map_rgb(255,255,255), numero[i].x,numero[i].y-10, ALLEGRO_ALIGN_CENTRE, "%i", numero[i].valor);
        }
    }
}
void ComecarNumero(Numeros numero[], int tamanho, int n, int nivel){
    int i;
    for(i = 0; i < tamanho; i++){
        if(!numero[i].vivo){
            if(n % 100 == 0){
                numero[i].vivo = true;
                if(nivel<3){
                    numero[i].x = 30 + rand() % (LARG - 60);
                    numero[i].y = 0;
                }else{
                    numero[i].x = ((LARG/2)+30) + rand() % (LARG - 60);
                    numero[i].y = 0;
                }
                switch(nivel){
                    case 5:
                    numero[i].valor = rand() % 70;
                    break;
                    default:
                    numero[i].valor = rand() % 1000;
                }
                break;
            }
        }
    }
}
void AtualizarNumero(Numeros numero[], int tamanho){
    int i;
    for(i=0; i<tamanho;i++){
        if(numero[i].vivo){
            numero[i].y += numero[i].velocidade;
        }
    }
}

//Colisão
void ColisaoNumeros(Numeros numero[], int tamanhoN, CaixaC *caixa, int nivel, int *acertos){
    bool colisaoa = false;
    bool colisaob = false;
    int i;
    for(i = 0; i < tamanhoN;i++){
        if(numero[i].vivo){
            if(numero[i].x - numero[i].limitex < caixa->x + caixa->limitex &&
               numero[i].x + numero[i].limitex > caixa->x - caixa->limitex &&
               numero[i].y - numero[i].limitey < caixa->y + caixa->limitey &&
               numero[i].y + numero[i].limitey > caixa->y - caixa->limitey){
                colisaoa = true;
                numero[i].vivo = false;
            }else if(numero[i].y == ALT){
                numero[i].vivo = false;
                colisaob = true;
			}
        }

        //PS: Nivel 1 - Pares
        //    Nivel 2 - Substantivos
        //    Nivel 3 - Numerais/Divisiveis por 5
        //    Nivel 4 - Adjetivos/Divisiveis por 3
        //    Nivel 5 - Preposições/Primos
        //    Nivel 6 - Verbos/Divisiveis por 7
        //    Nivel 7 - Adverbios/Quadrados Perfeitos
        switch(nivel){
        case 1:
            if(colisaoa){
                if(numero[i].valor % 2 == 0){
                    caixa->pontuacao += 10;
                    colisaoa = false;
				    *acertos+=1;
                }else{
                    caixa->vidas--;
                    if(caixa->pontuacao > 0)
                        caixa->pontuacao -= 10;
                    colisaoa = false;
                }
            }else if(colisaob){
                if(numero[i].valor % 2 == 0){
                    if(caixa->pontuacao > 0)
                        caixa->pontuacao -= 10;
                    caixa->vidas--;
                    colisaob = false;
                }else{
                    colisaob = false;
                }
            }
        break;
        case 2:
            if(colisaoa){
                if(numero[i].valor % 5 == 0){
                    caixa->pontuacao += 12;
                    colisaoa = false;
				    *acertos+=1;
                }else{
                    caixa->vidas--;
                    if(caixa->pontuacao > 0)
                        caixa->pontuacao -= 8;
                    colisaoa = false;
                }
            }else if(colisaob){
                if(numero[i].valor % 5 == 0){
                    if(caixa->pontuacao > 0)
                        caixa->pontuacao -= 8;
                    caixa->vidas--;
                    colisaob = false;
                }else{
                    colisaob = false;
                }
            }
        break;
        case 3:
			if(colisaoa){
				if(numero[i].valor % 3 == 0){
					caixa->pontuacao += 20;
					colisaoa=false;
					*acertos+=1;
				}else{
					caixa->vidas--;
					if(caixa->pontuacao > 0)
                        caixa->pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(numero[i].valor % 3 == 0){
					if(caixa->pontuacao > 0)
                        caixa->pontuacao -= 5;
					caixa->vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        case 4:
			if(colisaoa){
				if(primos(numero, i)){
					caixa->pontuacao += 15;
					colisaoa=false;
					*acertos+=1;
				}else{
					caixa->vidas--;
					if(caixa->pontuacao > 0)
                        caixa->pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(primos(numero, i)){
                    if(caixa->pontuacao > 0)
                        caixa->pontuacao -= 5;
					caixa->vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        case 5:
			if(colisaoa){
				if(numero[i].valor % 7 == 0){
					caixa->pontuacao += 20;
					colisaoa=false;
					*acertos+=1;
				}else{
					caixa->vidas--;
					if(caixa->pontuacao > 0)
                        caixa->pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(numero[i].valor % 7 == 0){
                    if(caixa->pontuacao > 0)
                        caixa->pontuacao -= 5;
					caixa->vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        case 6:
			if(colisaoa){
				if(quadperfeito(numero, i)){
					caixa->pontuacao += 25;
					colisaoa=false;
					*acertos+=1;
				}else{
					caixa->vidas--;
					if(caixa->pontuacao > 0)
                        caixa->pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(quadperfeito(numero, i)){
					if(caixa->pontuacao > 0)
                        caixa->pontuacao -= 5;
					caixa->vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        }
    }
}

bool primos(Numeros numero[], int i){
	int quantdiv = 0;
	int abc = 2;
	for(abc=2; abc < 100; abc++){
		if(numero[i].valor % abc == 0){
			quantdiv++;
		}
	}
	if(quantdiv>1){
		return false;
	}else{
		return true;
	}
}
bool quadperfeito(Numeros numero[], int i){
    int n,t,m;
    for(n=1;n<32;n+=2){
    m=n;
    t = 0;
        while(m>=1){
            t = t + m;
            m = m - 2;
        }
        if(numero[i].valor == t){
            return true;
        }
    }
    return false;
}

void PaineldeInfo(int acertos, int nivel, CaixaC caixa, ALLEGRO_FONT *fonte){
    al_draw_filled_rectangle(3,3, 120, 110, al_map_rgb(0,0,0));
    al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 5, 0, "Nivel  %i", nivel);
    al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 30, 0, "Vidas:  %i",caixa.vidas);
    al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 55, 0, "Pontos:  %i",caixa.pontuacao);
    al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 80, 0, "Acertos:  %i/10",acertos);
}

void DefinirPalavras(Palavras palavra[]){
    int i;
    for(i=0;i<QUANT_PALAVRAS;i++){
        palavra[i].ID=i;
        if(i>=0 && i<50) palavra[i].classificacao = 1;
        if(i>=50 && i<100) palavra[i].classificacao = 2;
        if(i>=100 && i<150) palavra[i].classificacao = 3;
        if(i>=150 && i<200) palavra[i].classificacao = 4;
        if(i>=200 && i<250) palavra[i].classificacao = 5;
        if(i>=250 && i<300) palavra[i].classificacao = 6;
    }

    //Definir as palavras:
    // 0 - 49:      Substantivos (Classificacao: 1)
    // 50 - 99:     Numerais     (Classificacao: 2)
    // 100 - 149:   Adjetivos    (Classificacao: 3)
    // 150 - 199:   Preposições  (Classificacao: 4)
    // 200 - 249:   Verbos       (Classificacao: 5)
    // 250 - 299:   Adverbios    (Classificacao: 6)

    //SOMENTE PALAVRAS COM >>DEZ<< LETRAS OU MENOS.

    //SINTAXE: palavra[ID].palavra = "palavra";
    //Ex://SUBSTANTIVOS
    /*Substantivos são palavras que nomeiam seres, lugares, qualidades,
    sentimentos, noções, entre outros. Podem ser flexionados em gênero
    (masculino e feminino), número (singular e plural)
    e grau (diminutivo, normal, aumentativo).*/

    palavra[0].palavra = "Amor";
    palavra[1].palavra = "Abacaxi";
    palavra[2].palavra = "Bola";
    palavra[3].palavra = "Baleia";
    palavra[4].palavra = "Cachorro";
    palavra[5].palavra = "Cebola";
    palavra[6].palavra = "Dado";
    palavra[7].palavra = "Doce";
    palavra[8].palavra = "Elefante";
    palavra[9].palavra = "Estrela";
    palavra[10].palavra = "Faca";
    palavra[11].palavra = "Folha";
    palavra[12].palavra = "Goiaba";
    palavra[13].palavra = "Geladeira";
    palavra[14].palavra = "Homem";
    palavra[15].palavra = "Igreja";
    palavra[16].palavra = "Ilha";
    palavra[17].palavra = "Joelho";
    palavra[18].palavra = "Janela";
    palavra[19].palavra = "Kart";
    palavra[20].palavra = "Laranja";
    palavra[21].palavra = "Lagarto";
    palavra[22].palavra = "Macaco";
    palavra[23].palavra = "Mesa";
    palavra[24].palavra = "Nave";
    palavra[25].palavra = "Nariz";
    palavra[26].palavra = "Oceano";
    palavra[27].palavra = "Orelha";
    palavra[28].palavra = "Pai";
    palavra[29].palavra = "Palmeira";
    palavra[30].palavra = "Quadrado";
    palavra[31].palavra = "Quadra";
    palavra[32].palavra = "Roda";
    palavra[33].palavra = "Rádio";
    palavra[34].palavra = "Sabonete";
    palavra[35].palavra = "Sal";
    palavra[36].palavra = "Tatuagem";
    palavra[37].palavra = "Taxi";
    palavra[38].palavra = "Universo";
    palavra[39].palavra = "Urso";
    palavra[40].palavra = "Vaca";
    palavra[41].palavra = "Vestido";
    palavra[42].palavra = "Xadrez";
    palavra[43].palavra = "Xerife";
    palavra[44].palavra = "Zembra";
    palavra[45].palavra = "Zumbi";
    palavra[46].palavra = "Bolha";
    palavra[47].palavra = "Avião";
    palavra[48].palavra = "Batata";
    palavra[49].palavra = "Pedra";


    //NUMERAIS
    /*Numeral é a palavra que indica os seres em termos numéricos, isto é,
    que atribui quantidade aos seres ou os situa em determinada sequência.*/

    palavra[50].palavra = "Um";
    palavra[51].palavra = "1";
    palavra[52].palavra = "Primeiro";
    palavra[53].palavra = "Segundo";
    palavra[54].palavra = "2";
    palavra[55].palavra = "dois";
    palavra[56].palavra = "Duplo";
    palavra[57].palavra = "Dobro";
    palavra[58].palavra = "Metade";
    palavra[59].palavra = "Meio";
    palavra[60].palavra = "Dupla";
    palavra[61].palavra = "Terceiro";
    palavra[62].palavra = "3";
    palavra[63].palavra = "Triplo";
    palavra[64].palavra = "Trio";
    palavra[65].palavra = "Terço";
    palavra[66].palavra = "4";
    palavra[67].palavra = "Quatro";
    palavra[68].palavra = "Quarto";
    palavra[69].palavra = "Quarteto";
    palavra[70].palavra = "Sete";
    palavra[71].palavra = "Sétimo";
    palavra[72].palavra = "9";
    palavra[73].palavra = "8";
    palavra[74].palavra = "Oitavo";
    palavra[75].palavra = "12";
    palavra[76].palavra = "Doze";
    palavra[77].palavra = "Dúzia";
    palavra[78].palavra = "1000";
    palavra[79].palavra = "Mil";
    palavra[80].palavra = "Novecentos";
    palavra[81].palavra = "Milésimo";
    palavra[82].palavra = "Centésimo";
    palavra[83].palavra = "Milhar";
    palavra[84].palavra = "Trigésimo";
    palavra[85].palavra = "60";
    palavra[86].palavra = "Sessenta";
    palavra[87].palavra = "100";
    palavra[88].palavra = "Cem";
    palavra[89].palavra = "Centena";
    palavra[90].palavra = "Milhão";
    palavra[91].palavra = "200";
    palavra[92].palavra = "10000";
    palavra[93].palavra = "Bilhonėsimo";
    palavra[94].palavra = "Milionésimo";
    palavra[95].palavra = "Trilionésimo";
    palavra[96].palavra = "32";
    palavra[97].palavra = "700";
    palavra[98].palavra = "Setencentos";
    palavra[99].palavra = "Quinhentos";

    //ADJETIVOS
    /*Adjetivos são palavras que caracterizam um substantivo, conferindo-lhe
    uma qualidade, característica, aspecto ou estado. Os adjetivos variam em gênero
    (masculino e feminino) e em número (singular e plural) conforme o
    substantivo que caracterizam.*/

    palavra[100].palavra = "Grande";
    palavra[101].palavra = "Bom";
    palavra[102].palavra = "Novo";
    palavra[103].palavra = "Pequeno";
    palavra[104].palavra = "Velho";
    palavra[105].palavra = "Alto";
    palavra[106].palavra = "Estranho";
    palavra[107].palavra = "Enorme";
    palavra[108].palavra = "Escuro";
    palavra[109].palavra = "Mau";
    palavra[110].palavra = "Antigo";
    palavra[111].palavra = "Bonito";
    palavra[112].palavra = "Forte";
    palavra[113].palavra = "Fraco";
    palavra[114].palavra = "Pobre";
    palavra[115].palavra = "Sério";
    palavra[116].palavra = "Bravo";
    palavra[117].palavra = "Feliz";
    palavra[118].palavra = "Magro";
    palavra[119].palavra = "Calmo";
    palavra[120].palavra = "Gordo";
    palavra[121].palavra = "Alegre";
    palavra[122].palavra = "Nervoso";
    palavra[123].palavra = "Curioso";
    palavra[124].palavra = "Lindo";
    palavra[125].palavra = "Infeliz";
    palavra[126].palavra = "Elegante";
    palavra[127].palavra = "Engraçado";
    palavra[128].palavra = "Cruel";
    palavra[129].palavra = "Rígido";
    palavra[130].palavra = "Moreno";
    palavra[131].palavra = "Loiro";
    palavra[132].palavra = "Loira";
    palavra[133].palavra = "Ruiva";
    palavra[134].palavra = "Frio";
    palavra[135].palavra = "Vazio";
    palavra[136].palavra = "Fino";
    palavra[137].palavra = "Grosso";
    palavra[138].palavra = "Seco";
    palavra[139].palavra = "Rico";
    palavra[140].palavra = "Violento";
    palavra[141].palavra = "Quieto";
    palavra[142].palavra = "Divertido";
    palavra[143].palavra = "Tranquilo";
    palavra[144].palavra = "Zangado";
    palavra[145].palavra = "Ruim";
    palavra[146].palavra = "Legal";
    palavra[147].palavra = "Chato";
    palavra[148].palavra = "Tímido";
    palavra[149].palavra = "Gentil";

    //PREPOSICOES
    /*Preposição é a palavra que estabelece uma relação entre dois ou mais termos da oração.
    Essa relação é do tipo subordinativa, ou seja, entre os elementos ligados pela preposição
    não há sentido dissociado, separado, individualizado; ao contrário, o sentido da expressão
    é dependente da união de todos os elementos que a preposição vincula.*/

    palavra[150].palavra = "a";
    palavra[151].palavra = "ante";
    palavra[152].palavra = "após";
    palavra[153].palavra = "com";
    palavra[154].palavra = "contra";
    palavra[155].palavra = "de";
    palavra[156].palavra = "desde";
    palavra[157].palavra = "em";
    palavra[158].palavra = "para";
    palavra[159].palavra = "per";
    palavra[160].palavra = "perante";
    palavra[161].palavra = "por";
    palavra[162].palavra = "sem";
    palavra[163].palavra = "sob";
    palavra[164].palavra = "sobre";
    palavra[165].palavra = "trás";
    palavra[166].palavra = "como";
    palavra[167].palavra = "conforme";
    palavra[168].palavra = "segundo";
    palavra[169].palavra = "durante";
    palavra[170].palavra = "salvo";
    palavra[171].palavra = "fora";
    palavra[172].palavra = "mediante";
    palavra[173].palavra = "tirante";
    palavra[174].palavra = "exceto";
    palavra[175].palavra = "senão";
    palavra[176].palavra = "visto";
    palavra[177].palavra = "a fim de";
    palavra[178].palavra = "além de ";
    palavra[179].palavra = "apesar de";
    palavra[180].palavra = "do";
    palavra[181].palavra = "disto";
    palavra[182].palavra = "disso";
    palavra[183].palavra = "daquilo";
    palavra[184].palavra = "naquele";
    palavra[185].palavra = "naqueles";
    palavra[186].palavra = "naquela";
    palavra[187].palavra = "naquelas";
    palavra[188].palavra = "em vez de";
    palavra[189].palavra = "graças a";
    palavra[190].palavra = "sob pena de";
    palavra[191].palavra = "a respeito de";
    palavra[192].palavra = "ao encontro de";
    palavra[193].palavra = "junto com";
    palavra[194].palavra = "junto de";
    palavra[195].palavra = "junto a";
    palavra[196].palavra = "defronte de";
    palavra[197].palavra = "através de";
    palavra[198].palavra = "graças a";
    palavra[199].palavra = "a par de";



    //VERBOS
    /*Verbo é a classe de palavras que se flexiona em pessoa, número, tempo, modo e voz. Pode indicar, entre outros processos:
      ação (correr);
      estado (ficar);
      fenômeno (chover);
      ocorrência (nascer);
      desejo (querer).*/
    palavra[200].palavra = "Abrir";
    palavra[201].palavra = "Achar";
    palavra[202].palavra = "Beijar";
    palavra[203].palavra = "Amar";
    palavra[204].palavra = "Cantar";
    palavra[205].palavra = "Carregar";
    palavra[206].palavra = "Demonstrar";
    palavra[207].palavra = "Denunciar";
    palavra[208].palavra = "Emprestar";
    palavra[209].palavra = "Enfiar";
    palavra[210].palavra = "Estar";
    palavra[211].palavra = "Forçar";
    palavra[212].palavra = "Ficar";
    palavra[213].palavra = "Fugir";
    palavra[214].palavra = "Guardar";
    palavra[215].palavra = "Guiar";
    palavra[216].palavra = "Haver";
    palavra[217].palavra = "Honrar";
    palavra[218].palavra = "Ir";
    palavra[219].palavra = "Jogar";
    palavra[220].palavra = "Jurar";
    palavra[221].palavra = "Orar";
    palavra[222].palavra = "Ouvir";
    palavra[223].palavra = "Pagar";
    palavra[224].palavra = "Parar";
    palavra[225].palavra = "Passar";
    palavra[226].palavra = "Perder";
    palavra[227].palavra = "Quebrar";
    palavra[228].palavra = "Querer";
    palavra[229].palavra = "Receber";
    palavra[230].palavra = "Reenascer";
    palavra[231].palavra = "Sair";
    palavra[232].palavra = "Salvar";
    palavra[233].palavra = "Segurar";
    palavra[234].palavra = "Tentar";
    palavra[235].palavra = "Sofrer";
    palavra[236].palavra = "Tomar";
    palavra[237].palavra = "Trabalhar";
    palavra[238].palavra = "Estudar";
    palavra[239].palavra = "Usar";
    palavra[240].palavra = "Ver";
    palavra[241].palavra = "Vestir";
    palavra[242].palavra = "Olhar";
    palavra[243].palavra = "Testar";
    palavra[244].palavra = "Aparecer";
    palavra[245].palavra = "Brincar";
    palavra[246].palavra = "Ler";
    palavra[247].palavra = "Avisar";
    palavra[248].palavra = "Esconder";
    palavra[249].palavra = "Assinar";

    //ADVERBIOS
    /*Advérbios são palavras que modificam um verbo, um adjetivo ou um advérbio, indicando uma circunstância
    (tempo, lugar, modo, intensidade,…). Em algumas situações podem modificar toda a oração, vindo, nesses casos,
    destacado por vírgulas no início ou no fim da mesma.*/

    palavra[250].palavra = "Aqui";
    palavra[251].palavra = "Ali";
    palavra[252].palavra = "Dentro";
    palavra[253].palavra = "Fora";
    palavra[254].palavra = "Além";
    palavra[255].palavra = "Acima";
    palavra[256].palavra = "Onde";
    palavra[257].palavra = "Perto";
    palavra[258].palavra = "Longe";
    palavra[259].palavra = "Hoje";
    palavra[260].palavra = "Logo";
    palavra[261].palavra = "Tarde";
    palavra[262].palavra = "Depois";
    palavra[263].palavra = "Nunca";
    palavra[264].palavra = "Ainda";
    palavra[265].palavra = "Agora";
    palavra[266].palavra = "Antes";
    palavra[267].palavra = "Antigamente";
    palavra[268].palavra = "Sempre";
    palavra[269].palavra = "Sim";
    palavra[270].palavra = "Certamente";
    palavra[271].palavra = "Realmente";
    palavra[272].palavra = "Não";
    palavra[273].palavra = "Nem";
    palavra[274].palavra = "Certo";
    palavra[275].palavra = "Jamais";
    palavra[276].palavra = "Acaso";
    palavra[277].palavra = "porventura";
    palavra[278].palavra = "Muito";
    palavra[279].palavra = "Demais";
    palavra[280].palavra = "Pouco";
    palavra[281].palavra = "Mais";
    palavra[282].palavra = "Menos";
    palavra[283].palavra = "Exclusivamente";
    palavra[284].palavra = "Extremamente";
    palavra[285].palavra = "Ainda";
    palavra[286].palavra = "com";
    palavra[287].palavra = "Primeiramente";
    palavra[288].palavra = "Então";
    palavra[289].palavra = "Assim";
    palavra[290].palavra = "Melhor";
    palavra[291].palavra = "Pior";
    palavra[292].palavra = "Depressa";
    palavra[293].palavra = "Breve";
    palavra[294].palavra = "Afinal";
    palavra[295].palavra = "Embaixo";
    palavra[296].palavra = "Aonde";
    palavra[297].palavra = "Detrás";
    palavra[298].palavra = "Propositalmente";
    palavra[299].palavra = "Bondosamente";
}


/*typedef struct Palavra{
    int x;
    int y;
    bool vivo;
    int velocidade;
    int limitey;
    int limitex;
    int ID;
    string palavra;
    int classificacao;
}Palavras;*/

void IniciarPalavra(Palavras palavra[], int tamanho){
    int i;
    for(i = 0; i < tamanho; i++){
        palavra[i].vivo = false;
        palavra[i].velocidade = 4;
        palavra[i].limitex = 50;
        palavra[i].limitey = 20;
    }
}
void DesenharPalavra(Palavras palavra[], int tamanho, ALLEGRO_FONT *fonte){
    int i;
    for(i = 0; i < tamanho; i++){
        if(palavra[i].vivo){
            al_draw_filled_rectangle(palavra[i].x-50,palavra[i].y -20, palavra[i].x + 50, palavra[i].y+20, al_map_rgb(0,0,0));
            al_draw_textf(fonte, al_map_rgb(255,255,255), palavra[i].x,palavra[i].y-10, ALLEGRO_ALIGN_CENTRE, palavra[palavra[i].ID].palavra.c_str());
        }
    }
}
void ComecarPalavra(Palavras palavra[], int tamanho, int n, int nivel){
    int i,aux;
    for(i = 0; i < tamanho; i++){
        if(!palavra[i].vivo){
            if(n % 100 == 0){
                aux = rand() % 300;
                palavra[aux].vivo = true;
                if(nivel<3){
                    palavra[aux].x = 30 + rand() % (LARG - 60);
                    palavra[aux].y = 0;
                }else{
                    palavra[aux].x = 30 + rand() % (LARG/2 - 100);
                    palavra[aux].y = 0;
                }
                break;
            }
        }
    }
}
void AtualizarPalavra(Palavras palavra[], int tamanho){
    int i;
    for(i=0; i<tamanho;i++){
        if(palavra[i].vivo){
            palavra[i].y += palavra[i].velocidade;
        }
    }
}

//Colisão
void ColisaoPalavras(Palavras palavra[], int tamanhoN, CaixaC *caixa,CaixaC *caixan, int nivel, int *acertos){
    bool colisaoa = false;
    bool colisaob = false;
    int i;
    for(i = 0; i < tamanhoN;i++){
        if(palavra[i].vivo){
            if(palavra[i].x - palavra[i].limitex < caixa->x + caixa->limitex &&
               palavra[i].x + palavra[i].limitex > caixa->x - caixa->limitex &&
               palavra[i].y - palavra[i].limitey < caixa->y + caixa->limitey &&
               palavra[i].y + palavra[i].limitey > caixa->y - caixa->limitey){
                colisaoa = true;
                palavra[i].vivo = false;
            }else if(palavra[i].y == ALT){
                palavra[i].vivo = false;
                colisaob = true;
			}
        }

        //PS: Nivel 1 - Pares
        //    Nivel 2 - Substantivos
        //    Nivel 3 - Numerais/Divisiveis por 5
        //    Nivel 4 - Adjetivos/Divisiveis por 3
        //    Nivel 5 - Preposições/Primos
        //    Nivel 6 - Verbos/Divisiveis por 7
        //    Nivel 7 - Adverbios/Quadrados Perfeitos

        //colisaoa = bateu na caixa
        //colisaob = bateu no chão
        switch(nivel){
        case 2:
            if(colisaoa){
                if(palavra[i].classificacao == 1){
                    caixan->pontuacao += 10;
                    colisaoa = false;
				    *acertos+=1;
                }else{
                    caixan->vidas--;
                    if(caixan->pontuacao > 0)
                        caixan->pontuacao -= 10;
                    colisaoa = false;
                }
            }else if(colisaob){
                if(palavra[i].classificacao == 1){
                    if(caixan->pontuacao > 0)
                        caixan->pontuacao -= 10;
                    caixan->vidas--;
                    colisaob = false;
                }else{
                    colisaob = false;
                }
            }
        break;
        case 3:
            if(colisaoa){
                if(palavra[i].classificacao == 2){
                    caixan->pontuacao += 12;
                    colisaoa = false;
				    *acertos+=1;
                }else{
                    caixan->vidas--;
                    if(caixan->pontuacao > 0)
                        caixan->pontuacao -= 8;
                    colisaoa = false;
                }
            }else if(colisaob){
                if(palavra[i].classificacao == 2){
                    if(caixan->pontuacao > 0)
                        caixan->pontuacao -= 8;
                    caixan->vidas--;
                    colisaob = false;
                }else{
                    colisaob = false;
                }
            }
        break;
        case 4:
			if(colisaoa){
				if(palavra[i].classificacao == 3){
					caixan->pontuacao += 20;
					colisaoa=false;
					*acertos+=1;
				}else{
					caixan->vidas--;
					if(caixan->pontuacao > 0)
                        caixan->pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(palavra[i].classificacao == 3){
					if(caixan->pontuacao > 0)
                        caixan->pontuacao -= 5;
					caixan->vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        case 5:
			if(colisaoa){
				if(palavra[i].classificacao == 4){
					caixan->pontuacao += 15;
					colisaoa=false;
					*acertos+=1;
				}else{
					caixan->vidas--;
					if(caixan->pontuacao > 0)
                        caixan->pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(palavra[i].classificacao == 4){
                    if(caixan->pontuacao > 0)
                        caixan->pontuacao -= 5;
					caixan->vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        case 6:
			if(colisaoa){
				if(palavra[i].classificacao == 5){
					caixan->pontuacao += 20;
					colisaoa=false;
					*acertos+=1;
				}else{
					caixan->vidas--;
					if(caixan->pontuacao > 0)
                        caixan->pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(palavra[i].classificacao == 5){
                    if(caixan->pontuacao > 0)
                        caixan->pontuacao -= 5;
					caixan->vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        case 7:
			if(colisaoa){
				if(palavra[i].classificacao == 6){
					caixan->pontuacao += 25;
					colisaoa=false;
					*acertos+=1;
				}else{
					caixan->vidas--;
					if(caixan->pontuacao > 0)
                        caixan->pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(palavra[i].classificacao == 6){
					if(caixan->pontuacao > 0)
                        caixan->pontuacao -= 5;
					caixan->vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        }
    }
}
