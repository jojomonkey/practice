// 機械学習.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MNISTtype.h"
#include "OpneFiles.h"
#include "Perceptron.h"


int main()
{
	static MNIST_IMAGE MnistImage[IMAGE_NUMBERS];
	static MNIST_IMAGE TestImage[IMAGE_NUMBERS];
	static double w[NUM_HIDDEN][PIXEL_SIZE];
	static double v[OUTPUT_NUMBERS][NUM_HIDDEN];
	char fileName1[] = "C:\\Users\\jojomonkey\\Downloads\\train-images-idx3-ubyte\\train-images.idx3-ubyte";
	char fileName2[] = "C:\\Users\\jojomonkey\\Downloads\\train-labels-idx1-ubyte\\train-labels.idx1-ubyte";
	char fileName3[] = "C:\\Users\\jojomonkey\\Downloads\\t10k-images-idx3-ubyte\\t10k-images.idx3-ubyte";
	char fileName4[] = "C:\\Users\\jojomonkey\\Downloads\\t10k-labels-idx1-ubyte\\t10k-labels.idx1-ubyte";

	int bRet;
	bRet = SetImageData(MnistImage, fileName1);
	if (bRet == FALSE) {
		printf("Wrong Image Data.\n");
		return 0;
	}
	bRet = SetLabelData(MnistImage, fileName2);
	if (bRet == FALSE) {
		printf("Wrong Label Data.\n");
		return 0;
	}
	bRet = SetImageData(TestImage, fileName3);
	if (bRet == FALSE) {
		printf("Wrong Image Data.\n");
		return 0;
	}
	bRet = SetLabelData(TestImage, fileName4);
	if (bRet == FALSE) {
		printf("Wrong Image Data.\n");
		return 0;
	}

	bRet = Perceptron(MnistImage, w, v);
	TestWeight(TestImage, w, v);
	return 0;
}