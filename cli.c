#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

struct sockaddr_in serv_addr;

int main(){
	int val;
	char buff[6] = "hello";
	int sockfd;
	//ソケットを作る
	sockfd=socket(AF_INET, SOCK_STREAM,0);
	if(sockfd == -1){
		perror("socket: ");
	}
	else{
		printf("Created!\n");
	}
	//サーバーのアドレスをつくる
	if(memset(&serv_addr, 0, sizeof(struct sockaddr_in)) == NULL){
		perror("memset:");
	}
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(50000);

	//コネクションを張るための要求をサーバーに送る
	if(connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) == -1){
		perror("connect:");
	}
	//ソケットで文字列を送る
	if(write(sockfd,(void *)buff,sizeof(buff)) == -1){
		perror("write:");
	}
	//ソケットを終了する
	if(close(sockfd) == -1){
		perror("close:");
	}
}
