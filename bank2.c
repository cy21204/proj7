#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#define N 10000

//cli.c側

int account[100];
char text[N]
int from[N], to[N], amount[N];
pthread_t thread[10];

void *func(void *arg);
void sumAmount(void);

int main(void){
	int i, number;

	
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
	for(i=0; i<N; i++){
		if((fscanf(istream, "%c,%d,%d,%d\n", &text[i], &from[i], &to[i], &amount[i]))==EOF){
			break;
		}
	}
	fclose(istream);
	for(i=0;i<N;i++){
		//振り替えのとき
		if(text[i]=='t'){
			account[from[i]]+=amount[i];
			account[to[i]]-=amount[i];
		}
		else if(text[i]=='o'){
			
		}	
	}
	
	
	//ソケットを作る
	int sockfd;
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
	
	//スレッドを作る
	for(i=0;i<10;i++){
		//担当するトランザクションの範囲を指定して各スレッドを作る
		pthread_create(&thread[i],NULL,func,(void *)i);
	}
	
	//スレッドの終了をまつ
	for(i=0;i<10;i++){
		//担当するトランザクションの範囲を指定して各スレッドを作る
		pthread_join(thread[i],NULL);
	}
	//全銀行口座の総額を確認(10000*100のはず)
	sumAmount();
}

//トランザクションをする関数
void *func(void *arg){
	int i,j;
	int from_num, to_num, amount_num;
	int val = (int)arg;
	int start_num = val*N/10;
	int finish_num = start_num + N/10;
	
	
	for(i=start_num; i<finish_num; i++){
		//トランザクションの情報を変数に保存
		from_num = from[i];
		to_num = to[i];
		amount_num = amount[i];
		for(j=0;j<199*amount_num;j++){
			
		}
		//トランザクションを実行
		account[from_num] -= amount_num;
		account[to_num] += amount_num;
	}	
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
