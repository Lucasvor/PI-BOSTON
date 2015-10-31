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
#define LARG 800
#define ALT 600
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
    int ID;
    int x;
    int y;
    bool vivo;
    int velocidade;
    int limitey;
    int limitex;
    string palavra;
}Palavras;

enum TECLAS{ESQUERDA,DIREITA};
bool teclas[2] = {false, false};

void PaineldeInfo(int acertos, int nivel, CaixaC caixa, ALLEGRO_FONT *fonte);
void IniciarCaixa(CaixaC *caixa);
void DesenharCaixa(CaixaC caixa, ALLEGRO_FONT *fonte, int nivel);
void MoverCaixaEsquerda(CaixaC *caixa);
void MoverCaixaDireita(CaixaC *caixa);

void ColisaoNumeros(Numeros numero[], int tamanhoN, CaixaC *caixa, int nivel, int *acertos);
void IniciarNumero(Numeros numero[], int tamanho);
void DesenharNumero(Numeros numero[], int tamanho, ALLEGRO_FONT *fonte);
void ComecarNumero(Numeros numero[], int tamanho, int n, int nivel);
void AtualizarNumero(Numeros numero[], int tamanho);

bool primos(Numeros numero[], int i);
bool quadperfeito(Numeros numero[], int i);

int main(int argc, char **argv){
    bool fim_do_jogo = false;
    int nivel = 1;
    int n = 0;
	int acertos=0;
	int Jogo = 0;
    bool redesenhar = false;
    bool isGameOver = false;
    int regiao = 0;

    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
    ALLEGRO_FONT *fonte = NULL;

    CaixaC caixa;
    Numeros numero[QUANT_NUMEROS];
    Palavras palavra[QUANT_PALAVRAS];

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
    imagem = al_load_bitmap("fase1.jpg");
    al_install_keyboard();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_mouse();
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);

    srand(time(NULL));
    IniciarCaixa(&caixa);
    IniciarNumero(numero, QUANT_NUMEROS);

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
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila_de_eventos, &evento);
                if(evento.type == ALLEGRO_EVENT_MOUSE_AXES){
                    if(evento.mouse.x >= 328 && evento.mouse.x <= 523 &&
                       evento.mouse.y >= 202 && evento.mouse.y <= 289)
                        regiao = 1;
                    else if(evento.mouse.x >= 270 && evento.mouse.x <= 609 &&
                            evento.mouse.y >= 278 && evento.mouse.y <= 367)
                        regiao = 2;
                    else if(evento.mouse.x >= 329 && evento.mouse.x <= 538 &&
                            evento.mouse.y >= 366 && evento.mouse.y <= 466)
                        regiao = 3;
                    else if(evento.mouse.x >= 665 && evento.mouse.x <= 796 &&
                            evento.mouse.y >= 6 && evento.mouse.y <= 95)
                        regiao = 4;
                    else
                        regiao = 0;

                }
                if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if(regiao == 4)
                        fim_do_jogo = true;
                    if (regiao == 1)
                        Jogo = 1;
                }
                if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    return 0;
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
            redesenhar = true;
			if(acertos == 10){
				nivel++;
				acertos = 0;
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
                al_flip_display();
				al_draw_bitmap(imagem, 0,0,0);
				DesenharCaixa(caixa, fonte, nivel);
				ComecarNumero(numero, QUANT_NUMEROS, n, nivel);
				AtualizarNumero(numero, QUANT_NUMEROS);
				ColisaoNumeros(numero, QUANT_NUMEROS, &caixa, nivel, &acertos);
				PaineldeInfo(acertos, nivel, caixa, fonte);
				DesenharCaixa(caixa, fonte, nivel);
				DesenharNumero(numero, QUANT_NUMEROS, fonte);
                al_flip_display();
			}
            if(teclas[ESQUERDA] && !isGameOver){
                MoverCaixaEsquerda(&caixa);
            }
            if(teclas[DIREITA] && !isGameOver){
                MoverCaixaDireita(&caixa);
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
            }
        }else if(evento.type == ALLEGRO_EVENT_KEY_UP){
            switch(evento.keyboard.keycode){
            case ALLEGRO_KEY_LEFT:
                teclas[ESQUERDA] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                teclas[DIREITA] = false;
                break;
            }
        }
        if(isGameOver && al_is_event_queue_empty(fila_de_eventos)){
            if(isGameOver){
                al_clear_to_color(al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Game  Over!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pontos:  %i", caixa.pontuacao);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ESC  para  sair.");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+50, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  voltar  ao  menu.");
                if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
                    switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_PAD_ENTER:
                        Jogo = 0;
                        IniciarCaixa(&caixa);
                        IniciarNumero(numero,QUANT_NUMEROS);
                        acertos=0;
                        nivel = 0;
                        isGameOver = false;
                        break;
                    case ALLEGRO_KEY_ENTER:
                        Jogo = 0;
                        IniciarCaixa(&caixa);
                        IniciarNumero(numero,QUANT_NUMEROS);
                        acertos=0;
                        nivel = 1;
                        isGameOver = false;
                        break;
                    }
                }
                al_flip_display();
            }
        }
        n++;
        }
    }
    al_destroy_display(janela);
    al_destroy_bitmap(imagem);
    return 0;
}

