#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
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



void PaineldeInfo(int acertos, int nivel, CaixaC caixa, ALLEGRO_FONT *fonte);
void IniciarCaixa(CaixaC *caixa,int nivel);
void DesenharCaixa(CaixaC caixa, ALLEGRO_FONT *fonte, int nivel, int *status);
void MoverCaixaEsquerda(CaixaC *caixa,int nivel);
void MoverCaixaDireita(CaixaC *caixa,int nivel);

void IniciarCaixaP(CaixaC *caixa,int nivel);
void DesenharCaixaP(CaixaC caixa, ALLEGRO_FONT *fonte, int nivel, int *status);
void MoverCaixaEsquerdaP(CaixaC *caixa,int nivel);
void MoverCaixaDireitaP(CaixaC *caixa,int nivel);

void ColisaoNumeros(Numeros numero[], int tamanhoN, CaixaC *caixa, int nivel, int *acertos);
void IniciarNumero(Numeros numero[], int tamanho, int nivel);
void DesenharNumero(Numeros numero[], int tamanho, ALLEGRO_FONT *fonte);
void ComecarNumero(Numeros numero[], int tamanho, int n, int nivel);
void AtualizarNumero(Numeros numero[], int tamanho, CaixaC caixa, int *status);

bool primos(Numeros numero[], int i);
bool quadperfeito(Numeros numero[], int i);

void DefinirPalavras(Palavras palavra[]);
void ColisaoPalavras(Palavras palavra[], int tamanhoN, CaixaC *caixa,CaixaC *caixan, int nivel, int *acertos);
void IniciarPalavra(Palavras palavra[], int tamanho, int nivel);
void DesenharPalavra(Palavras palavra[], int tamanho, ALLEGRO_FONT *fonte);
void ComecarPalavra(Palavras palavra[], int tamanho, int n, int nivel);
void AtualizarPalavra(Palavras palavra[], int tamanho, int *status, CaixaC caixa);

#endif // FUNCTION_H_INCLUDED
