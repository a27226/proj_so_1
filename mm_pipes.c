#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

#define clrscr() printf("\033[H\033[J") //limpa ecra

#define VERMELHO     "\x1b[31m"  //coloring
#define VERDE   "\x1b[32m"
#define AMARELO  "\x1b[33m"
#define AZUL    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CIANO    "\x1b[36m"
#define DEFAULT   "\x1b[0m"

void handler(int signum) //Sig handler
{
}

void jogocpu();

void jogopvp();

int main()
{
	int x;
	clrscr();
    	printf("Mastermind - Game\n\n");
	printf("Insira uma opcao: ");
    	printf("\n\n1 - Jogar contra pc.\n");
		printf("2 - Jogar contra outro jogador.\n");
    	printf("3 - Sair.\n\n");
    	scanf("%d",&x);
    	printf("\n\n");
	switch(x)
	{
		case 1 :
		{
		    jogocpu();
			break;
		}
		case 2 :
		{
		    jogopvp();
			break;
		}
		case 3 :
			break;
	}
}

void jogocpu()
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
			printf("Bem vindo ao modo PvPC do jogo Mastermind!\nO objectivo do jogo e adivinhar a combinacao correta de cores usando o metodo de tentativa/erro.\nNao se esqueca que pode haver cores repetidas!\nTem 15 tentativas. Quando elas acabarem perde o jogo.\nBoa sorte!\n\n");
			fflush(stdout);
			for(int i=0;i<15;i++)//game cycle for client
			{
				for(j=0;j<4;j++)//code choosing
				{	//escolhe codigo
					do
					{
						printf(DEFAULT"Insira cor %d\n",(j+1));
						fflush(stdout);
						printf(VERMELHO"0 - Vermelho\n");
						fflush(stdout);
						printf(VERDE"1 - Verde\n");
						fflush(stdout);
						printf(AMARELO"2 - Amarelo\n");
						fflush(stdout);
						printf(AZUL"3 - Azul\n");
						fflush(stdout);
						printf(MAGENTA"4 - Magenta\n");
						fflush(stdout);
						printf(CIANO"5 - Ciano\n");
						fflush(stdout);
						printf(DEFAULT"");
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
					printf(VERDE"\n\nParabens! Voce venceu o jogo do Mastermind!\n");
					kill(serverpid,SIGTERM);
					printf(DEFAULT"");
					fflush(stdout);
					return ;
				}
				else
				{
					printf(DEFAULT"Tem %d cores em %d posicoes corretas(Tentativa - %d).\n", colorc, posc, (i+1));
					fflush(stdout);
				}
			}
			printf(VERMELHO"Esgotaram-se as tentativas...\n\nGAME OVER\n\n");
			sleep(1);
			printf(DEFAULT"");
			fflush(stdout);
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
			kill(clientpid,SIGCONT);
			pause();//passa controlo para o cliente (kill e depois pause)
			//server checks if code is right
			for(int b=0;b<15;b++)
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
								colors[codeclient[k]]=1;
							}
							if(l==k)
							{
								if(checkp[k]==0)
								{
									rp++;
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

	return;
}

void jogopvp()
{
	signal(SIGCONT, handler);
	srand(time(NULL));
	int p1[2],p2[2], codeclient[4],codeserver[4], colors[6]={0,0,0,0,0,0}, checkp[4]={0,0,0,0}, pid, i, j, k, clientpid, serverpid, rc=0, rp=0, t=rand()%3+1;
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
			printf("Bem vindo ao modo PvP do jogo do Mastermind!\nO objectivo do jogo e adivinhar a combinacao correta de cores usando o metodo de tentativa/erro.\nNao se esqueca que pode haver cores repetidas!\nCada jogador tem 10 tentativas. Quando elas acabarem perde o jogo.\nBoa sorte!\n\n");
			fflush(stdout);
			for(int i=0;i<20;i++)//game cycle for client
			{
				printf(DEFAULT"Turno do Jogador %d.\n",((t%2)+1));
				fflush(stdout);
				for(j=0;j<4;j++)//code choosing
				{	//escolhe codigo
					do
					{
						printf(DEFAULT"Insira cor %d\n",(j+1));
						fflush(stdout);
						printf(VERMELHO"0 - Vermelho\n");
						fflush(stdout);
						printf(VERDE"1 - Verde\n");
						fflush(stdout);
						printf(AMARELO"2 - Amarelo\n");
						fflush(stdout);
						printf(AZUL"3 - Azul\n");
						fflush(stdout);
						printf(MAGENTA"4 - Magenta\n");
						fflush(stdout);
						printf(CIANO"5 - Ciano\n");
						fflush(stdout);
						printf(DEFAULT"");
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
					printf(VERDE"Parabens Jogador %d! Ganhou o jogo e e o Mastermind!\n",((t%2)+1));
					fflush(stdout);
					kill(serverpid,SIGTERM);
					printf(DEFAULT"");
					fflush(stdout);
					return ;
				}
				else
				{
					printf(DEFAULT"Tem %d cores em %d posicoes corretas(Tentativa - %d).\n", colorc, posc, (i+1));
					fflush(stdout);
				}
				t++;
			}
			printf(VERMELHO"Esgotaram-se as tentativas...\n\nGAME OVER\n\n");
			sleep(1);
			printf(DEFAULT"");
			fflush(stdout);
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
			kill(clientpid,SIGCONT);
			pause();//passa controlo para o cliente (kill e depois pause)
			//server checks if code is right
			for(int b=0;b<20;b++)
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
								colors[codeclient[k]]=1;
							}
							if(l==k)
							{
								if(checkp[k]==0)
								{
									rp++;
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

	return;
}