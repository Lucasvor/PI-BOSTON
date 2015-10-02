//Inicializa��o das bibliotecas
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "objetos.h"

//Defini��o de constantes (Largura, Altura, FPS e Quantidade de N�meros)
#define LARG 800
#define ALT 600
#define FPS 60
#define QUANT_NUMEROS 50

enum TECLAS{ESQUERDA,DIREITA};
bool teclas[2] = {false, false};

void IniciarCaixa(CaixaC &caixa);
void DesenharCaixa(CaixaC &caixa, ALLEGRO_FONT *fonte, int &nivel);
void MoverCaixaEsquerda(CaixaC &caixa);
void MoverCaixaDireita(CaixaC &caixa);

void ColisaoNumeros(Numeros numero[], int tamanhoN, CaixaC &caixa, int &nivel, int &acertos);
void IniciarNumero(Numeros numero[], int tamanho);
void DesenharNumero(Numeros numero[], int tamanho, ALLEGRO_FONT *fonte);
void ComecarNumero(Numeros numero[], int tamanho, int &n, int &nivel);
void AtualizarNumero(Numeros numero[], int tamanho);

bool primos(Numeros numero[], int &i);
bool quadperfeito(Numeros numero[], int &i);
bool numtriangular(Numeros numero[], int &i);

int main(int argc, char **argv){
    int fim_do_jogo = 0;
    int nivel = 1;
    int n = 0;
	int acertos=0;
    bool redesenhar = false;
    bool isGameOver = false;

    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
    ALLEGRO_FONT *fonte = NULL;

    CaixaC caixa;
    Numeros numero[QUANT_NUMEROS];

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

    srand(time(NULL));
    IniciarCaixa(caixa);
    IniciarNumero(numero, QUANT_NUMEROS);

    fila_de_eventos = al_create_event_queue();
    if(!fila_de_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos!\n");
        return -1;
    }

    fonte = al_load_font("digital-7.ttf", 18, 0);
    if(!fonte){
        fprintf(stderr, "Falha ao carregar fonte!\n");
        return -1;
    }
    al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_de_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_de_eventos, al_get_timer_event_source(timer));

    al_clear_to_color(al_map_rgb(0,0,0));
    DesenharCaixa(caixa, fonte, nivel);
    al_flip_display();

    al_start_timer(timer);
    while (!fim_do_jogo){

        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_de_eventos, &evento);
        if(!isGameOver){
            if(caixa.vidas <= 0){
                isGameOver = true;
            }
            al_flip_display();
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

				al_draw_bitmap(imagem, 0,0,0);
				DesenharCaixa(caixa, fonte, nivel);
				ComecarNumero(numero, QUANT_NUMEROS, n, nivel);
				AtualizarNumero(numero, QUANT_NUMEROS);
				ColisaoNumeros(numero, QUANT_NUMEROS, caixa, nivel,acertos);
				al_draw_filled_rectangle(3,3, 95, 75, al_map_rgb(0,0,0));
				al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 5, 0, "Nivel %i", nivel);
				al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 30, 0, "Vidas: %i",caixa.vidas);
				al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 55, 0, "Pontos: %i",caixa.pontuacao);
				DesenharCaixa(caixa, fonte, nivel);
				DesenharNumero(numero, QUANT_NUMEROS, fonte);
				al_flip_display();
			}
            if(teclas[ESQUERDA] && !isGameOver){
                MoverCaixaEsquerda(caixa);
            }
            if(teclas[DIREITA] && !isGameOver){
                MoverCaixaDireita(caixa);
            }
        }else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            fim_do_jogo = 1;
        }else if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(evento.keyboard.keycode){
            case ALLEGRO_KEY_ESCAPE:
                fim_do_jogo = 1;
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
        if(redesenhar && al_is_event_queue_empty(fila_de_eventos)){
            redesenhar = false;
            if(!isGameOver){


            }else{
                al_clear_to_color(al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTER, "Game Over!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTER, "Pontos: %i", caixa.pontuacao);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTER, "Aperte ESC para sair.");
            }
        }
        al_flip_display();
        n++;
    }

    al_destroy_display(janela);
    al_destroy_bitmap(imagem);
    return 0;
}

