#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

void main(){
	FILE *fp;
	int n;
	int client_desc;
	char rcvg[100];
	char name[110],fname[100];
	struct sockaddr_in server_addr;
	socklen_t server_addr_len;
	
	client_desc=socket(AF_INET,SOCK_STREAM,0);
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(6466);
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	connect(client_desc,(struct sockaddr*)&server_addr,sizeof(server_addr));
	
	printf("Enter file name : ");
	scanf("%49s",name);
	printf("Enter new file name: ");
	scanf("%49s",fname);
	
	fp=fopen(fname,"w");
	if(fp==NULL){
		printf("Cannot open file\n");
	}
	
	send(client_desc,name,strlen(name),0);
	
	while(1){
		int s=recv(client_desc,rcvg,100,0);
		if(s<=0){
			printf("Error\n");
			break;
		}	
		rcvg[s]='\0';
		
		if(strcmp(rcvg,"error")==0){
			printf("File not exisitng\n");
		}
		if(strcmp(rcvg,"completed")==0){
			printf("Transfer done\n");
			fclose(fp);
			break;
		}else{
			fputs(rcvg,stdout);
			printf("Pallidnrome IS ::: %s",rcvg);
			fprintf(fp,"%s",rcvg);
		}
		
	}	
	close(client_desc);
	
	


}
