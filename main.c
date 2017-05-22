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
	int p1[2],p2[2], codeclient[4],codeserver[4], pid, i, j, k, clientpid, serverpid,rc=0,rp=0;
	pipe(p1);
	pipe(p2);
	pid = fork(); 
	//while(1==1)
		if (0==pid)
		{
			//cliente
			int colorc, posc;
			clientpid=getpid();
			write(p1[1], &clientpid, sizeof(&clientpid));
			sleep(4);
			read(p2[0],&serverpid,sizeof(&serverpid));
			printf("waiting for signal");
			fflush(stdout);
			getchar();
			kill(serverpid, SIGCONT);
			pause();
			for(i=0;i<10;i++)//game cycle for client
			{
				for(j=0;j<4;j++)//code choosing
				{	//escolhe codigo
					do
					{
						printf("(Client)Insira digito %d\n",(j+1));
						fflush(stdout);
						scanf("%d", &codeclient[j]);
					} 
					while(codeclient[j]<0 || codeclient[j]>5);
					//passa controlo para o cliente (kil e depois pause)
				}
				write(p1[1], codeclient, sizeof(codeclient));//escreve tentativa na pipe
				kill(serverpid,SIGCONT);//passa controlo e pausa
				pause();
				printf("Switched to client for new try.\n");
				fflush(stdout);
				read(p2[0],&posc, sizeof(&posc));
				read(p2[0],&colorc, sizeof(&colorc));
				printf("Tem %d cores em %d posicoes corretas.\n", colorc, posc);
				fflush(stdout);
				if(posc==4)
				{
					printf("Parabens! Voce venceu o jogo do Mastermind!\n");
					kill(serverpid,SIGSTOP);
					return 1;
				}
				else
					printf("Tem %d cores em %d posicoes corretas.\n", colorc, posc);
					fflush(stdout);
			}
			kill(serverpid,SIGCONT);
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
			write(p2[1],&serverpid,sizeof(&serverpid));
			pause();
			read(p1[0],&clientpid,sizeof(&clientpid));
			for(i=0;i<4;i++)//code choosing
			{	//escolhe codigo
				do
				{
					printf("(Server)Insira digito %d\n",(i+1));
					fflush(stdout);
					scanf("%d", &codeserver[i]);
				} while(codeserver[i]<0 || codeserver[i]>5);
			}
			i=0;
			kill(clientpid,SIGCONT);
			pause();//passa controlo para o cliente (kil e depois pause)
			fflush(stdout);
			//server checks if code is right
			for(i=0;i<10;i++)
			{	
				rc=0;
				rp=0;
				fflush(stdout);
				read(p1[0], codeclient, sizeof(codeclient));
				for(k=0;k<4;k++)
				{
					for(int l=0;l<4;l++)
					{
						if(codeclient[l]==codeserver[l])
						{
							if(l==k)
							{
								rp++;
								rc++;
								break;
							}
							rc++;
						}
					}	
				}
				fflush(stdout);
				if(rc>4)
					rc=4;
				printf("O jogador tem %d cores em %d posicoes corretas.\n", rc, rp);
				write(p2[1],&rp,sizeof(&rp));
				sleep(1);
				write(p2[1],&rc,sizeof(&rc));
				rc=0;
				rp=0;
				printf("Switching to client for new try.\n");
				fflush(stdout);
				kill(clientpid,SIGCONT);
				pause();//shift control
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