//Par�metros da Caixa
void IniciarCaixa(CaixaC &caixa){
    caixa.x = LARG / 2;
    caixa.y = ALT - 10;
    caixa.ID = CAIXA;
    caixa.vidas = 3;
    caixa.velocidade = 9;
    caixa.limitex = 40;
    caixa.limitey = 30;
    caixa.pontuacao = 0;
}
void DesenharCaixa(CaixaC &caixa, ALLEGRO_FONT *fonte, int &nivel){

    al_draw_filled_rectangle(caixa.x-40,caixa.y -30, caixa.x + 40, caixa.y, al_map_rgb(142,107,35));
    al_draw_filled_rectangle(caixa.x-50,caixa.y -30, caixa.x - 30, caixa.y -35, al_map_rgb(142,107,35));
    al_draw_filled_rectangle(caixa.x+50,caixa.y -30, caixa.x + 30, caixa.y -35, al_map_rgb(142,107,35));
    al_draw_rectangle(caixa.x-40,caixa.y -30, caixa.x + 40, caixa.y, al_map_rgb(0,0,0),1);
    al_draw_rectangle(caixa.x-50,caixa.y -30, caixa.x - 30, caixa.y -35, al_map_rgb(0,0,0),1);
    al_draw_rectangle(caixa.x+50,caixa.y -30, caixa.x + 30, caixa.y -35, al_map_rgb(0,0,0),1);

    switch(nivel){
    case 1:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-25, ALLEGRO_ALIGN_CENTER, "PARES");
        break;
    case 2:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-25, ALLEGRO_ALIGN_CENTER, "/ POR 5");
        break;
	case 4:
		al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-25, ALLEGRO_ALIGN_CENTER, "PRIMOS");
        break;
    case 3:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-25, ALLEGRO_ALIGN_CENTER, "/ POR 3");
        break;
    case 5:
        al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-25, ALLEGRO_ALIGN_CENTER, "/ POR 7");
        break;
	case 6:
		al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-25, ALLEGRO_ALIGN_CENTER, "QUADRADOS PERFEITOS");
        break;
	case 7:
		al_draw_textf(fonte, al_map_rgb(255,255,255), caixa.x,caixa.y-25, ALLEGRO_ALIGN_CENTER, "NUMEROS TRIANGULARES");
        break;
    }

}
void MoverCaixaEsquerda(CaixaC &caixa){
    caixa.x -= caixa.velocidade;
    if(caixa.x<50){
        caixa.x = 50;
    }
}
void MoverCaixaDireita(CaixaC &caixa){
    caixa.x += caixa.velocidade;
    if(caixa.x>LARG-50){
        caixa.x = LARG-50;
    }
}

