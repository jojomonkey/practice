// 機械学習.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// convert MNIST file to Image structure

#define PIXEL_SIZE 28*28
#define IMAGE_NUMBERS 100
#define KBR_FALSE 0
#define KBR_TRUE 1
#define NUM_HIDDEN 20
#define LOOP_TIMES 1000
#define OUTPUT_NUMBERS 10

typedef struct MNIST_IMAGE_t {
	double Data[PIXEL_SIZE];
	int Label[OUTPUT_NUMBERS];
}MNIST_IMAGE;


bool SetImageData(MNIST_IMAGE *MnistImage) {
	char fileName1[] = "C:\\Users\\jojomonkey\\Downloads\\train-images-idx3-ubyte\\train-images.idx3-ubyte";
	errno_t error;
	FILE *fp1;
	// open MNIST image file
	error = fopen_s(&fp1, fileName1, "rb");
	if (error != NULL) {
		fprintf(stderr, "ERROR: Cannot open %s.\n", fileName1);
		return KBR_FALSE;
	}

	{
		// read magic number 0x0000, 2byte
		char magic[2];
		int sz = fread(magic, 1, 2, fp1);
		if (sz != 2) {
			printf("Wrong Magic Number.\n");
			return  KBR_FALSE;
		}
		if (magic[0] != 0 || magic[1] != 0) {
			printf("Wrong Magic Number.\n");
			return  KBR_FALSE;
		}
		printf("Magic Number is 0x0000.\n");
	}

	{
		// read magic datatype, 1byte
		char datatype;
		int sz = fread(&datatype, 1, 1, fp1);
		if (sz != 1) {
			printf("Wrong Datatype.\n");
			return  KBR_FALSE;
		}
		if (datatype != 0x08) {
			printf("Wrong Datatype.\n");
			return  KBR_FALSE;
		}
		printf("Datatype is %d.\n", (int)datatype);
	}

	{
		// read dimensions
		char dim;
		int sz = fread(&dim, 1, 1, fp1);
		if (sz != 1) {
			printf("Wrong Dimensions.\n");
			return  KBR_FALSE;
		}
		if (dim != 3) {
			printf("Wrong Dimensions.\n");
			return  KBR_FALSE;
		}
		printf("Dimension is %d.\n", (int)dim);
	}

	{
		// each dimension's size
		int DimNumbers[3];
		char OneDimSize[4];
		int i, j;
		int sz;
		int totallength;
		unsigned char *data;

		for (i = 0; i < 3; i++) {
			sz = fread(OneDimSize, 1, 4, fp1);
			if (sz != 4) {
				printf("Wrong Dimension's Bits.\n");
				return KBR_FALSE;
			}
			DimNumbers[i] = 0;
			for (j = 0; j < 4; j++) {
				DimNumbers[i] = DimNumbers[i] + ((OneDimSize[j] & 0xFF) << 8 * (3 - j));
			}
			printf("Dimension %d is %d.\n", i, DimNumbers[i]);
		}

		totallength = DimNumbers[0] * DimNumbers[1] * DimNumbers[2];
		data = (unsigned char *)malloc(totallength);

		if (data == NULL) {
			printf("Malloc miss.\n");
		}
		sz = fread(data, 1, totallength, fp1);
		if (sz != totallength) {
			printf("Wrong DataSize.\n");
		}

		//IMAGE_NUMBERS個分のImage構造体に輝度値情報をセット
		for (i = 0; i < IMAGE_NUMBERS; i++) {
			for (j = 0; j < PIXEL_SIZE; j++) {
				MnistImage[i].Data[j] = ((double)data[j + i * PIXEL_SIZE])/255;
			}
		}
		free(data);
	}
	fclose(fp1);

	return KBR_TRUE;
}

bool SetLabelData(MNIST_IMAGE *MnistImage) {
	char fileName2[] = "C:\\Users\\jojomonkey\\Downloads\\train-labels-idx1-ubyte\\train-labels.idx1-ubyte";
	errno_t error;
	FILE *fp2;

	// open MNIST label file
	error = fopen_s(&fp2, fileName2, "rb");
	if (error != NULL) {
		fprintf(stderr, "ERROR: Cannot open %s.\n", fileName2);
		return KBR_FALSE;
	}

	{
		// read magic number 0x0000, 2byte
		char magic[2];
		int sz = fread(magic, 1, 2, fp2);
		if (sz != 2) {
			printf("Wrong Magic Number.\n");
			return  KBR_FALSE;
		}
		if (magic[0] != 0 || magic[1] != 0) {
			printf("Wrong Magic Number.\n");
			return  KBR_FALSE;
		}
		printf("Magic Number is 0x0000.\n");
	}

	{
		// read magic datatype, 1byte
		char datatype;
		int sz = fread(&datatype, 1, 1, fp2);
		if (sz != 1) {
			printf("Wrong Datatype.\n");
			return  KBR_FALSE;
		}
		if (datatype != 0x08) {
			printf("Wrong Datatype.\n");
			return  KBR_FALSE;
		}
		printf("Datatype is %d.\n", (int)datatype);
	}

	{
		// read dimensions
		char dim;
		int sz = fread(&dim, 1, 1, fp2);
		if (sz != 1) {
			printf("Wrong Dimensions.\n");
			return  KBR_FALSE;
		}
		if (dim != 1) {
			printf("Wrong Dimensions.\n");
			return  KBR_FALSE;
		}
		printf("Dimension is %d.\n", (int)dim);
	}

	{
		// each dimension's size
		int DimNumbers[1];
		char OneDimSize[4];
		int i, j;
		int sz;
		int totallength;
		unsigned char *data;

		for (i = 0; i < 1; i++) {
			sz = fread(OneDimSize, 1, 4, fp2);
			if (sz != 4) {
				printf("Wrong Dimension's Bits.\n");
				return KBR_FALSE;
			}
			DimNumbers[i] = 0;
			for (j = 0; j < 4; j++) {
				DimNumbers[i] = DimNumbers[i] + ((OneDimSize[j] & 0xFF) << 8 * (3 - j));
			}
			printf("Dimension %d is %d.\n", i, DimNumbers[i]);
		}

		totallength = DimNumbers[0];
		data = (unsigned char *)malloc(totallength);

		if (data == NULL) {
			printf("Malloc miss.\n");
		}
		sz = fread(data, 1, totallength, fp2);
		if (sz != totallength) {
			printf("Wrong DataSize.\n");
		}

		for (i = 0; i < IMAGE_NUMBERS; i++) {
			for (j = 0; j < OUTPUT_NUMBERS; j++) {
				MnistImage[i].Label[j] = ((int)data[i] == j ? 1 : 0);
			}
		}
		free(data);
	}
	fclose(fp2);
	return KBR_TRUE;
}

