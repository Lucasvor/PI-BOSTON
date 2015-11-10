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

#include "Function.h"

enum TECLAS{ESQUERDA,DIREITA,ESQUERDAP,DIREITAP};
bool teclas[4] = {false, false, false, false};

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
    		Menu = al_load_bitmap("Menu/Menu.jpg");
    		Menu1= al_load_bitmap("Menu/Menu1.jpg");
    		Menu2= al_load_bitmap("Menu/Menu2.jpg");
   			Menu3= al_load_bitmap("Menu/Menu3.jpg");
   		 	Sair = al_load_bitmap("Menu/Sair.jpg");
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
                        imagem = al_load_bitmap("Fases/fase1.png");
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
                case 1:
                    imagem = al_load_bitmap("Fases/fase1.png");
                    break;
                case 2:
                    imagem = al_load_bitmap("Fases/fase2.jpg");
                    break;
                case 3:
                    imagem = al_load_bitmap("Fases/fase3.jpg");
                    break;
                case 4:
                    imagem = al_load_bitmap("Fases/fase4.jpg");
                    break;
                case 5:
                    imagem = al_load_bitmap("Fases/fase5.jpg");
                    break;
                case 6:
                    imagem = al_load_bitmap("Fases/fase6.jpg");
                    break;
                case 7:
                    imagem = al_load_bitmap("Fases/fase7.jpg");
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
            //PS: Nivel 1 - Pares
            //    Nivel 2 - Substantivos
            //    Nivel 3 - Numerais/Divisiveis por 5
            //    Nivel 4 - Adjetivos/Divisiveis por 3
            //    Nivel 5 - Preposições/Primos
            //    Nivel 6 - Verbos/Divisiveis por 7
            //    Nivel 7 - Adverbios/Quadrados Perfeitos
            switch(nivel){
            case 1:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+75, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Numeros Pares", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  NUMEROS  PARES!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  com  as  setas!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+50, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 2:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+75, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Substantivos", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  SUBSTANTIVOS!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  com  as  teclas  A  e  D!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+50, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 3:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-250,ALT/2-55, LARG/2+250, ALT/2+75, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-50, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Numerais  e  Divisiveis  por  5", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  NUMERAIS  e  os  NUMEROS  DIVISIVEIS  POR  5!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  dos  numeros  com  as  setas!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  das  palavras  com  as  teclas  A  e  D!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+50, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 4:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-250,ALT/2-55, LARG/2+250, ALT/2+75, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-50, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Adjetivos  e  Divisiveis  por  3", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  ADJETIVOS  e  os  NUMEROS  DIVISIVEIS  POR  3!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  dos  numeros  com  as  setas!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  das  palavras  com  as  teclas  A  e  D!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+50, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 5:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-250,ALT/2-55, LARG/2+250, ALT/2+75, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-50, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Preposicoes  e  Primos", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Pegue  somente  as  PREPOSICOES  e  os  NUMEROS  PRIMOS!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  dos  numeros  com  as  setas!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  das  palavras  com  as  teclas  A  e  D!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+50, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 6:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-250,ALT/2-55, LARG/2+250, ALT/2+75, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-50, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Verbos  e  Divisiveis por 7", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  VERBOS  e  os  NUMEROS  DIVISIVEIS  POR  7!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  dos  numeros  com  as  setas!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  das  palavras  com  as  teclas  A  e  D!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+50, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                al_flip_display();
                break;
            case 7:
                al_draw_bitmap(imagem, 0,0,0);
                al_draw_filled_rectangle(LARG/2-250,ALT/2-55, LARG/2+250, ALT/2+75, al_map_rgb(0,0,0));
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-50, ALLEGRO_ALIGN_CENTRE, "Nivel  %d:  Adverbios  e  Quadrados Perfeitos", nivel);
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Pegue  somente  os  ADVERBIOS  e  os  QUADRADOS PERFEITOS!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  dos  numeros  com  as  setas!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Movimente  a  caixa  das  palavras  com  as  teclas  A  e  D!");
                al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+50, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
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
            IniciarNumero(numero,QUANT_NUMEROS,nivel);
            IniciarPalavra(palavra,QUANT_PALAVRAS,nivel);
            if(nivel>2){
                caixa.x = (LARG / 4) * 3;
                caixa2.x = LARG / 4;
                caixa.velocidade = 9;
                caixa2.velocidade = 9;
            }
        }
    }
    al_destroy_display(janela);
    al_destroy_bitmap(imagem);
    return 0;
}
