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
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


#include "Function.h"

enum TECLAS{ESQUERDA,DIREITA,ESQUERDAP,DIREITAP};
bool teclas[4] = {false, false, false, false};

ALLEGRO_SAMPLE *acertou = NULL;
ALLEGRO_SAMPLE *errou = NULL;
ALLEGRO_SAMPLE_INSTANCE *musicaIf1 = NULL,*musicaIf2 = NULL,*musicaIf3 = NULL,*musicaIf4 = NULL,*musicaIf5 = NULL,*musicaIf6 = NULL,*musicaIf7 = NULL;
ALLEGRO_SAMPLE *musicaf1 = NULL,*musicaf2 = NULL,*musicaf3 = NULL,*musicaf4 = NULL,*musicaf5 = NULL,*musicaf6 = NULL,*musicaf7 = NULL;

int main(int argc, char **argv){
    bool fim_do_jogo = false, jogo_pausado = false;
    int nivel = 1;
    int n = 0,status = 0,status2 = 0;
	int acertos=0;
	int Jogo = 0;
    bool isGameOver = false;
    int regiao = 0;

    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_DISPLAY *janela = NULL;
    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_BITMAP *chao = NULL;
    ALLEGRO_BITMAP *corrente = NULL;
    ALLEGRO_EVENT_QUEUE *fila_de_eventos = NULL;
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_BITMAP *pause = NULL;
    ALLEGRO_BITMAP *chip = NULL;
    ALLEGRO_BITMAP *coisa = NULL;

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
    al_install_audio();
    al_init_acodec_addon();
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    al_reserve_samples(10);
    fonte = al_load_font("BEBAS.ttf", 18, 0);
    if(!fonte){
        fprintf(stderr, "Falha ao carregar fonte!\n");
        return -1;
    }
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2-50, ALT/2, 0, "Carregando...");
    al_flip_display();
    acertou = al_load_sample("sounds/acertou.ogg");
    errou = al_load_sample("sounds/errou.ogg");
    musicaf1 = al_load_sample("sounds/fase1.ogg");
    musicaf2 = al_load_sample("sounds/fase2.ogg");
    musicaf3 = al_load_sample("sounds/fase3.ogg");
    musicaf4 = al_load_sample("sounds/fase4.ogg");
    musicaf5 = al_load_sample("sounds/fase5.ogg");
    musicaf6 = al_load_sample("sounds/fase6.ogg");
    musicaf7 = al_load_sample("sounds/fase7.ogg");
    chao = al_load_bitmap("Backgrounds/chao.jpg");
    corrente = al_load_bitmap("Backgrounds/correntes.png");
    pause = al_load_bitmap("Backgrounds/Menupausa.png");
    coisa = al_load_bitmap("Backgrounds/palavra.png");
    chip = al_load_bitmap("Backgrounds/chip.png");


    musicaIf1 = al_create_sample_instance(musicaf1);
    musicaIf2 = al_create_sample_instance(musicaf2);
    musicaIf3 = al_create_sample_instance(musicaf3);
    musicaIf4 = al_create_sample_instance(musicaf4);
    musicaIf5 = al_create_sample_instance(musicaf5);
    musicaIf6 = al_create_sample_instance(musicaf6);
    musicaIf7 = al_create_sample_instance(musicaf7);
    al_set_sample_instance_playmode(musicaIf1, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(musicaIf2, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(musicaIf3, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(musicaIf4, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(musicaIf5, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(musicaIf6, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_playmode(musicaIf7, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(musicaIf1, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(musicaIf2, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(musicaIf3, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(musicaIf4, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(musicaIf5, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(musicaIf6, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(musicaIf7, al_get_default_mixer());

    srand(time(NULL));
    fila_de_eventos = al_create_event_queue();
    if(!fila_de_eventos){
        fprintf(stderr, "Falha ao criar fila de eventos!\n");
        return -1;
    }


    al_register_event_source(fila_de_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_de_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_de_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_de_eventos, al_get_mouse_event_source());
    al_start_timer(timer);
    ALLEGRO_EVENT evento;
    while (!fim_do_jogo){
    	if(Jogo == 0){
            int tempo=0;
            al_play_sample_instance(musicaIf7);
    		ALLEGRO_BITMAP *Menu = NULL,*Menu1s1 = NULL,*Menu1s2 = NULL,*Menu1s3 = NULL,*Menu1s4 = NULL,*Menu2s1 = NULL,*Menu2s2 = NULL,*Menu2s3 = NULL,*Menu2s4 = NULL,*Menu3s1 = NULL,*Menu3s2 = NULL,*Menu3s3 = NULL,*Menu3s4 = NULL,*Sair = NULL;
    		Menu = al_load_bitmap("Menu/Menu.jpg");
    		Menu1s1= al_load_bitmap("Menu/Menu1s1.jpg");
    		Menu1s2= al_load_bitmap("Menu/Menu1s2.jpg");
    		Menu1s3= al_load_bitmap("Menu/Menu1s3.jpg");
    		Menu1s4= al_load_bitmap("Menu/Menu1s4.jpg");
    		Menu2s1= al_load_bitmap("Menu/Menu2s1.jpg");
    		Menu2s2= al_load_bitmap("Menu/Menu2s2.jpg");
    		Menu2s3= al_load_bitmap("Menu/Menu2s3.jpg");
    		Menu2s4= al_load_bitmap("Menu/Menu2s4.jpg");
    		Menu3s1= al_load_bitmap("Menu/Menu3s1.jpg");
    		Menu3s2= al_load_bitmap("Menu/Menu3s2.jpg");
    		Menu3s3= al_load_bitmap("Menu/Menu3s3.jpg");
    		Menu3s4= al_load_bitmap("Menu/Menu3s4.jpg");
   		 	Sair = al_load_bitmap("Menu/sair.jpg");
   		 	while(Jogo == 0){
                switch(regiao){
                case 4:
                    al_draw_bitmap(Sair,0,0,0);
                    break;
                case 3:
                     if(tempo==0)
                    al_draw_bitmap(Menu3s1,0,0,0);
                    else if(tempo==10)
                    al_draw_bitmap(Menu3s2,0,0,0);
                    else if(tempo==20)
                    al_draw_bitmap(Menu3s3,0,0,0);
                    else if(tempo==30)
                    al_draw_bitmap(Menu3s4,0,0,0);
                    else if(tempo>=40)
                        tempo=0;
                    tempo++;
                    break;
                case 2:
                     if(tempo==0)
                    al_draw_bitmap(Menu2s1,0,0,0);
                    else if(tempo==10)
                    al_draw_bitmap(Menu2s2,0,0,0);
                    else if(tempo==20)
                    al_draw_bitmap(Menu2s3,0,0,0);
                    else if(tempo==30)
                    al_draw_bitmap(Menu2s4,0,0,0);
                    else if(tempo>=40)
                        tempo=0;
                    tempo++;
                    break;
                case 1:
                    if(tempo==0)
                    al_draw_bitmap(Menu1s1,0,0,0);
                    else if(tempo==10)
                    al_draw_bitmap(Menu1s2,0,0,0);
                    else if(tempo==20)
                    al_draw_bitmap(Menu1s3,0,0,0);
                    else if(tempo==30)
                    al_draw_bitmap(Menu1s4,0,0,0);
                    else if(tempo>=40)
                        tempo=0;
                    tempo++;
                    break;
                case 0:
                    al_draw_bitmap(Menu,0,0,0);
                    tempo=0;
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
                    if (regiao == 3){
                        Jogo = 3;
                    }
                    if (regiao == 1){
                        imagem = al_load_bitmap("Fases/fase1.png");
                        nivel = 1;
                        acertos = 0;
                        Jogo = 2;
                        IniciarCaixa(&caixa,nivel);
                        IniciarCaixaP(&caixa2,nivel);
                        al_stop_sample_instance(musicaIf1);
                    al_stop_sample_instance(musicaIf2);
                    al_stop_sample_instance(musicaIf3);
                    al_stop_sample_instance(musicaIf4);
                    al_stop_sample_instance(musicaIf5);
                    al_stop_sample_instance(musicaIf6);
                    al_stop_sample_instance(musicaIf7);
                    }
                }
                if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                    fim_do_jogo = true;
                    break;
                }

            }
            al_destroy_bitmap(Menu);
            al_destroy_bitmap(Menu1s1);
            al_destroy_bitmap(Menu1s2);
            al_destroy_bitmap(Menu1s3);
            al_destroy_bitmap(Menu1s4);
            al_destroy_bitmap(Menu2s1);
            al_destroy_bitmap(Menu2s2);
            al_destroy_bitmap(Menu2s3);
            al_destroy_bitmap(Menu2s4);
            al_destroy_bitmap(Menu3s1);
            al_destroy_bitmap(Menu3s2);
            al_destroy_bitmap(Menu3s3);
            al_destroy_bitmap(Menu3s4);
            al_destroy_bitmap(Sair);
            al_clear_to_color(al_map_rgb(0,0,0));
            al_flip_display();
    	}
    	if(Jogo == 1){
            regiao=0;
            if(!jogo_pausado){
                if(!isGameOver){
                    switch(nivel){
                    case 1:
                        al_play_sample_instance(musicaIf1);
                        break;
                    case 2:
                        al_play_sample_instance(musicaIf2);
                        break;
                    case 3:
                        al_play_sample_instance(musicaIf3);
                        break;
                    case 4:
                        al_play_sample_instance(musicaIf4);
                        break;
                    case 5:
                        al_play_sample_instance(musicaIf5);
                        break;
                    case 6:
                        al_play_sample_instance(musicaIf6);
                        break;
                    case 7:
                        al_play_sample_instance(musicaIf7);
                        break;
                    }
                }
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila_de_eventos, &evento);
                if(!isGameOver){
                    if(caixa.vidas <= 0){
                        isGameOver = true;
                    }

                }
                if(evento.type == ALLEGRO_EVENT_TIMER){
                    if(nivel<3){
                        if(acertos >= 10){
                            nivel++;
                            acertos = 0;
                            status=1;status2=1;
                            Jogo = 2;
                            caixa.vidas++;
                            teclas[ESQUERDA] = false;
                            teclas[DIREITA] = false;
                            teclas[ESQUERDAP] = false;
                            teclas[DIREITAP] = false;
                            switch(nivel){
                            case 1:
                                al_destroy_bitmap(imagem);
                                imagem = al_load_bitmap("Fases/fase1.png");
                                break;
                            case 2:
                                al_destroy_bitmap(imagem);
                                imagem = al_load_bitmap("Fases/fase2.jpg");
                                break;
                            case 3:
                                al_destroy_bitmap(imagem);
                                imagem = al_load_bitmap("Fases/fase3.jpg");
                                break;
                            }
                        }
                    }else{
                        if(acertos >= 20){
                            nivel++;
                            acertos = 0;
                            status=1;status2=1;
                            Jogo = 2;
                            teclas[ESQUERDA] = false;
                            teclas[DIREITA] = false;
                            teclas[ESQUERDAP] = false;
                            teclas[DIREITAP] = false;
                            switch(nivel){
                            case 3:
                                al_destroy_bitmap(imagem);
                                imagem = al_load_bitmap("Fases/fase3.jpg");
                                break;
                            case 4:
                                al_destroy_bitmap(imagem);
                                imagem = al_load_bitmap("Fases/fase4.jpg");
                                break;
                            case 5:
                                al_destroy_bitmap(imagem);
                                imagem = al_load_bitmap("Fases/fase5.jpg");
                                break;
                            case 6:
                                al_destroy_bitmap(imagem);
                                imagem = al_load_bitmap("Fases/fase6.jpg");
                                break;
                            case 7:
                                al_destroy_bitmap(imagem);
                                imagem = al_load_bitmap("Fases/fase7.jpg");
                                break;
                            }
                        }
                    }
                    if(!isGameOver){

                        al_draw_bitmap(imagem, 0,0,0);
                        PaineldeInfo(acertos, nivel, caixa, fonte);

                        al_draw_bitmap(chao,0,ALT-45,0);
                        if(nivel!=2){
                            DesenharCaixa(caixa, fonte, nivel,&status);
                            ComecarNumero(numero, QUANT_NUMEROS, n, nivel);
                            AtualizarNumero(numero, QUANT_NUMEROS, caixa, &status);
                            ColisaoNumeros(numero, QUANT_NUMEROS, &caixa, nivel, &acertos, acertou, errou);
                            DesenharNumero(numero, QUANT_NUMEROS, fonte);
                        }

                        if(nivel>=2){
                            DesenharCaixaP(caixa2, fonte, nivel, &status2);
                            ComecarPalavra(palavra, QUANT_PALAVRAS, n, nivel);
                            AtualizarPalavra(palavra, QUANT_PALAVRAS, &status2, caixa2);
                            ColisaoPalavras(palavra, QUANT_PALAVRAS, &caixa2, &caixa, nivel, &acertos, acertou, errou);
                            DesenharPalavra(palavra, QUANT_PALAVRAS, fonte);
                        }
                        if(nivel>=3) al_draw_bitmap(corrente,(590),0,0);
                        al_flip_display();
                    }
                    if(teclas[ESQUERDA] && !isGameOver){
                        MoverCaixaEsquerda(&caixa,nivel);
                        status = 2;
                    }
                    if(teclas[DIREITA] && !isGameOver){
                        MoverCaixaDireita(&caixa,nivel);
                        status = 1;
                    }
                    if(teclas[ESQUERDAP] && !isGameOver){
                        MoverCaixaEsquerdaP(&caixa2,nivel);
                        status2 = 2;
                    }
                    if(teclas[DIREITAP] && !isGameOver){
                        MoverCaixaDireitaP(&caixa2,nivel);
                        status2 = 1;
                    }
                }else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                    fim_do_jogo = true;
                }else if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
                    switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_ESCAPE:
                        jogo_pausado = true;
                        al_stop_sample_instance(musicaIf1);
                        al_stop_sample_instance(musicaIf2);
                        al_stop_sample_instance(musicaIf3);
                        al_stop_sample_instance(musicaIf4);
                        al_stop_sample_instance(musicaIf5);
                        al_stop_sample_instance(musicaIf6);
                        al_stop_sample_instance(musicaIf7);
                        break;
                    case ALLEGRO_KEY_LEFT:
                        if(!isGameOver){
                            teclas[ESQUERDA] = true;
                            status = 2;
                        }
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        if(!isGameOver){
                            teclas[DIREITA] = true;
                            status = 1;
                        }
                        break;
                    case ALLEGRO_KEY_UP:
                        status = 3;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        status = 4;
                        break;
                    case ALLEGRO_KEY_A:
                        if(!isGameOver){
                            teclas[ESQUERDAP] = true;
                            status2 = 2;
                        }
                        break;
                    case ALLEGRO_KEY_D:
                        if(!isGameOver){
                            teclas[DIREITAP] = true;
                            status2 = 1;
                        }
                        break;
                    case ALLEGRO_KEY_K:
                        if(!isGameOver){
                            if(nivel<=2) acertos=10;
                            if(nivel>2) acertos=20;
                        }
                        break;
                    }
                }else if(evento.type == ALLEGRO_EVENT_KEY_UP){
                    switch(evento.keyboard.keycode){
                    case ALLEGRO_KEY_LEFT:
                        status = 0;
                        teclas[ESQUERDA] = false;
                        break;
                    case ALLEGRO_KEY_RIGHT:
                        status = 0;
                        teclas[DIREITA] = false;
                        break;
                    case ALLEGRO_KEY_A:
                        status2 = 0;
                        teclas[ESQUERDAP] = false;
                        break;
                    case ALLEGRO_KEY_D:
                        status2 = 0;
                        teclas[DIREITAP] = false;
                        break;
                    case ALLEGRO_KEY_UP:
                        status = 0;
                        break;
                    case ALLEGRO_KEY_DOWN:
                        status = 0;
                        break;
                    }
                }
            if(isGameOver && al_is_event_queue_empty(fila_de_eventos)){
                if(isGameOver){
                    al_stop_sample_instance(musicaIf1);
                    al_stop_sample_instance(musicaIf2);
                    al_stop_sample_instance(musicaIf3);
                    al_stop_sample_instance(musicaIf4);
                    al_stop_sample_instance(musicaIf5);
                    al_stop_sample_instance(musicaIf6);
                    al_stop_sample_instance(musicaIf7);
                    al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+80, al_map_rgb(0,0,0));
                    if(nivel>7)
                        al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Parabens!  Voce  terminou  o  jogo!");
                    else
                        al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Game  Over!");
                    al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "Pontos:  %i", caixa.pontuacao);
                    al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ESC  para  sair.");
                    al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+50, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  voltar  ao  menu.");
                    if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
                        switch(evento.keyboard.keycode){
                        case ALLEGRO_KEY_PAD_ENTER:
                            Jogo = 0;
                            isGameOver = false;
                            al_clear_to_color(al_map_rgb(0,0,0));
                            al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2-50, ALT/2, 0, "Carregando...");
                            al_flip_display();
                            break;
                        case ALLEGRO_KEY_ENTER:
                            Jogo = 0;
                            isGameOver = false;
                            al_clear_to_color(al_map_rgb(0,0,0));
                            al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2-50, ALT/2, 0, "Carregando...");
                            al_flip_display();
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
            }else if(jogo_pausado){
                while(jogo_pausado){
                switch(regiao){
                case 3:
                    al_draw_filled_circle(513, 428, 20, al_map_rgb(203,255,187));
                    break;
                case 2:
                    al_draw_filled_circle(441, 345, 20, al_map_rgb(203,255,187));
                    break;
                case 1:
                    al_draw_filled_circle(460, 264, 20, al_map_rgb(203,255,187));
                    break;
                default:
                    al_draw_bitmap(pause,0,0,0);
                }
                al_flip_display();

                al_wait_for_event(fila_de_eventos, &evento);
                if(evento.type == ALLEGRO_EVENT_MOUSE_AXES){
                    if(evento.mouse.x >= 496 && evento.mouse.x <= 774 &&
                       evento.mouse.y >= 233 && evento.mouse.y <= 295)
                        regiao = 1;
                    else if(evento.mouse.x >= 473 && evento.mouse.x <= 808 &&
                            evento.mouse.y >= 318 && evento.mouse.y <= 376)
                        regiao = 2;
                    else if(evento.mouse.x >= 558 && evento.mouse.x <= 709 &&
                            evento.mouse.y >= 391 && evento.mouse.y <= 453)
                        regiao = 3;
                    else
                        regiao = 0;
                }
                if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                    if(regiao == 3){
                        Jogo = 0;
                        al_clear_to_color(al_map_rgb(0,0,0));
                        al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2-50, ALT/2, 0, "Carregando...");
                        al_flip_display();
                        jogo_pausado = false;
                        break;
                    }
                    if(regiao == 2){
                        imagem = al_load_bitmap("Fases/fase1.png");
                        nivel = 1;
                        acertos = 0;
                        Jogo = 2;
                        IniciarCaixa(&caixa,nivel);
                        IniciarCaixaP(&caixa2,nivel);
                        jogo_pausado = false;
                        break;
                    }
                    if (regiao == 1){
                        switch(nivel){
                    case 1:
                        al_play_sample_instance(musicaIf1);
                        break;
                    case 2:
                        al_play_sample_instance(musicaIf2);
                        break;
                    case 3:
                        al_play_sample_instance(musicaIf3);
                        break;
                    case 4:
                        al_play_sample_instance(musicaIf4);
                        break;
                    case 5:
                        al_play_sample_instance(musicaIf5);
                        break;
                    case 6:
                        al_play_sample_instance(musicaIf6);
                        break;
                    case 7:
                        al_play_sample_instance(musicaIf7);
                        break;
                    }
                    jogo_pausado = false;
                    }
                }
                if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                    fim_do_jogo = true;
                    break;
                }

            }
            }
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
            al_stop_sample_instance(musicaIf1);
            al_stop_sample_instance(musicaIf2);
            al_stop_sample_instance(musicaIf3);
            al_stop_sample_instance(musicaIf4);
            al_stop_sample_instance(musicaIf5);
            al_stop_sample_instance(musicaIf6);
            al_stop_sample_instance(musicaIf7);

            al_wait_for_event(fila_de_eventos, &evento);
            if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
                switch(evento.keyboard.keycode){
                case ALLEGRO_KEY_PAD_ENTER:
                    if(nivel<8)
                        Jogo = 1;
                    if(nivel>=8)
                        Jogo = 0;

                    break;
                case ALLEGRO_KEY_ENTER:
                    if(nivel<8)
                        Jogo = 1;
                    if(nivel>=8)
                        Jogo = 0;

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
            status = 0;
            status2 = 0;
            if(nivel>2){
                caixa.x = (LARG / 4) * 3;
                caixa2.x = LARG / 4;
                caixa.velocidade = 9;
                caixa2.velocidade = 9;
            }
        }
        if(Jogo==3){
            int tempo=0;
            int parte=0;
            int xPersonagem=LARG-300;
            int x1=LARG-330;
            int x2=370;
            int x3=350;
            int x5=300;
            int x4=LARG-450;
            int y=0;
            int dir_y=1;
            int dir_x=2;
            bool vivo=true;
            ALLEGRO_BITMAP *instrucao=NULL;
            ALLEGRO_BITMAP *instrucao1=NULL;
            ALLEGRO_BITMAP *teclas=NULL;
            ALLEGRO_BITMAP *direita=NULL;
            ALLEGRO_BITMAP *esquerda=NULL;
            ALLEGRO_BITMAP *AED=NULL;
            ALLEGRO_BITMAP *A=NULL;
            ALLEGRO_BITMAP *D=NULL;
            ALLEGRO_BITMAP *personagem=NULL;
            ALLEGRO_BITMAP *balao=NULL;
            ALLEGRO_BITMAP *robo=NULL;
            ALLEGRO_FONT *fonte1 = NULL;
            ALLEGRO_FONT *fonteEnter = NULL;
            fonte1 = al_load_font("H-man.ttf", 27, 0);
            fonteEnter = al_load_font("H-man.ttf", 20, 0);
            instrucao = al_load_bitmap("Instrucoes/fundo2.jpg");
            instrucao1 = al_load_bitmap("Instrucoes/fundo.jpg");
            teclas = al_load_bitmap("Instrucoes/seta.png");
            direita = al_load_bitmap("Instrucoes/Direita.png");
            esquerda = al_load_bitmap("Instrucoes/Esquerda.png");
            AED = al_load_bitmap("Instrucoes/teclado.png");
            A = al_load_bitmap("Instrucoes/A.png");
            D = al_load_bitmap("Instrucoes/D.png");
            personagem = al_load_bitmap("Instrucoes/professor.png");
            balao = al_load_bitmap("Instrucoes/balao.png");
            robo = al_load_bitmap("Instrucoes/sem.png");
            ALLEGRO_EVENT evento;
            while(Jogo==3){
            al_wait_for_event(fila_de_eventos, &evento);


            if(parte==0||parte==1||parte==3 || parte==5){

                if(evento.type == ALLEGRO_EVENT_KEY_DOWN){
                        switch(evento.keyboard.keycode){
                            case ALLEGRO_KEY_ENTER:
                                parte++;
                            break;
                        }
                    }
                }

                    if(evento.type == ALLEGRO_EVENT_TIMER){
                switch(parte){
                    case 0:
                        al_draw_bitmap(instrucao,0,0,0);
                        al_draw_bitmap(robo, 500, 600, 0);
                        al_draw_bitmap(personagem, xPersonagem, 260, 0);

                        xPersonagem-=6*dir_x;
                        if(xPersonagem<-100){
                            xPersonagem=-100;
                        }
                        if(xPersonagem==-100){
                        al_draw_bitmap(balao, 290, 20, 0);
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 350,40, 0, "Ola, esse e o robo Donnie! Apos a");
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,80, 0, "guerra que destruiu o nosso planeta,");
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,120, 0, "ele perdeu a memoria. Ajude nosso ");
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,160, 0, "robo a recuperar sua inteligencia, ");
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,200, 0, "coletando os chips de numeros ");
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,240, 0, "ou palavras de acordo com as ");
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,280, 0, "instrucoes de cada nivel. ");
                        al_draw_textf(fonteEnter, al_map_rgb(0,0,0), 500,320, 0, "aperte enter para continuar!");
                        }
                        al_flip_display();
                    break;
                    case 1:
                    status=0;
                    al_draw_bitmap(instrucao1,0,0,0);
                    IniciarCaixa(&caixa, nivel);
                    al_draw_bitmap(chao,0,ALT-45,0);

                    //al_draw_bitmap(robo, 500, 600, 0);


                    DesenharCaixa(caixa, fonte, nivel,&status);
                    al_draw_bitmap(personagem, -100, 260, 0);
                    al_draw_bitmap(balao, 290, 20, 0);
                    al_draw_bitmap(teclas, 400, 200, 0);
                    //al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+50, al_map_rgb(0,0,0));
                    al_draw_textf(fonte1, al_map_rgb(0,0,0), 350,40, 0, "Para instrucoes que contem ");
                    al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,80, 0, "numeros, uitilize as teclas de");
                    al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,120, 0, "esquerda e direita para movimentar");
                    al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,160, 0, "nosso robo!");
                    al_draw_textf(fonteEnter, al_map_rgb(0,0,0), 500,320, 0, "aperte enter para continuar!");
                    //al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Utilize as teclas do teclado");
                    //al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "para mover a caixa de numeros");
                    //al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                    al_flip_display();
                    break;
                    case 2:
                    if(tempo==0){
                    if(caixa.x<LARG-301){
                        status=1;
                    }

                        al_draw_bitmap(instrucao1,0,0,0);
                        al_draw_bitmap(chao,0,ALT-45,0);
                        al_draw_bitmap(direita, LARG-200, 10, 0);
                        if(vivo==true){
                            al_draw_bitmap(chip,x1,y,0);

                            //al_draw_filled_circle(x1, y, raio, al_map_rgb(255, 0, 0));

                        }

                        DesenharCaixa(caixa, fonte, nivel,&status);
                        al_flip_display();
                         y += 4 * dir_y;
                        caixa.x += 5;
                        if(caixa.x>LARG-300){
                        caixa.x = LARG-300;
                        status=0;
                        }
                        if(y>ALT-300 && y<ALT-50){
                            status=5;
                        }
                        if(y==ALT-100){
                        vivo=false;
                        y=0;
                        tempo++;
                        }

                    }
                    vivo=true;
                    if(tempo==1){
                        if(caixa.x>LARG-301){
                        status=2;
                        }
                        al_draw_bitmap(instrucao1,0,0,0);
                        al_draw_bitmap(chao,0,ALT-45,0);

                        al_draw_bitmap(esquerda, LARG-200, 10, 0);
                        if(vivo==true){
                        al_draw_bitmap(chip,x2,y,0);
                        //al_draw_filled_circle(x2, y, raio, al_map_rgb(255, 0, 0));
                        }
                        DesenharCaixa(caixa, fonte, nivel,&status);
                        al_flip_display();
                        y += 4 * dir_y;
                        caixa.x -= 5;
                        if(caixa.x<400){
                        caixa.x = 400;
                        status=0;
                        }
                        if(y>ALT-200 && y<ALT-50){
                            status=5;
                        }
                        if(y==ALT-100){
                        vivo=false;
                        tempo++;
                        parte++;
                        }

                    }

                        break;


                    case 3:
                    status=0;
                    al_draw_bitmap(instrucao1,0,0,0);
                    IniciarCaixa(&caixa, nivel);
                    DesenharCaixa(caixa, fonte, nivel,&status);
                    al_draw_bitmap(chao,0,ALT-45,0);
                    al_draw_bitmap(personagem, -100, 260, 0);
                    al_draw_bitmap(balao, 290, 20, 0);
                    al_draw_bitmap(AED,400,170,0);
                    //al_draw_bitmap(robo, 500, 600, 0);

                    al_draw_textf(fonte1, al_map_rgb(0,0,0), 350,40, 0, "Para instrucoes que contem ");
                    al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,80, 0, "palavras, uitilize as teclas ");
                    al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,120, 0, "A e D para movimentar nosso robo!");
                    al_draw_textf(fonteEnter, al_map_rgb(0,0,0), 500,320, 0, "aperte enter para continuar!");
                    //al_draw_filled_rectangle(LARG/2-200,ALT/2-30, LARG/2+200, ALT/2+50, al_map_rgb(0,0,0));
                    //al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)-25, ALLEGRO_ALIGN_CENTRE, "Utilize as teclas A e D");
                    //al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2), ALLEGRO_ALIGN_CENTRE, "para mover a caixa de palavras");
                    //al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2,(ALT/2)+25, ALLEGRO_ALIGN_CENTRE, "Aperte  ENTER  para  continuar!");
                    al_flip_display();
                    y=0;
                    break;
                    case 4:
                    if(tempo==2){
                        if(caixa.x<x4){
                        status=1;
                        }
                        al_draw_bitmap(instrucao1,0,0,0);
                        al_draw_bitmap(chao,0,ALT-45,0);

                        al_draw_bitmap(D,LARG-200,3,0);
                        if(vivo==true){
                        al_draw_bitmap(coisa,x4,y,0);
                        //al_draw_filled_rectangle(x4-50,y-20, x4+50, y+20, al_map_rgb(0,0,0));
                        }
                        DesenharCaixa(caixa, fonte, nivel,&status);
                        al_flip_display();
                        y += 4 * dir_y;
                        caixa.x += 5;
                        if(caixa.x>LARG-400){
                        caixa.x = LARG-400;
                        status=0;
                        }

                        if(y>ALT-300 && y<ALT-50){
                            status=5;
                        }
                        if(y==ALT-100){
                        vivo=false;
                        y=0;
                        tempo++;
                        }

                    }
                    vivo=true;

                    if(tempo==3){
                        if(caixa.x>x3){
                        status=2;
                        }
                        al_draw_bitmap(instrucao1,0,0,0);
                        al_draw_bitmap(chao,0,ALT-45,0);

                        if(vivo==true){
                        al_draw_bitmap(coisa,x5,y,0);
                        //al_draw_filled_rectangle(x3-50,y-20, x3+50, y+20, al_map_rgb(0,0,0));
                        }
                        DesenharCaixa(caixa, fonte, nivel,&status);
                        al_draw_bitmap(A,LARG-200,3,0);
                        al_flip_display();
                        y +=4 * dir_y;
                        caixa.x -= 5;
                        if(caixa.x<x3){
                        caixa.x = x3;
                        status=0;
                        }
                        if(y>ALT-200 && y<ALT-50){
                            status=5;
                        }
                        if(y==ALT-100){
                        vivo=false;
                        tempo++;
                        parte++;
                        }
                    }
                    break;
                    case 5:
                        al_draw_bitmap(instrucao,0,0,0);
                        al_draw_bitmap(robo, 500, 600, 0);
                        al_draw_bitmap(personagem, -100, 260, 0);
                        al_draw_bitmap(balao, 290, 20, 0);
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 350,40, 0, "Entendeu como se faz?");
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,80, 0, "Nao perca mais tempo e ajude a ");
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,120, 0, "recuperar a memoria do nosso amigo");
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,160, 0, "Donnnie! O futuro dele esta em");
                        al_draw_textf(fonte1, al_map_rgb(0,0,0), 320,200, 0, "suas maos!");
                        al_draw_textf(fonteEnter, al_map_rgb(0,0,0), 480,320, 0, "aperte enter para voltar ao menu");

                    al_flip_display();

                    break;
                    case 6:
                    Jogo=0;
                    al_clear_to_color(al_map_rgb(0,0,0));
                    al_draw_textf(fonte, al_map_rgb(255,255,255), LARG/2-50, ALT/2, 0, "Carregando...");
                    al_flip_display();
                    al_destroy_bitmap(instrucao);
                    al_destroy_bitmap(instrucao1);
                    al_destroy_bitmap(teclas);
                    al_destroy_bitmap(direita);
                    al_destroy_bitmap(esquerda);
                    al_destroy_bitmap(AED);
                    al_destroy_bitmap(A);
                    al_destroy_bitmap(D);
                    al_destroy_bitmap(personagem);
                    al_destroy_bitmap(balao);
                    al_destroy_bitmap(robo);
                    break;

                }

            }
                    if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                return 0;

        }







        }
    }
    al_destroy_display(janela);
    al_destroy_bitmap(imagem);
    al_destroy_bitmap(corrente);
    al_destroy_bitmap(chao);
    al_destroy_sample(acertou);
    al_destroy_sample(errou);
    al_destroy_sample(musicaf1);
    al_destroy_sample(musicaf2);
    al_destroy_sample(musicaf3);
    al_destroy_sample(musicaf4);
    al_destroy_sample(musicaf5);
    al_destroy_sample(musicaf6);
    al_destroy_sample(musicaf7);
    al_destroy_sample_instance(musicaIf1);
    al_destroy_sample_instance(musicaIf2);
    al_destroy_sample_instance(musicaIf3);
    al_destroy_sample_instance(musicaIf4);
    al_destroy_sample_instance(musicaIf5);
    al_destroy_sample_instance(musicaIf6);
    al_destroy_sample_instance(musicaIf7);
    return 0;
}
