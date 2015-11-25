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

int frameatual = 0, framecontador = 0, framecd = 1, faltura,flargura,numframes;
int frameatualp = 0, framecontadorp = 0, framecdp = 1, falturap,flargurap,numframesp;
void PaineldeInfo(int acertos, int nivel, CaixaC caixa, ALLEGRO_FONT *fonte){
    al_draw_filled_rectangle(3,3, 120, 110, al_map_rgb(0,0,0));
    al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 5, 0, "Nivel  %i", nivel);
    al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 30, 0, "Vidas:  %i",caixa.vidas);
    al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 55, 0, "Pontos:  %i",caixa.pontuacao);
    al_draw_textf(fonte, al_map_rgb(255,255,255), 5, 80, 0, "Acertos:  %i/10",acertos);
}
//Parâmetros da Caixa e CaixaP(alavra)

void IniciarCaixa(CaixaC *caixa, int nivel){
    caixa->x = LARG / 2;
    caixa->y = ALT - 10;
    caixa->ID = 12;
    caixa->vidas = 3;
    caixa->velocidade = 10;
    caixa->limitex = 22;
    caixa->limitey = 85;
    caixa->pontuacao = 0;
}

void DesenharCaixa(CaixaC caixa, ALLEGRO_FONT *fonte, int nivel, int *status){
    ALLEGRO_BITMAP *imagem = NULL;
    switch(*status){ //0: Parado / 1: Andando Direita / 2: Andando Esquerda /
                    //3: Fantasma Direita /  4: Fantasma Esquerdo / 5: Olhar para cima direita /
    case 0:
        imagem = al_load_bitmap("sprites/frente.png");
        numframes = 5;
        flargura = 65;
        faltura = 76;
        break;
    case 1:
        imagem = al_load_bitmap("sprites/cu.png");
        numframes = 4;
        flargura = 63;
        faltura = 81;
        break;
    case 2:
        imagem = al_load_bitmap("sprites/Cuesquerdo.png");
        numframes = 4;
        flargura = 63;
        faltura = 81;
        break;
    case 3:
        imagem = al_load_bitmap("sprites/fantasma-atualizado-D.png");
        numframes = 14;
        flargura = 67;
        faltura = 72;
        break;
    case 4:
        imagem = al_load_bitmap("sprites/fantasma-atualizado-E.png");
        numframes = 14;
        flargura = 67;
        faltura = 72;
        break;
    case 5:
        imagem = al_load_bitmap("sprites/ceu.png");
        numframes = 8;
        flargura = 65;
        faltura = 78;
        break;
    }
    //al_draw_filled_rectangle(caixa.x-caixa.limitex-30,caixa.y-caixa.limitey-100,caixa.x+caixa.limitex+30,caixa.y-caixa.limitey-5, al_map_rgb(0,0,0));
    if(++framecontador%5 == framecd){
        if(++frameatual >= numframes){
            if(*status==5 || *status==6)
                *status = 0;
            frameatual=0;
        }
        al_draw_bitmap_region(imagem,frameatual*flargura,0,flargura,faltura,caixa.x-(1.5*caixa.limitex),caixa.y-100,0);
    }
    al_draw_bitmap_region(imagem,frameatual*flargura,0,flargura,faltura,caixa.x-(1.5*caixa.limitex),caixa.y-100,0);

    //PS: Nivel 1 - Pares
    //    Nivel 2 - Substantivos
    //    Nivel 3 - Numerais/Divisiveis por 5
    //    Nivel 4 - Adjetivos/Divisiveis por 3
    //    Nivel 5 - Preposições/Primos
    //    Nivel 6 - Verbos/Divisiveis por 7
    //    Nivel 7 - Adverbios/Quadrados Perfeitos
}

void MoverCaixaEsquerda(CaixaC *caixa,int nivel){
    caixa->x -= caixa->velocidade;
    if(nivel==1){
        if(caixa->x<22){
            caixa->x = 22;
        }
    }else{
        if(caixa->x<(LARG/2)+22){
            caixa->x = (LARG/2)+22;
        }
    }
}
void MoverCaixaDireita(CaixaC *caixa,int nivel){
    caixa->x += caixa->velocidade;
    if(caixa->x>LARG-22){
        caixa->x = LARG-22;
    }
}

