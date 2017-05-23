#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#define clrscr() printf("\033[H\033[J") //limpa ecra

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

void jogo();

int main()
{
	int x;
	clrscr();
    	printf("Mastermind - Game\n\n");
	printf("Insira um numero de 1 a 5: ");
    	printf("\n\n1 - Jogar contra pc.\n");
    	printf("2 - ...\n");
    	printf("3 - ...\n");
    	printf("4 - ...\n");
    	printf("5 - Sair\n\n");
    	scanf("%d",&x);
    	printf("\n\n");
	switch(x)
	{
		case 1 :
		    	jogo();
			break;
		case 2 :
			break;
		case 3 :
			break;
		case 4 :
			break;
		case 5 :
			break;
	}
}

void jogo()
{
	signal(SIGCONT, handler);
	int p1[2],p2[2], codeclient[4],codeserver[4], colors[6]={0,0,0,0,0,0}, checkp[4]={0,0,0,0}, pid, i, j, k, clientpid, serverpid,rc=0,rp=0;
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
			sleep(1);
			read(p2[0],&serverpid,sizeof(&serverpid));
			kill(serverpid, SIGCONT);
			pause();
			for(int i=0;i<10;i++)//game cycle for client
			{
				for(j=0;j<4;j++)//code choosing
				{	//escolhe codigo
					do
					{
						printf(COLORRESET"Insira cor %d\n",(j+1));
						fflush(stdout);
						printf(RED"0 - Vermelho\n");
						fflush(stdout);
						printf(GREEN"1 - Verde\n");
						fflush(stdout);
						printf(YELLOW"2 - Amarelo\n");
						fflush(stdout);
						printf(BLUE"3 - Azul\n");
						fflush(stdout);
						printf(MAGENTA"4 - Magenta\n");
						fflush(stdout);
						printf(CYAN"5 - Ciano\n");
						fflush(stdout);
						printf(COLORRESET"");
						fflush(stdout);
						scanf("%d", &codeclient[j]);
					} 
					while(codeclient[j]<0 || codeclient[j]>5);
					//passa controlo para o cliente (kil e depois pause)
				}
				write(p1[1], codeclient, sizeof(codeclient));//escreve tentativa na pipe
				kill(serverpid,SIGCONT);//passa controlo e pausa
				pause();
				fflush(stdout);
				read(p2[0],&posc, sizeof(&posc));
				read(p2[0],&colorc, sizeof(&colorc));
				fflush(stdout);
				if(posc==4)
				{
					printf(COLORRESET"Parabens! Voce venceu o jogo do Mastermind!\n");
					kill(serverpid,SIGTERM);
					return ;
				}
				else
				{
					printf(COLORRESET"Tem %d cores em %d posicoes corretas(%d).\n", colorc, posc, (i+1));
					fflush(stdout);
				}
			}
			printf(RED"Esgotaram-se as tentativas...\n\nGAME OVER\n\n");
			sleep(1);
			kill(serverpid,SIGTERM);
		}
		else
		{
			//server
			serverpid=getpid();
			sleep(1);
			write(p2[1],&serverpid,sizeof(&serverpid));
			pause();
			read(p1[0],&clientpid,sizeof(&clientpid));
			srand(time(NULL));
			for(int a=0;a<4;a++)//code choosing
				codeserver[a]= rand()%6;
			for(int c=0;c<4;c++)
				printf("%d\n",codeserver[c]);
			kill(clientpid,SIGCONT);
			pause();//passa controlo para o cliente (kil e depois pause)
			//server checks if code is right
			for(int b=0;b<10;b++)
			{	
				rc=0;
				rp=0;
				fflush(stdout);
				read(p1[0], codeclient, sizeof(codeclient));
				for(k=0;k<4;k++)
				{
					for(int l=0;l<4;l++)
					{
						if(codeclient[k]==codeserver[l])
						{
							if(colors[codeclient[k]]==0)
							{
								rc++;
								printf("%d bem.\n",rc);
								fflush(stdout);
								colors[codeclient[k]]=1;
							}
							if(l==k)
							{
								if(checkp[k]==0)
								{
									rp++;
									printf("%d no sitio certo.\n",rp);
									fflush(stdout);
									checkp[k]=1;
									break;
								}
							}
						}
					}	
				}
				colors[0]=0;
				colors[1]=0;
				colors[2]=0;
				colors[3]=0;
				colors[4]=0;
				colors[5]=0;
				checkp[0]=0;
				checkp[1]=0;
				checkp[2]=0;
				checkp[3]=0;
				write(p2[1],&rp,sizeof(&rp));
				sleep(1);
				write(p2[1],&rc,sizeof(&rc));
				kill(clientpid,SIGCONT);
				pause();//shift control
			}
		}
	return ;
}
