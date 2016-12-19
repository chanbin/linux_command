#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <crypt.h>

#include <stdlib.h>
#include <time.h>

#define MAXLINE 512
#define CYLINE 98

char pwd[MAXLINE], rpwd[MAXLINE];
char *cyper;

//gcc argument -lcrypt

int verify_passwd(char *user);
int write_shadow(FILE *fp, char *user, long size);
void passwd_crypt(char *rpwd);

int main(int argc, char *argv[]){
	FILE *fp1, *fp2;		//fp1 = read shadow, fp2 = append shadow
	char buffer[MAXLINE], buffer2[MAXLINE];
	char *token = NULL;
	char s2[] = ":\n";
	int ret1,ret2;
	long size;

	if(argc!=2){
		fprintf(stderr, "usage: %s username\n", argv[0]);
		return 1;
	}


	fp1 = fopen("/home/chanbin/Desktop/projects/shadow1", "r");
	if(fp1 == NULL)
		printf("fp1 file open error\n");
	
	while(fgets(buffer, MAXLINE, fp1) != NULL){			//search per line
		strcpy(buffer2, buffer);
		token = strtok(buffer2, s2);				//line string tokenize
		while(token != NULL){	
			if(strstr(token, argv[1])!=NULL && strlen(token)==strlen(argv[1])){
				size = ftell(fp1)-(sizeof(argv[1])+CYLINE+17);
				ret1 = verify_passwd(argv[1]);
				if(ret1 == 1){
					printf("Sorry, passwords do not match\n");
					return 1;
				}
				else if(ret1 == 0){
					fclose(fp1);
					fp2 = fopen("/home/chanbin/Desktop/projects/shadow1", "r+");
					if(fp2 == NULL)
						printf("fp2 file open error\n");
					passwd_crypt(rpwd);//encrypt pwd
					ret2 = write_shadow(fp2,argv[1],size);//writing shadow
					if(ret2 == 1){
						fclose(fp2);
						fprintf(stderr, "password have unchanged\n");
						return 1;
					}
					else if(ret2 == 0){
						fclose(fp2);
						fprintf(stderr, "password have changed\n");
						return 0;
					}
				}
			}
			token = strtok(NULL,s2);
		}
	}if(feof(fp1) != 0)
		fprintf(stderr, "user '%s' does not exist\n", argv[1]);

	fclose(fp1);
	return 1;
}
	

int verify_passwd(char *user){
	printf("Changing password for user %s\n", user);
	printf("New password: ");
	scanf("%s",pwd);
	printf("Retype new password: ");
	scanf("%s",rpwd);

	if(strcmp(pwd,rpwd) == 0)
		return 0;
	else
		return 1;
}

int write_shadow(FILE *fp, char *user, long size){ //size=present file pointer
	fseek(fp, size+(sizeof(user)-3), SEEK_SET);
	if(fwrite(cyper,1,(sizeof(cyper)*12+2),fp) != -1){//98, sizeof(cyper)=8bytes=pointer		
		printf("password updated successfully\n");
		return 0;
	}
	else
		return 1;
}

void passwd_crypt(char *rpwd){
	char salt[11];
	char r_salt;
	int i;

	salt[0]='$';salt[1]='6';salt[2]='$';
	srand(time(NULL));
	for(i=3;i<11;i++){
		r_salt = rand();
		do{
			if((r_salt>='\x2F'&&r_salt<='\x39')||(r_salt>='\x41'&&r_salt<='\x5A')||(r_salt>='\x61'&&r_salt<='\x7A')){
				salt[i]=r_salt;
				break;
			}
		}while(r_salt = rand());
	}
	cyper = crypt(rpwd,salt);
	printf("cyper = %s\n",cyper);
}
