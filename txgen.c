#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	//引数は3つ。3つじゃないとエラーを吐いて終了する	
    int num_txn, num_account, max_amount, i;
    int select=0;
    if (argc!=4) {
            printf("usage: txgen num_txn num_account max_amount\n");
            exit(1);
    }
    /*
    1つめの引数：何個トランザクションを作るか
    2つめの引数：銀行口座の番号の上限
    3つめの引数：最大何円振り替えるか
    */
    num_txn = atoi(argv[1]);
    num_account = atoi(argv[2]);
    max_amount = atoi(argv[3]);

    srand(time(NULL));

//    printf("num_txn=%d, num_account=%d, max_amount=%d\n", num_txn, num_account, max_amount);
    for (i = 0; i<num_txn; i++) {
            int toVal, amount;
            int fromVal = rand() % num_account;
            while (1) {
                    toVal = rand() % num_account;
                    if (fromVal != toVal) break;
            }
            amount = (rand() % max_amount) + 1;
            select = rand() % 2;
            if(select==0){
        		printf("t,%d,%d,%d\n", fromVal, toVal, amount);
           	 }
           	else if(select==1){
        		printf("o,%d,%d,%d\n", fromVal, toVal, amount);
           	}

    }
    return 0;
}