double sigmoid(double x) {
	return 1 / (1 + exp(-x));
}

double d_sigmoid(double x) {
	double a = 0.1;
	return a * x*(1 - x);
}

bool Perceptron(MNIST_IMAGE *MnistImage) {
	static double w[NUM_HIDDEN + 1][PIXEL_SIZE + 1];
	static double v[NUM_HIDDEN + 1];
	static double y[IMAGE_NUMBERS + 1][NUM_HIDDEN + 1];
	static double z[IMAGE_NUMBERS + 1];

	double eta = 20;
	int UpdateTimes = LOOP_TIMES;
	int i, j, k, l, m;
	double tmp = 0;

	//入力層から中間層への重みをランダムで決める。
	for (l = 0; l < NUM_HIDDEN + 1; l++) {
		for (i = 0; i < PIXEL_SIZE + 1; i++) {
			w[l][i] = ((double)rand() / ((double)RAND_MAX + 1));
		}
	}
	//中間層から出力層への重みをランダムで決める
	for (i = 0; i < NUM_HIDDEN + 1; i++) {
		v[i] = ((double)rand() / ((double)RAND_MAX + 1));
	}

	for (k = 0; k < UpdateTimes; k++) {
		//今の重みを用いて入力層から中間層まで通る
		for (j = 0; j < 10; j++) {
			//hidden
			for (l = 0; l < NUM_HIDDEN; l++) {
				for (i = 0; i < PIXEL_SIZE + 1; i++) {
					tmp += MnistImage[0].Data[i] * w[l][i];
				}
				y[j][l] = sigmoid(tmp);
				tmp = 0;
			}
			y[j][NUM_HIDDEN] = -1;
			//中間層から出力層まで通る
			for (i = 0; i < NUM_HIDDEN + 1; i++) {
				tmp += y[j][i] * v[i];
			}
			z[j] = sigmoid(tmp);
			tmp = 0;

			//教師信号の誤差をvに伝播
			for (i = 0; i < NUM_HIDDEN + 1; i++) {
				v[i] = v[i] - eta * y[j][i] * d_sigmoid(z[j]) * (z[j] - MnistImage[0].Label[j]);
			}

			//wに伝播
			for (l = 0; l < PIXEL_SIZE + 1; l++) {
				for (i = 0; i < NUM_HIDDEN + 1; i++) {
					w[i][l] = w[i][l] - eta * MnistImage[j].Data[l] * d_sigmoid(y[j][i]) * d_sigmoid(z[j]) * (z[j] - MnistImage[0].Label[j]) * v[i];
				}
			}
		}
	}

	for (j = 0; j < 10; j++) {
		for (l = 0; l < NUM_HIDDEN; l++) {
			for (i = 0; i < PIXEL_SIZE + 1; i++) {
				tmp += MnistImage[0].Data[i] * w[l][i];
			}
			y[j][l] = sigmoid(tmp);
			tmp = 0;
		}
		y[j][NUM_HIDDEN] = -1;
		for (i = 0; i < NUM_HIDDEN + 1; i++) {
			tmp += y[j][i] * v[i];
		}
		z[j] = sigmoid(tmp);
		tmp = 0;
	}

	printf("z=");
	for (i = 0; i < 10; i++) {
		printf("%f ", z[i]);
	}
	printf("LoopNumber:%d\n", k);
	return KBR_TRUE;
}

int main()
{
	static MNIST_IMAGE MnistImage[IMAGE_NUMBERS];
	int bRet;
	bRet = SetImageData(MnistImage);
	if (bRet == KBR_FALSE) {
		printf("Wrong Image Data.\n");
		return 0;
	}
	bRet = SetLabelData(MnistImage);
	if (bRet == KBR_FALSE) {
		printf("Wrong Label Data.\n");
		return 0;
	}

	bRet = Perceptron(MnistImage);
	return 0;
}