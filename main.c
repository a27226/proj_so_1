#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#define sz sizeof(int)

int Menu()
{
	int i;
    	printf("Mastermind - Game\n\n");
    	printf("1 - ...\n");
    	printf("2 - ...\n");
    	printf("3 - ...\n");
    	printf("4 - ...\n");
    	printf("5 - ...\n\n");
    	printf("Choose: ");
    	scanf("%d",&i);
    	printf("\n\n");
    	return i;
}

int main()
{
	char code[4];
	int p1[2],p2[2], pid;
	pipe(p1);
	pipe(p2);
	pid = fork();
	int max,r1,r2;
	while(1==1)
	{
		if (0==pid)
		{
			//cliente
			printf("Introduza o valor(Client): \n");
			fflush(stdout);
			scanf("%d",&max);
			write(p1[1],&max,sz);
			read(p2[0],&r1,sz);
			printf("test client\n");
			fflush(stdout);
			printf("%d\n",r1);
			fflush(stdout);
			read(p1[0],&max,sz);
			srand(time(NULL));
			r2=1+rand()%max;
			write(p2[1],&r2,sz);
			printf("Cliente à espera...\n");
			fflush(stdout);
			sleep(1);
		}
		else
		{
			//server
			printf("1S\n");
			read(p1[0],&max,sz);
			printf("2S\n");
			srand(time(NULL));
			r1=1+rand()%max;
			write(p2[1],&r1,sz);
			printf("Servidor à espera...\n");
			fflush(stdout);
			sleep(1);
			printf("Introduza o valor(Server): \n");
			scanf("%d",&max);
			write(p1[1],&max,sz);
			read(p2[0],&r2,sz);
			printf("test server\n");
			fflush(stdout);
			printf("%d\n",r2);
			fflush(stdout);
		}
	}
	return 0;
}
