#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#define N 10000

//serv.c側
//銀行口座を管理する配列
char data[N];
char text[N]
int from[N], to[N], amount[N];
//スレッド・mutexの変数
pthread_t thread_id;
pthread_mutex_t mutex;
//関数
void sumAmount(void);
void recieve(void);

int main(void){
	int i, number;
//銀行口座の初期設定・CSVファイル読み込み
	//すべての口座の初期値を10000円にする
	for(i=0;i<100;i++){
		account[i]=10000;
	}
	//全銀行口座の総額を確認(10000*100のはず)
	sumAmount();
	
	//csvファイルから各配列に保存
	FILE *istream;
	if((istream = fopen("./pj/txn/trans.csv", "r")) == NULL){	//ファイルを開く。開けなかったらエラーを表示
		printf("ERROR:can't read file.\n");
	}
	while(1){
		if((fgets(data[i],sizeof(data[i]),istream))==0){
			break;
		}
	}
	fclose(istream);
	for(i=0;i<N;i++){
		sscanf(data[i],"%c, %d, %d, %d",&text[N], &from[N], &to[N], &amount[N]);
	}
	
	
//スレッド・mutexの準備
	pthread_mutex_init(&mutex);
	pthread_create(&thread_id, NULL, recieve, NULL);
	//受金スレッドを動かす
	pthread_join(thread_id,NULL);
	
//ソケット通信の準備
	int sockfd, new_sockfd;
	//ソケットを作る
	sockfd=socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket: ");
	}
	else{
		printf("Created!\n");
	}
	//アドレスを作る
	if(memset(&serv_addr, 0, sizeof(struct sockaddr_in)) == NULL){
		perror("memset: ");
	}
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(50000);
	//ソケットにアドレスを割り当てる
	if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) == -1){
		perror("bind: ");
	}
	else{
		printf("Socket binded!\n");
	}
	//コネクション要求を待たせる
	if(listen(sockfd, 5) == -1){
		perror("listen: ");
	}
	new_sockfd = accept(sockfd, NULL, NULL);
	if(new_sockfd == -1){
		perror("accept: ");
	}
	
//送金スレッド操作
	for(i=0;i<N;i++){
		//振り替えのとき
		if(text[i]=='t'){
			while(1){
				if(pthread_mutex_trylock(&mutex) == 0){
					break;
				}	
			}
			account[from[i]]+=amount[i];
			account[to[i]]-=amount[i];
			pthread_mutex_unlock(&mutex);
		}
		//振り込みのとき
		else if(text[i]=='o'){
			while(1){
				if(pthread_mutex_trylock(&mutex) == 0){
					break;
				}	
			}
			account[from[i]]-=amount[i];
			//送金操作
			write(sockfd,data[i],sizeof(data[i]));
			pthread_mutex_unlock(&mutex);
		}
	}
	

	//全銀行口座の総額を確認(10000*100のはず)
	sumAmount();
}

//受金スレッド操作
void recieve(void *arg){
	char buff[128],buff_text;
	int buff_from, buff_to, buff_amount;
	//受金操作
	new_sockfd=accept(sockfd,NULL,NULL);
	read(new_sockfd,buff,sizeof(data[0]));
	sscanf(buff,"%c, %d, %d, %d",&buff_text, &buff_from, &buff_to, &buff_amount);
	while(1){
		if(pthread_mutex_trylock(&mutex) == 0){
			break;
		}	
	}
	amount[buff_to]-=buff_amount;
	pthread_mutex_unlock(&mutex);
		
}
//全銀行口座の総額を計算して総額を表示する関数
void sumAmount(void){
	int i,sum=0;
	
	for(i=0; i<100; i++){
		sum+=account[i];
		printf("%d\n", account[i]);
	}
	printf("銀行口座総額: %d\n",sum);
}