void IniciarCaixaP(CaixaC *caixa, int nivel){
    caixa->x = LARG / 2;
    caixa->y = ALT - 10;
    caixa->ID = 12;
    caixa->vidas = 3;
    caixa->velocidade = 10;
    caixa->limitex = 22;
    caixa->limitey = 85;
    caixa->pontuacao = 0;
}

void DesenharCaixaP(CaixaC caixa, ALLEGRO_FONT *fonte, int nivel, int *status){
    ALLEGRO_BITMAP *imagem = NULL;
    switch(*status){ //0: Parado / 1: Andando Direita / 2: Andando Esquerda /
                    //3: Fantasma Direita /  4: Fantasma Esquerdo / 5: Olhar para cima direita /
    case 0:
        imagem = al_load_bitmap("sprites/frente.png");
        numframesp = 5;
        flargurap = 65;
        falturap = 76;
        break;
    case 1:
        imagem = al_load_bitmap("sprites/cu.png");
        numframesp = 4;
        flargurap = 63;
        falturap = 81;
        break;
    case 2:
        imagem = al_load_bitmap("sprites/Cuesquerdo.png");
        numframesp = 4;
        flargurap = 63;
        falturap = 81;
        break;
    case 3:
        imagem = al_load_bitmap("sprites/fantasma-atualizado-D.png");
        numframesp = 14;
        flargurap = 67;
        falturap = 72;
        break;
    case 4:
        imagem = al_load_bitmap("sprites/fantasma-atualizado-E.png");
        numframesp = 14;
        flargurap = 67;
        falturap = 72;
        break;
    case 5:
        imagem = al_load_bitmap("sprites/ceu.png");
        numframesp = 8;
        flargurap = 65;
        falturap = 78;
        break;
    }
    //al_draw_filled_rectangle(caixa.x-caixa.limitex-30,caixa.y-caixa.limitey-100,caixa.x+caixa.limitex+30,caixa.y-caixa.limitey-5, al_map_rgb(0,0,0));
    if(++framecontadorp%5 == framecdp){
        if(++frameatualp >= numframesp){
            if(*status==5 || *status==6)
                *status = 0;
            frameatualp=0;
        }
        al_draw_bitmap_region(imagem,frameatual*flargura,0,flargura,faltura,caixa.x-(1.5*caixa.limitex),caixa.y-100,0);
    }
    al_draw_bitmap_region(imagem,frameatual*flargura,0,flargura,faltura,caixa.x-(1.5*caixa.limitex),caixa.y-100,0);

    //PS: Nivel 1 - Pares
    //    Nivel 2 - Substantivos
    //    Nivel 3 - Numerais/Divisiveis por 5
    //    Nivel 4 - Adjetivos/Divisiveis por 3
    //    Nivel 5 - Preposições/Primos
    //    Nivel 6 - Verbos/Divisiveis por 7
    //    Nivel 7 - Adverbios/Quadrados Perfeitos
}
void MoverCaixaEsquerdaP(CaixaC *caixa,int nivel){
    caixa->x -= caixa->velocidade;
    if(caixa->x<60){
        caixa->x = 60;
    }
}
void MoverCaixaDireitaP(CaixaC *caixa,int nivel){
    caixa->x += caixa->velocidade;
    if(nivel==2){
        if(caixa->x>LARG-60){
            caixa->x = LARG-60;
        }
    }else{
        if(caixa->x>(LARG/2)-60){
            caixa->x = (LARG/2)-60;
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
            }else if(numero[i].y >= ALT-60){
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
        case 3:
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
        case 4:
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
        case 5:
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
        case 6:
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
        case 7:
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

//Parâmetros dos Números
void IniciarNumero(Numeros numero[], int tamanho, int nivel){
    int i;
    for(i = 0; i < tamanho; i++){
        numero[i].ID = rand();
        numero[i].vivo = false;
        numero[i].velocidade = 3;
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
    int i,certo;
    for(i = 0; i < tamanho; i++){
        if(!numero[i].vivo){
            if(n % 100 == 0){
                numero[i].vivo = true;
                if(nivel<3){
                    numero[i].x = 40 + rand() % (LARG - 60);
                    numero[i].y = 0;
                }else{
                    numero[i].x = ((LARG/2)+40) + rand() % ((LARG/2) - 60);
                    numero[i].y = 0;
                }
                certo = rand() % 2;
                switch(nivel){
                    case 1:
                        numero[i].valor = rand() % 1000;
                        if(numero[i].valor % 2 != 0){
                            if(certo==1)
                                numero[i].valor++;
                        }else{
                            if(certo!=1)
                                numero[i].valor++;
                        }
                        break;
                    case 3:
                        numero[i].valor = rand() % 1000;
                        if(numero[i].valor % 5 != 0){
                            if(certo==1){
                                while(numero[i].valor % 5 != 0)
                                    numero[i].valor++;
                            }
                        }else{
                            if(certo!=1)
                                numero[i].valor++;
                        }
                        break;
                    case 4:
                        numero[i].valor = rand() % 1000;
                        if(numero[i].valor % 3 != 0){
                            if(certo==1){
                                while(numero[i].valor % 3 != 0)
                                    numero[i].valor++;
                            }
                        }else{
                            if(certo!=1)
                                numero[i].valor++;
                        }
                        break;
                    case 5:
                        numero[i].valor = rand() % 70;
                        if(!primos(numero,i)){
                            if(certo==1){
                                while(!primos(numero,i))
                                    numero[i].valor--;
                            }
                        }else{
                            if(certo!=1)
                                numero[i].valor++;
                        }
                        break;
                    case 6:
                        numero[i].valor = rand() % 1000;
                        if(numero[i].valor % 7 != 0){
                            if(certo==1){
                                while(numero[i].valor % 7 != 0)
                                    numero[i].valor++;
                            }
                        }else{
                            if(certo!=1)
                                numero[i].valor++;
                        }
                        break;
                    case 7:
                        numero[i].valor = rand() % 1000;
                        if(!quadperfeito(numero,i)){
                            if(certo==1){
                                while(!quadperfeito(numero,i)){
                                    if(numero[i].valor % 2 == 0)
                                        numero[i].valor-=3;
                                    else
                                        numero[i].valor-=2;
                                }
                            }
                        }else{
                            if(certo!=1)
                                numero[i].valor++;
                        }
                        break;
                }
                break;
            }
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
    palavra[51].palavra = "Tres";
    palavra[52].palavra = "Primeiro";
    palavra[53].palavra = "Segundo";
    palavra[54].palavra = "Trinta";
    palavra[55].palavra = "dois";
    palavra[56].palavra = "Duplo";
    palavra[57].palavra = "Dobro";
    palavra[58].palavra = "Metade";
    palavra[59].palavra = "Meio";
    palavra[60].palavra = "Dupla";
    palavra[61].palavra = "Terceiro";
    palavra[62].palavra = "Vinte";
    palavra[63].palavra = "Triplo";
    palavra[64].palavra = "Trio";
    palavra[65].palavra = "Terço";
    palavra[66].palavra = "Oitenta";
    palavra[67].palavra = "Quatro";
    palavra[68].palavra = "Quarto";
    palavra[69].palavra = "Quarteto";
    palavra[70].palavra = "Sete";
    palavra[71].palavra = "Sétimo";
    palavra[72].palavra = "Noventa";
    palavra[73].palavra = "Cinquenta";
    palavra[74].palavra = "Oitavo";
    palavra[75].palavra = "Treze";
    palavra[76].palavra = "Doze";
    palavra[77].palavra = "Dúzia";
    palavra[78].palavra = "Meio";
    palavra[79].palavra = "Mil";
    palavra[80].palavra = "Novecentos";
    palavra[81].palavra = "Milésimo";
    palavra[82].palavra = "Centésimo";
    palavra[83].palavra = "Milhar";
    palavra[84].palavra = "Trigésimo";
    palavra[85].palavra = "Dez";
    palavra[86].palavra = "Sessenta";
    palavra[87].palavra = "Duzentos";
    palavra[88].palavra = "Cem";
    palavra[89].palavra = "Centena";
    palavra[90].palavra = "Milhão";
    palavra[91].palavra = "Trezentos";
    palavra[92].palavra = "Oitocentos";
    palavra[93].palavra = "Quatorze";
    palavra[94].palavra = "Quinze";
    palavra[95].palavra = "Dezesseis";
    palavra[96].palavra = "Onze";
    palavra[97].palavra = "Dezoito";
    palavra[98].palavra = "Seis";
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
    palavra[115].palavra = "Serio";
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
    palavra[129].palavra = "Rigido";
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
    palavra[148].palavra = "Timido";
    palavra[149].palavra = "Gentil";

    //PREPOSICOES
    /*Preposição é a palavra que estabelece uma relação entre dois ou mais termos da oração.
    Essa relação é do tipo subordinativa, ou seja, entre os elementos ligados pela preposição
    não há sentido dissociado, separado, individualizado; ao contrário, o sentido da expressão
    é dependente da união de todos os elementos que a preposição vincula.*/

    palavra[150].palavra = "a";
    palavra[151].palavra = "ante";
    palavra[152].palavra = "apos";
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
    palavra[165].palavra = "tras";
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
    palavra[178].palavra = "alem de";
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
    palavra[189].palavra = "gracas a";
    palavra[190].palavra = "sob pena de";
    palavra[191].palavra = "a respeito de";
    palavra[192].palavra = "ao encontro de";
    palavra[193].palavra = "junto com";
    palavra[194].palavra = "junto de";
    palavra[195].palavra = "junto a";
    palavra[196].palavra = "defronte de";
    palavra[197].palavra = "atraves de";
    palavra[198].palavra = "gracas a";
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
    palavra[211].palavra = "Forcar";
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
    palavra[254].palavra = "Alem";
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
    palavra[267].palavra = "Jamais";
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
    palavra[283].palavra = "Bastante";
    palavra[284].palavra = "Menos";
    palavra[285].palavra = "Ainda";
    palavra[286].palavra = "com";
    palavra[287].palavra = "Somente";
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
    palavra[298].palavra = "Rapidamente";
    palavra[299].palavra = "Ultimamente";
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
            }else if(palavra[i].y == ALT-60){
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

void IniciarPalavra(Palavras palavra[], int tamanho, int nivel){
    int i;
    for(i = 0; i < tamanho; i++){
        palavra[i].vivo = false;
        palavra[i].velocidade = 3;
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
    int i,aux,certo;
    for(i = 0; i < tamanho; i++){
        if(!palavra[i].vivo){
            if(n % 100 == 0){
                certo = rand() % 2;
                if(certo==1){
                    switch(nivel){
                    case 2:
                        aux = rand() % 50;
                        break;
                    case 3:
                        aux = rand() % 50 + 50;
                        break;
                    case 4:
                        aux = rand() % 50 + 100;
                        break;
                    case 5:
                        aux = rand() % 50 + 150;
                        break;
                    case 6:
                        aux = rand() % 50 + 200;
                        break;
                    case 7:
                        aux = rand() % 50 + 250;
                        break;
                    }
                }else{
                    aux = rand() % 300;
                    switch(nivel){
                    case 2:
                        if(aux>=0 && aux<50)
                            aux+=50;
                        break;
                    case 3:
                        if(aux>=50 && aux<100)
                            aux+=50;
                        break;
                    case 4:
                        if(aux>=100 && aux<150)
                            aux+=50;
                        break;
                    case 5:
                        if(aux>=150 && aux<200)
                            aux+=50;
                        break;
                    case 6:
                        if(aux>=200 && aux<250)
                            aux+=50;
                        break;
                    case 7:
                        if(aux>=250 && aux<300)
                            aux-=50;
                        break;
                    }
                }

                palavra[aux].vivo = true;
                if(nivel<3){
                    palavra[aux].x = 100 + rand() % (LARG - 100);
                    palavra[aux].y = 0;
                }else{
                    palavra[aux].x = 100 + rand() % ((LARG/2) - 150);
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