//Parâmetros da Caixa
void IniciarCaixa(CaixaC *caixa){
    caixa->x = LARG / 2;
    caixa->y = ALT - 10;
    caixa->ID = 12;
    caixa->vidas = 3;
    caixa->velocidade = 9;
    caixa->limitex = 40;
    caixa->limitey = 30;
    caixa->pontuacao = 0;
}
void DesenharCaixa(CaixaC caixa, ALLEGRO_FONT *fonte, int nivel){

    al_draw_filled_rectangle(caixa.x-40,caixa.y -30, caixa.x + 40, caixa.y, al_map_rgb(142,107,35));
    al_draw_filled_rectangle(caixa.x-50,caixa.y -30, caixa.x - 30, caixa.y -35, al_map_rgb(142,107,35));
    al_draw_filled_rectangle(caixa.x+50,caixa.y -30, caixa.x + 30, caixa.y -35, al_map_rgb(142,107,35));
    al_draw_rectangle(caixa.x-40,caixa.y -30, caixa.x + 40, caixa.y, al_map_rgb(0,0,0),1);
    al_draw_rectangle(caixa.x-50,caixa.y -30, caixa.x - 30, caixa.y -35, al_map_rgb(0,0,0),1);
    al_draw_rectangle(caixa.x+50,caixa.y -30, caixa.x + 30, caixa.y -35, al_map_rgb(0,0,0),1);

    //PS: Nivel 1 - Pares
        //    Nivel 2 - Divisíveis por 5
        //    Nivel 3 - Divisiveis por 3
        //    Nivel 4 - Primos
        //    Nivel 5 - Divisiveis por 7
        //    Nivel 6 - Quadrados Perfeitos
        //    Nivel 7 - Números Triangulares
    switch(nivel){
    case 1:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-28, ALLEGRO_ALIGN_CENTRE, "PARES");
        break;
    case 2:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-28, ALLEGRO_ALIGN_CENTRE, "/ POR 5");
        break;
    case 3:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-28, ALLEGRO_ALIGN_CENTRE, "/ POR 3");
        break;
    case 4:
		al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-28, ALLEGRO_ALIGN_CENTRE, "PRIMOS");
        break;
    case 5:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-28, ALLEGRO_ALIGN_CENTRE, "/ POR 7");
        break;
	case 6:
		al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-28, ALLEGRO_ALIGN_CENTRE, "QUADRADOS PERFEITOS");
        break;
    }
}
void MoverCaixaEsquerda(CaixaC *caixa){
    caixa->x -= caixa->velocidade;
    if(caixa->x<50){
        caixa->x = 50;
    }
}
void MoverCaixaDireita(CaixaC *caixa){
    caixa->x += caixa->velocidade;
    if(caixa->x>LARG-50){
        caixa->x = LARG-50;
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
                numero[i].x = 30 + rand() % (LARG - 60);
                numero[i].y = 0;
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
        //    Nivel 5 - Pronomes/Primos
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
    }

    //Definir as palavras:
    // 0 - 50: Substantivos
    // 50 - 100: Numerais
    // 100 - 150: Adjetivos
    // 150 - 200: Pronomes
    // 200 - 250: Verbos
    // 250 - 300: Adverbios

    //SOMENTE PALAVRAS COM >>DEZ<< LETRAS OU MENOS.

    //SINTAXE: palavra[ID].palavra = "palavra";
    //Ex:

    palavra[0].palavra = "Batata";
    palavra[1].palavra = "Flor";
}