//Par�metros dos N�meros
void IniciarNumero(Numeros numero[], int tamanho){
    for(int i = 0; i < tamanho; i++){
        numero[i].ID = rand();
        numero[i].vivo = false;
        numero[i].velocidade = 4;
        numero[i].limitex = 13;
        numero[i].limitey = 13;
    }
}
void DesenharNumero(Numeros numero[], int tamanho, ALLEGRO_FONT *fonte){
    for(int i = 0; i < tamanho; i++){
        if(numero[i].vivo){
            al_draw_filled_circle(numero[i].x, numero[i].y, 15, al_map_rgb(255,0,0));
            al_draw_textf(fonte, al_map_rgb(255,255,255), numero[i].x,numero[i].y-7, ALLEGRO_ALIGN_CENTER, "%i", numero[i].valor);
        }
    }
}
void ComecarNumero(Numeros numero[], int tamanho, int &n, int &nivel){
    for(int i = 0; i < tamanho; i++){
        if(!numero[i].vivo){
            if(n % 100 == 0){
                numero[i].vivo = true;
                numero[i].x = 30 + rand() % (LARG - 60);
                numero[i].y = 0;
                switch(nivel){
                    case 4:
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
    for(int i=0; i<tamanho;i++){
        if(numero[i].vivo){
            numero[i].y += numero[i].velocidade;
        }
    }
}

//Colis�o
void ColisaoNumeros(Numeros numero[], int tamanhoN, CaixaC &caixa, int &nivel, int &acertos){
    bool colisaoa = false;
    bool colisaob = false;
    for(int i = 0; i < tamanhoN;i++){
        if(numero[i].vivo){
            if(numero[i].x - numero[i].limitex < caixa.x + caixa.limitex &&
               numero[i].x + numero[i].limitex > caixa.x - caixa.limitex &&
               numero[i].y - numero[i].limitey < caixa.y + caixa.limitey &&
               numero[i].y + numero[i].limitey > caixa.y - caixa.limitey){
                colisaoa = true;
                numero[i].vivo = false;
            }else if(numero[i].y == ALT){
                numero[i].vivo = false;
                colisaob = true;
			}
        }

        //PS: Nivel 1 - Pares
        //    Nivel 2 - Divis�veis por 5
        //    Nivel 3 - Divisiveis por 3
        //    Nivel 4 - Primos
        //    Nivel 5 - Divisiveis por 7
        //    Nivel 6 - Quadrados Perfeitos
        //    Nivel 7 - N�meros Triangulares
        switch(nivel){
        case 1:
            if(colisaoa){
                if(numero[i].valor % 2 == 0){
                    caixa.pontuacao += 10;
                    colisaoa = false;
				    acertos++;
                }else{
                    caixa.vidas--;
                    caixa.pontuacao -= 10;
                    colisaoa = false;
                }
            }else if(colisaob){
                if(numero[i].valor % 2 == 0){
                    caixa.pontuacao -= 10;
                    caixa.vidas--;
                    colisaob = false;
                }else{
                    colisaob = false;
                }
            }
        break;
        case 2:
            if(colisaoa){
                if(numero[i].valor % 5 == 0){
                    caixa.pontuacao += 12;
                    colisaoa = false;
				    acertos++;
                }else{
                    caixa.vidas--;
                    caixa.pontuacao -= 8;
                    colisaoa = false;
                }
            }else if(colisaob){
                if(numero[i].valor % 5 == 0){
                    caixa.pontuacao -= 8;
                    caixa.vidas--;
                    colisaob = false;
                }else{
                    colisaob = false;
                }
            }
        break;
		case 4:
			if(colisaoa){
				if(primos(numero, i)){
					caixa.pontuacao += 15;
					colisaoa=false;
					acertos++;
				}else{
					caixa.vidas--;
					caixa.pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(primos(numero, i)){
					caixa.pontuacao -= 5;
					caixa.vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        case 3:
			if(colisaoa){
				if(numero[i].valor % 3 == 0){
					caixa.pontuacao += 20;
					colisaoa=false;
					acertos++;
				}else{
					caixa.vidas--;
					caixa.pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(numero[i].valor % 3 == 0){
					caixa.pontuacao -= 5;
					caixa.vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        case 5:
			if(colisaoa){
				if(numero[i].valor % 7 == 0){
					caixa.pontuacao += 20;
					colisaoa=false;
					acertos++;
				}else{
					caixa.vidas--;
					caixa.pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(numero[i].valor % 7 == 0){
					caixa.pontuacao -= 5;
					caixa.vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        case 6:
			if(colisaoa){
				if(quadperfeito(numero, i)){
					caixa.pontuacao += 25;
					colisaoa=false;
					acertos++;
				}else{
					caixa.vidas--;
					caixa.pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(quadperfeito(numero, i)){
					caixa.pontuacao -= 5;
					caixa.vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        case 7:
			if(colisaoa){
				if(numtriangular(numero, i)){
					caixa.pontuacao += 30;
					colisaoa=false;
					acertos++;
				}else{
					caixa.vidas--;
					caixa.pontuacao -=5;
					colisaoa = false;
				}
			}else if(colisaob){
				if(numtriangular(numero, i)){
					caixa.pontuacao -= 5;
					caixa.vidas--;
					colisaob=false;
				}else{
					colisaob = false;
				}
			}
        break;
        }
    }
}

bool primos(Numeros numero[], int &i){
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

bool quadperfeito(Numeros numero[], int &i){
    int n,t=0,m;
    for(n=1;n<101;n+=2){
    m=n;
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

bool numtriangular(Numeros numero[], int &i){
    int n=0, tri=0;
    for(n=1;n<45;n++){
        tri = ((n*n)+n)/2;
        if(numero[i].valor == tri){
            return true;
        }
    }
    return false;
}
