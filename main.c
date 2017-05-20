#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#define sz sizeof(int)

#define clrscr() printf("\033[H\033[J") //limpa ecra

#define gotoxy(y,x) printf("\033[%d;%dH", (x), (y)) //reposiciona cursor

#define RED     "\x1b[31m"  //coloring
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define COLORRESET   "\x1b[0m"

void handler(int signum) //Sig handler
{
}

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
	signal(SIGCONT, handler);
	int p1[2],p2[2], pid, i, j;
	pipe(p1);
	pipe(p2);
	pid = fork();
	int code[4],codeserver[4];
	int max,r1,r2, clientpid, serverpid;
	//while(1==1)
		if (0==pid)
		{
			//cliente
			clientpid=getpid();
			write(p1[1], &clientpid, sizeof(&clientpid));
			sleep(4);
			read(p2[0],&serverpid,sizeof(&serverpid));
			printf("waiting for signal");
			getchar();
			kill(serverpid, SIGCONT);
			pause();
			for(i=0;i<10;i++)//game cycle for client
			{
				printf("Tentativa %d.",(i+1));	//faz uma tentativa
				for(j=0;j<4;j++)//code choosing
			{	//escolhe codigo
				do
				{
					printf("Insira digito %d\n",(j+1));
					scanf("%d", &code[j]);
				} while(code[j]>0 && code[j]<5);
				write(p1[1], &code, sizeof(&code));
				kill(serverpid,SIGCONT);
				pause();//passa controlo para o cliente (kil e depois pause)
			}
			}	//escreve tentativa na pipe
				//passa controlo

				//le feedback e escreve no ecra
				//volta a enviar tentativa


			/*printf("Introduza o valor(Client): \n");
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
			sleep(1);*/
		}
		else
		{
			//server
			serverpid=getpid();
			sleep(2);
			read(p1[0],&clientpid,sizeof(&clientpid));
			write(p2[1],&serverpid,sizeof(&serverpid));
			pause();
			for(i=0;i<4;i++)//code choosing
			{	//escolhe codigo
				do
				{
					printf("Insira digito %d\n",(i+1));
					scanf("%d", &code[i]);
				} while(code[i]>0 && code[i]<5);
			}
			kill(clientpid,SIGCONT);
			pause();//passa controlo para o cliente (kil e depois pause)

			//game cycle for sv
			for(i=0;i<10;i++)//game cycle for client
			{	
				read(p1[0],&codeserver,sizeof(&codeserver));
				//testa
				//mostra feedback
				//shift control
			}
			/*printf("1S\n");
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
			fflush(stdout);*/
		}
	return 0;
}
