#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

struct sockaddr_in serv_addr;

int main(){
	int val;
	int sockfd, new_sockfd;
	int flag=1;

  //ソケットを作る
	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket: ");
		exit(EXIT_FAILURE);
	}
  	else{
		printf("Created!\n");
  	}
  //アドレスを作る
  	if(memset(&serv_addr, 0, sizeof(struct sockaddr_in)) == NULL){
 		perror("memset: ");
  	}
  //ポート番号80を割り振る
  	serv_addr.sin_family=AF_INET;
  	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
 	serv_addr.sin_port=htons(80);
  	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(int));
  //ソケットにアドレスを割り当てる
	if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) == -1){
		perror("bind: ");
		exit(EXIT_FAILURE);
  	}
  	else{
    		printf("Socket binded!\n");
  	}
  //コネクションの要求を待つ
	listen(sockfd,5);
	FILE*istream=fdopen(new_sockfd,"r+");
	while(1){
    //要求を受け入れる
		new_sockfd=accept(sockfd,NULL,NULL);
		if(new_sockfd == -1){
			perror("accept: ");
      		exit(EXIT_FAILURE);
    		}
    //ソケットをファイルストリームへと変換する
		if(istream == NULL){
			perror("fdopen:");
 		}
		if(setvbuf(istream,NULL,_IONBF,0) != 0){
			perror("setvbuf:");
		}
    		char buff[1024];
		while(fgets(buff, sizeof(buff), istream) != NULL){
			printf("%s", buff);
  		}
	}	
 	fclose(istream);
}
