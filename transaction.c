#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define N 10000

int account[100];
int from[N], to[N], amount[N];
int ready = 0;

//時間を保存する変数を宣言
struct timespec start;
struct timespec finish;

pthread_t thread[10];
pthread_mutex_t mutex;
pthread_cond_t cvar;

void *func(void *arg);
void sumAmount(void);

int main(void){
	int i, number;
	char text;
	long startt,finisht;
	
	//始める時刻を記録
	clock_gettime(CLOCK_REALTIME, &start);	//経過時間を計算するために現在時間を保存
	startt = start.tv_sec*1000000000+start.tv_nsec;	//ナノ秒を抽出
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
	for(i=0; i<10000; i++){
		if((fscanf(istream, "%c,%d,%d,%d\n", &text, &from[i], &to[i], &amount[i]))==EOF){
			break;
		}
	}
	fclose(istream);
	
	//条件変数の初期化
	pthread_cond_init(&cvar, NULL);
	//mutexの初期化
	pthread_mutex_init(&mutex, NULL);
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
	//終わった時間を記録して経過時間を表示
	clock_gettime(CLOCK_REALTIME, &finish);
	finisht = finish.tv_sec*1000000000 + finish.tv_nsec;
	printf("経過時間:%ld\n", finisht-startt);
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
			//なにもしないforループをはさむ
			for(j=0;j<100*amount_num;j++){
				
			}
			//リソースを獲得
			pthread_mutex_lock(&mutex);
			//振り替えると銀行口座が0円となってしまうときはcond_waitする
			while(account[from_num]- amount_num == 0){
				pthread_cond_wait(&cvar, &mutex);
			}
			while(account[from_num]-amount_num == 0){
				pthread_cond_wait(&cvar, &mutex);
			}
			//トランザクションを実行
			account[from_num] -= amount_num;
			account[to_num] += amount_num;
			//振り替えることができたらcvarにcond_signalを送って待っているスレッドを起こす
			pthread_cond_signal(&cvar);
			pthread_mutex_unlock(&mutex);
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
