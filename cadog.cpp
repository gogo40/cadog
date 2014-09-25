#include "cadog.h"

void Cadog::load(const char* fname)//carrega tabuleiro
{
	ifstream in(fname);

	if(in.fail()){
		cerr<<"Falha ao abrir:"<<fname<<"!"<<endl;
		exit(1);
	}

    in>>Dx>>Dy>>Dz;

	Lobjs.clear();

    int nObjs;
    string s;

    ObjProp o;
/*
   string MashName;//Nome da malha associada ao objeto
    string MaterialName;//Nome do material usado
    double px, py, pz;//posicao relativa
    double sx, sy, sz;//fatores de escala
    double yaw, pitch, roll;//fatores de rotacao
*/
    in>>nObjs;
    while(nObjs>0){
        in>>s>>o.MashName>>o.MaterialName;
        in>>o.px>>o.py>>o.pz;
        in>>o.sx>>o.sy>>o.sz;
        in>>o.yaw>>o.pitch>>o.roll;
        Lobjs[s[0]]=o;
        nObjs--;
    }

    in>>N>>M>>dif>>direcao;

    T.resize(N);

	for(int i=0;i<N;i++) in>>T[i];

	posPlayer=locateP();
	posEnemy=locateAI();
	stat=0;
	nmoves=0;
	distOgro=-1;
}

bool Cadog::move(point po, int mov, point& d, bool& tel)//efetua movimento do jogador
{

	point pf;

//Verifica se foi usado teletransporte
	pf.x=po.x+dx[mapaDig[mov]];
	pf.y=po.y+dy[mapaDig[mov]];

	if(pf.x<=-1 || pf.x>=N || pf.y<=-1||pf.y>=M) tel=true;
	else tel=false;
//Efetua movimento
	pf.x=(po.x+dx[mapaDig[mov]]+N)%N;
	pf.y=(po.y+dy[mapaDig[mov]]+M)%M;

	if(pf.x>-1 && pf.x<N && pf.y>-1&&pf.y<M)
		if(T[pf.x][pf.y]=='.' ||T[pf.x][pf.y]=='q'){
			T[pf.x][pf.y]='r';
			T[po.x][po.y]='.';
			d=pf;
			nmoves++;
			return true;
		}
    return false;
}

#define abs(x) (((x)<0)?(-(x)):(x))

int Cadog::H(point po, point pf){
	int Dx=abs(po.x-pf.x);
	int Dy=abs(po.y-pf.y);
	return (int)(dif*(Dx*Dx+Dy*Dy));
}

int Cadog::dist(point po, point pf){
	int Dx=abs(po.x-pf.x);
	int Dy=abs(po.y-pf.y);
	return Dx*Dx+Dy*Dy;
}

#define INF 1<<29

void Cadog::moveAI()//efetua movimento da ai;
{
	point pR;//posicao rato
	point pC;//posicao gato
	int posR;
	int posC;

	vector<int> vis(N*M);
	vector<int> D(N*M);
	vector<int> pi(N*M);
	priority_queue< pair<int,int> > Q;

    bool find_q;
    bool find_r;
    bool find_c;

    find_r=find_c=find_q=false;
	for(int i=0;i<N;i++)
		for(int j=0;j<M;j++)
			if(T[i][j]=='r'){
				pR.x=i; pR.y=j;
				posR=i*M+j;
				posPlayer=pR;
				find_r=true;
			}else if(T[i][j]=='c'){
				pC.x=i; pC.y=j;
				posC=i*M+j;
				posEnemy=pC;
				find_c=true;
			}else if(T[i][j]=='q') find_q=true;

    if(!find_q && find_r){
        stat=1;
        return;
    }
    if(!find_r){
        stat=-1;
        return;
    }

	for(int i=0;i<N*M;i++){
		vis[i]=0;
		D[i]=INF;
		pi[i]=-1;
	}

	D[posC]=0;
	Q.push(pair<int,int>(-D[posC],posC));

	while(!Q.empty()){
		int u=Q.top().second; Q.pop();
		if(u==posR) break;
		if(vis[u]) continue;

		vis[u]=1;

		int ux, uy;
		point pu;

		ux=u/M; uy=u%M;

		pu.x=ux; pu.y=uy;

		for(int i=0;i<9;i++){
			int x, y, p;
			point pv;

			x=ux+dx[i]; y=uy+dy[i];

			pv.x=x; pv.y=y;

			if(x>-1&&x<N&&y>-1&&y<M){
				p=x*M+y;
				if(D[p]>D[u]+dist(pu,pv) && (T[x][y]=='.' || T[x][y]=='r')){
					int h=D[u]+dist(pu,pv)+H(pv,pR);
					D[p]=D[u]+dist(pu,pv);
					pi[p]=u;
					Q.push(pair<int,int>(-h,p));
				}
			}
		}
	}

    if(pi[posR]==posC) stat=-1;

    if(D[posR]<INF)
        distOgro=D[posR];
    else
        distOgro=-1;

    for(int i=posR;i!=-1;i=pi[i])
		if(i!=-1)
			if(pi[i]==posC){
				int x=i/M;
				int y=i%M;
				int ux=posC/M;
				int uy=posC%M;
				T[x][y]='c';
				T[ux][uy]='.';
				return;
			}

}

int Cadog::status()//Verifica estado do jogo: 1- jogador venceu 0- jogo está rolando -1 - jogador perdeu
{
	return stat;
}

void Cadog::print()
{
	for(int i=0;i<N;i++) cout<<T[i]<<endl;
}

point Cadog::locateP()
{
	point p;
	p.x=-1; p.y=-1;
	for(int i=0;i<N;i++)
		for(int j=0;j<M;j++)
			if(T[i][j]=='r'){
				p.x=i;
				p.y=j;
				return p;
			}
	return p;
}

point Cadog::locateAI()
{
	point p;
	p.x=-1; p.y=-1;
	for(int i=0;i<N;i++)
		for(int j=0;j<M;j++)
			if(T[i][j]=='c'){
				p.x=i;
				p.y=j;
				return p;
			}
	return p;
}
