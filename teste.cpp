#include "cadog.h"

int main()
{
	Cadog c;
	point pR;
	
	c.load("cena1.txt");
	
	
	c.print();
		
	while(true){
		int cmd;
		
		printf("cmd:");
		scanf("%d",&cmd);
		
		pR=c.locateP();
	
		if(cmd>0&&cmd<10){
			c.move(pR,cmd);
		}
		c.moveAI();
		
		int status;
		status=c.status();
		
		if(status!=0){
			if(status==1){
				printf("Jogador venceu!\n");
				return 0;
			}else{
				printf("Maquina venceu!\n");
				return 0;
			}
		}
		c.print();
	}
	return 0;
}