#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
const int LARGURA_TELA = 800;
const int ALTURA_TELA = 600;

int main()
{
	ALLEGRO_DISPLAY *janela = NULL;
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

	ALLEGRO_BITMAP *Menu = NULL, *Menu1 = NULL,*Menu2 = NULL,*Menu3 = NULL,*Sair = NULL;
	bool sair = false;
    int regiao = 0;
	//const int x = 328, xx = 523, y = 202, yy = 289;
	if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return -1;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return -1;
    }
    if(!al_install_mouse())
    {
        return -1;
    }
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return -1;
    }
    al_set_window_title(janela, "Teste");
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    Menu = al_load_bitmap("Menu.jpg");
    Menu1= al_load_bitmap("Menu1.jpg");
    Menu2= al_load_bitmap("Menu2.jpg");
    Menu3= al_load_bitmap("Menu3.jpg");
    Sair = al_load_bitmap("Sair.jpg");
    while(!sair)
    {    
        if(regiao == 4)
            al_draw_bitmap(Sair,0,0,0);
        if(regiao == 3)
            al_draw_bitmap(Menu3,0,0,0);
        if(regiao == 2)
            al_draw_bitmap(Menu2,0,0,0);
    	if(regiao == 1)
    		al_draw_bitmap(Menu1,0,0,0);
    	if(regiao == 0)
    		al_draw_bitmap(Menu,0,0,0);
    	al_flip_display();
    	ALLEGRO_EVENT evento;
    	al_wait_for_event(fila_eventos, &evento);
    	if(evento.type == ALLEGRO_EVENT_MOUSE_AXES)
    	{
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
        if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if(regiao == 4)
                sair = true;
        }
    	if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            sair = true;
        }
    }
    al_destroy_bitmap(Menu);
    al_destroy_bitmap(Menu1);
    al_destroy_bitmap(Menu2);
    al_destroy_bitmap(Menu3);
    al_destroy_bitmap(Sair);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);
    return 0;
}
