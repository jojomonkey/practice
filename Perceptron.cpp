#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MNISTtype.h"

double sigmoid(double x) {
	double a = 10;
	x = x / a;
	return 1 / (1 + exp(-x));
}

double d_sigmoid(double x) {
	double a = 0.1;
	return a * x * (1 - x);
}
void InitWeight(double w[][PIXEL_SIZE], double v[][NUM_HIDDEN]) {
	int i, j, k;

	//“ü—Í‘w‚©‚ç’†ŠÔ‘w‚Ö‚Ìd‚İ‚ğƒ‰ƒ“ƒ_ƒ€‚ÅŒˆ‚ß‚éB
	for (j = 0; j < NUM_HIDDEN; j++) {
		for (i = 0; i < PIXEL_SIZE; i++) {
			w[j][i] = ((double)rand() / ((double)RAND_MAX + 1));
		}
	}
	//’†ŠÔ‘w‚©‚ço—Í‘w‚Ö‚Ìd‚İ‚ğƒ‰ƒ“ƒ_ƒ€‚ÅŒˆ‚ß‚é
	for (k = 0; k < OUTPUT_NUMBERS; k++) {
		for (j = 0; j < NUM_HIDDEN; j++) {
			v[k][j] = ((double)rand() / ((double)RAND_MAX + 1));
		}
	}
}

void Forward_propagation(MNIST_IMAGE *MnistImage, double w[][PIXEL_SIZE], double v[][NUM_HIDDEN], double y[][NUM_HIDDEN], double z[][OUTPUT_NUMBERS], int p) {
	int i, j, k;
	double SumOutput;
	//hidden
	for (j = 0; j < NUM_HIDDEN; j++) {
		for (i = 0; i < PIXEL_SIZE; i++) {
			SumOutput += MnistImage[p].Data[i] * w[j][i];
		}
		y[p][j] = sigmoid(SumOutput);
		SumOutput = 0;
	}

	//’†ŠÔ‘w‚©‚ço—Í‘w‚Ü‚Å’Ê‚é
	for (k = 0; k < OUTPUT_NUMBERS; k++) {
		for (j = 0; j < NUM_HIDDEN; j++) {
			SumOutput += y[p][j] * v[k][j];
		}
		z[p][k] = sigmoid(SumOutput);
		SumOutput = 0;
	}

}


void Err_propagation(MNIST_IMAGE *MnistImage, double w[][PIXEL_SIZE], double v[][NUM_HIDDEN], double y[][NUM_HIDDEN], double z[][OUTPUT_NUMBERS], int p) {
	int i, j, k;
	double eta = 20;
	static double SumV[NUM_HIDDEN];
	//‹³tM†‚ÌŒë·‚ğv‚É“`”d
	for (k = 0; k < OUTPUT_NUMBERS; k++) {
		for (j = 0; j < NUM_HIDDEN; j++) {
			v[k][j] = v[k][j] - eta * y[p][j] * d_sigmoid(z[p][k]) * (z[p][k] - MnistImage[p].Label[k]);
		}
	}

	for (j = 0; j < NUM_HIDDEN; j++) {
		SumV[j] = 0;
		for (k = 0; k < OUTPUT_NUMBERS; k++) {
			SumV[j] += d_sigmoid(z[p][k]) * (z[p][k] - MnistImage[p].Label[k]) * v[k][j];
		}
	}

	for (j = 0; j < NUM_HIDDEN; j++) {
		for (i = 0; i < PIXEL_SIZE; i++) {
			w[j][i] = w[j][i] - eta * MnistImage[p].Data[i] * d_sigmoid(y[p][j]) * SumV[j];
		}
	}

}

bool Perceptron(MNIST_IMAGE *MnistImage, double w[][PIXEL_SIZE], double v[][NUM_HIDDEN]) {
	static double y[IMAGE_NUMBERS][NUM_HIDDEN];
	static double z[IMAGE_NUMBERS][OUTPUT_NUMBERS];
	int UpdateTimes = LOOP_TIMES;
	int l, p;
	double SumOutput = 0;
	int cnt = 0;

	InitWeight(w, v);

	for (l = 0; l < UpdateTimes; l++) {
		for (p = 0; p < IMAGE_NUMBERS; p++) {
			 Forward_propagation(MnistImage, w, v, y, z, p);
			 Err_propagation(MnistImage, w, v, y, z, p);
		}
	}
	return TRUE;
}

void TestWeight(MNIST_IMAGE *TestImage, double w[][PIXEL_SIZE], double v[][NUM_HIDDEN]) {

	static double y[TEST_NUMBERS][NUM_HIDDEN];
	static double z[TEST_NUMBERS][OUTPUT_NUMBERS];
	int i, j, k, p;
	double tmp = 0;
	int cnt = 0;

	for (p = 0; p < TEST_NUMBERS; p++) {
		for (j = 0; j < NUM_HIDDEN; j++) {
			for (i = 0; i < PIXEL_SIZE; i++) {
				tmp += TestImage[p].Data[i] * w[j][i];
			}
			y[p][j] = sigmoid(tmp);
			tmp = 0;
		}
		for (k = 0; k < OUTPUT_NUMBERS; k++) {
			for (j = 0; j < NUM_HIDDEN; j++) {
				tmp += y[p][j] * v[k][j];
			}
			z[p][k] = sigmoid(tmp);
			tmp = 0;
		}
	}

	for (p = 0; p < TEST_NUMBERS; p++) {
		printf("z=");
		for (k = 0; k < OUTPUT_NUMBERS; k++) {
			if (z[p][k] >= 0.9) {
				TestImage[p].solveLabel = k;
				TestImage[p].bRet = (TestImage[p].idealLabel == k ? 1 : 0);
				if (TestImage[p].bRet == TRUE) {
					cnt++;
				}
			}
			printf("%f ", z[p][k]);
		}
		printf("%d ", TestImage[p].idealLabel);
		printf("%d ", TestImage[p].solveLabel);
		printf("%d ", TestImage[p].bRet);
		printf("\n");
	}

	printf("cnt = %d", cnt);
	printf("LoopNumber:%d\n", k);
}