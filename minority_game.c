#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 101
#define try 1000
#define d_min 4

//���������֐�
//�Q��URL http://www.sat.t.u-tokyo.ac.jp/~omi/random_variables_generation.html
//rand()��0�`1�ȊO�̐������������邽�߁A���K��
double create_rand() {
	return ((double)rand() + 1.0) / ((double)RAND_MAX + 2.0);
}

//2��ڈȍ~�̈�`�qp����
//�����Ɍ��݂̈�`�qp
double create_p(double p) {
	double new_P;
	int sign;
	sign = rand();

	//���݂̈�`�q�ɑ����̂������̂��������_���Ō���
	sign = sign % 2;
	if (sign == 0) 	sign = -1;
	else sign = 1;

	//���݂̈�`�q����V�������̂𐶐�
	new_P = p + create_rand()* 0.1 * sign;

	//0�`1�͈̔͂Ɏ��܂��Ă��邩�m�F
	if (new_P > 1) return 1;
	else if (new_P < 0) return 0;
	else return new_P;
}

//�헪�̓��e
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
	//agent[i][0] <- ��`�qp
	//agent[i][1] <- ����d
	//agent[i][2] <- 0 or 1
	double p_th;
	int i, j, count;
	int history = 0;

	srand((unsigned)time(NULL));

	//������������
	for (i = 0; i < N; i++) {
		agent[i][0] = create_rand();
		agent[i][1] = 0;
	}

	for (j = 0; j < try; j++) {
		p_th = create_rand();
		count = 0;

		//0 or 1�̌���i1��ځ`3��ڂ܂Łj
		//�O�񏑂����v���O���������̂܂܍̗p
		if (j < 3) {
			for (i = 0; i < N; i++) {
				if (agent[i][0] > p_th) {	//1�̐l����count�ŊǗ�
					agent[i][2] = 1;
					count++;
				}
				else agent[i][2] = 0;
			}
		}

		//0 or 1�̌���i4��ڈȍ~�A�����̊��p�j
		//p_th���傫�����strategy�i�헪�j�Ƃ͈Ⴄ���̂�I��
		if (j >= 3){
			for (i = 0; i < N; i++) {
				if (agent[i][0] > p_th) agent[i][2] = Not(strategy(history));
				else agent[i][2] = strategy(history);

				if (agent[i][2] == 1) count++;
			}
		}

		for (i = 0; i < N; i++) {
			//�����h�Ə����h�ɂ��ꂼ�꓾�_��^����
			if (count > N / 2) {	//1�������h
				if (agent[i][2] == 1) agent[i][1]--;
				else agent[i][1]++;
			}
			else {	//0�������h
				if (agent[i][2] == 0) agent[i][1]--;
				else agent[i][1]++;
			}

			//�V������`�q�̌���id=-4�̂Ɉ�`�q��ύX����̂��A-4�ȉ��ŕύX����̂����m�F�j
			if (agent[i][1] == d_min) agent[i][0] = 1 - agent[i][0];
			else agent[i][0] = create_p(agent[i][0]);
		}
		//�����̍X�V//
		/*
		��2����100�̗]��𗘗p���āA���o��
		�����10�{����
		����̏����h�̌��ʂ𑫂�
		���̌��ʁA
			1�̈ʁ��ŐV�̌���
			10�̈ʁ�2�Ԗڂ̌���
			100�̈ʁ�3�Ԗڂ̌���
		�ɂȂ�
		*/
		if (count > N / 2) history = (history % 100) * 10;
		else history = (history % 100) * 10 + 1;
	}

	//�����̕\��
	for (i = 0; i < N; i++)printf("agent[%d]�̗�����%3.0lf�ł�\n", i, agent[i][1]);

	return 0;
}

