#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 101
#define try 1000
#define d_min 4

//乱数生成関数
//参照URL http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html
//rand()は0～1以外の数字も生成するため、正規化
double create_rand() {
	return ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
}

//2回目以降の遺伝子p生成
//引数に現在の遺伝子p
double create_p(double p) {
	double new_P;
	int sign;
	sign = rand();

	//現在の遺伝子に足すのか引くのかをランダムで決定
	sign = sign % 2;
	if (sign == 0) 	sign = -1;
	else sign = 1;

	//現在の遺伝子から新しいものを生成
	new_P = p + create_rand()* 0.1 * sign;

	//0～1の範囲に収まっているか確認
	if (new_P > 1) return 1;
	else if (new_P < 0) return 0;
	else return new_P;
}

//戦略の内容
int strategy(int personal_history) {

	switch (personal_history)
	{
	case 000:
	case 011:
	case 100:
	case 110:
		return 0;
	case 001:
	case 010:
	case 101:
	case 111:
		return 1;
	default:
		return -1;
	}
}

int Not(int value) {
	if (value == 0) return 1;
	else return 0;
}


int main(void) {
	double agent[N][3];
	//agent[i][0] <- 遺伝子p
	//agent[i][1] <- 利得d
	//agent[i][2] <- 0 or 1
	double p_th;
	int i, j, count;
	int history = 0;

	srand((unsigned)time(NULL));

	//初期乱数生成
	for (i = 0; i < N; i++) {
		agent[i][0] = create_rand();
		agent[i][1] = 0;
	}

	for (j = 0; j < try; j++) {
		p_th = create_rand();
		count = 0;

		//0 or 1の決定（1回目～3回目まで）
		//前回書いたプログラムをそのまま採用
		if (j < 3) {
			for (i = 0; i < N; i++) {
				if (agent[i][0] > p_th) {	//1の人数をcountで管理
					agent[i][2] = 1;
					count++;
				}
				else agent[i][2] = 0;
			}
		}

		//0 or 1の決定（4回目以降、履歴の活用）
		//p_thより大きければstrategy（戦略）とは違うものを選択
		if (j >= 3){
			for (i = 0; i < N; i++) {
				if (agent[i][0] > p_th) agent[i][2] = Not(strategy(history));
				else agent[i][2] = strategy(history);

				if (agent[i][2] == 1) count++;
			}
		}

		for (i = 0; i < N; i++) {
			//多数派と少数派にそれぞれ得点を与える
			if (count > N / 2) {	//1が多数派
				if (agent[i][2] == 1) agent[i][1]--;
				else agent[i][1]++;
			}
			else {	//0が多数派
				if (agent[i][2] == 0) agent[i][1]--;
				else agent[i][1]++;
			}

			//新しい遺伝子の決定（d=-4のに遺伝子を変更するのか、-4以下で変更するのかを確認）
			if (agent[i][1] == d_min) agent[i][0] = 1 - agent[i][0];
			else agent[i][0] = create_p(agent[i][0]);
		}
		//履歴の更新//
		/*
		下2桁を100の余りを利用して、取り出し
		それを10倍して
		今回の少数派の結果を足す
		その結果、
			1の位→最新の結果
			10の位→2番目の結果
			100の位→3番目の結果
		になる
		*/
		if (count > N / 2) history = (history % 100) * 10;
		else history = (history % 100) * 10 + 1;
	}

	//答えの表示
	for (i = 0; i < N; i++)printf("agent[%d]の利得は%3.0lfです\n", i, agent[i][1]);

	return 0;
}

