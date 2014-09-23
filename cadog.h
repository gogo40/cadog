#ifndef __CADOG__H
#define __CADOG__H


#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <map>
#include <cmath>
#include <cstring>

using namespace std;

struct point{
	int x, y;
};
//pos                   0, 1, 2,  3,  4,  5, 6, 7, 8, 9
static const int dx[]={ 1, 1, 0, -1, -1, -1, 0, 1, 0, 2};
static const int dy[]={ 0,-1,-1, -1,  0,  1, 1, 1, 0, 2};

//o que o cara digita       0, 1, 2, 3, 4, 5, 6, 7, 8, 9
static const int mapaDig[8]={ 2, 1, 0, 7, 6, 5, 4, 3};

/*

descricao objeto:
<simbolo objeto> <Nome da malha associada ao objeto> <Nome do material usado> px py pz sx sy sz yaw pitch roll

Alfabeto do tabuleiro:

c - gato
r - rato
# - parede
. - chao livre
* - obstrucao invisivel
q - queijo

*/

struct ObjProp{
    string MashName;//Nome da malha associada ao objeto
    string MaterialName;//Nome do material usado
    double px, py, pz;//posicao relativa
    double sx, sy, sz;//fatores de escala
    double yaw, pitch, roll;//fatores de rotacao
};


struct Cadog{
    double Dx, Dy, Dz; //tamanhos das celulas

    map<char,ObjProp> Lobjs;
	std::vector<string> T;//Tabuleiro do jogo
	double dif;
    int direcao;
    point posPlayer;//Aviso: só acesse essa variavel depois de ter usado a funcao load!!!
    point posEnemy;//Aviso: só acesse essa variavel depois de ter usado a funcao load!!!
    int stat;
    int N, M;//Numero de linhas e colunas do tabuleiro
    int nmoves;//Numero de movimentos
    int distOgro;//Distancia do ogro para jogador

	int H(point po, point pf);
	int dist(point po, point pf);
    int status();//Verifica estado do jogo: 1- jogador venceu 0- jogo está rolando -1 - jogador perdeu

	void load(const char* fname);//carrega tabuleiro
	bool move(point po, int mov, point& d,bool& tel);//efetua movimento do jogador, retorna true se movimento eh valido
	//Tel diz se foi usado teletransporte
	void moveAI();//efetua movimento da ai;
	void print();//Imprime estado do jogo

	point locateP();//localiza jogador
	point locateAI();
};

#endif
