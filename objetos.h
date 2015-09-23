//IDs dos Objetos
enum IDS{CAIXA, NUMERO};

//Caixa
struct CaixaC{
    int ID;
    int x;
    int y;
    int vidas;
    int velocidade;
    int limitex;
    int limitey;
    int pontuacao;
};

struct Numeros{
    int ID;
    int x;
    int y;
    bool vivo;
    int velocidade;
    int limitey;
    int limitex;
    int valor;
};
