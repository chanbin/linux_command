#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLINE 512
#define MAXCMD 56

int main(int argc, char *arg[]){
	FILE *fp;
	int status, edge;
	char buffer[MAXLINE]="";
	char *string=" ";


//start
	if((status = system("ps -efH > pstree2")) < 0)
		perror("sytem() error\n");
	//printf("exit code %d\n",WEXITSTATUS(status));

	if((fp = fopen("pstree2","r")) == NULL){
		fprintf(stderr, "fileopen error\n");
		return 1;
	}

	while(fgets(buffer, MAXLINE, fp) != NULL){
		for(edge=46; edge<MAXLINE; edge++){
			if(buffer[edge] == '\x20'){
				edge++;//clear
				while(edge != MAXLINE && edge != '\x0A' && edge!= '\x0D'){//'\x0A'=LF, '\x0D'=CR
					if(buffer[edge]=='\x20'&&buffer[edge+1]!='\x20')buffer[edge]='-';//print branch
					if(buffer[edge]=='\x20'&&buffer[edge+2]!='\x20')buffer[edge]='|';//print trunk
					printf("%c",buffer[edge]);//CMD print
					if(edge>70 && (buffer[edge]=='/'||buffer[edge]=='\x20')){
						printf(" (more data...)");printf("\n");
						memset(buffer,'\x00',MAXLINE);
						break;
					}
					edge++;
				}
				memset(buffer,0,MAXLINE);
				break;
			}
		}
	}


//end
	if((status = system("rm pstree2")) < 0)
		perror("sytem() error\n");
}