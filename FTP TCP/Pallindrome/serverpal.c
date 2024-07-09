#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
void reverseString(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

void removeNewline(char *str) {
    int len = strlen(str);
    if (str[len - 1] == '\n') {
        str[len - 1] = '\0'; // Replace newline with null terminator
    }
}
void main(){
	FILE *fp;
	int n;
	int sock_desc,new_sock;
	char rcv[100],fileread[200];
	struct sockaddr_in server_addr;
	struct sockaddr_storage client_addr;
	socklen_t client_addr_len;
	
	sock_desc=socket(AF_INET,SOCK_STREAM,0);
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(6466);
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	bind(sock_desc,(struct sockaddr*)&server_addr,sizeof(server_addr));
	
	if(listen(sock_desc,5)==0){
		printf("Listening..\n");
	}
	
	client_addr_len=sizeof(client_addr);
	new_sock=accept(sock_desc,(struct sockaddr*)&client_addr,&client_addr_len);

	n=recv(new_sock,rcv,100,0);
	rcv[n]='\0';
	fp=fopen(rcv,"r");
	if(fp==NULL){
		printf("No file exists\n");
		send(new_sock,"error",5,0);
		close(new_sock);
	}
	else{
		while(fgets(fileread,sizeof(fileread),fp)){
		removeNewline(fileread); // Strip newline character
			char temp[200];
			strcpy(temp,fileread);
			reverseString(fileread);
			if(strcmp(temp,fileread)==0){
				printf("Pallidnrome\n");
			}
			else{
				printf("Not pallindrome\n");
			}
		}
		fclose(fp);
		send(new_sock,"completed",9,0);		
	}
	close(sock_desc);
	close(new_sock);
}
