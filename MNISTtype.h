#pragma once

#include "stdafx.h"


#define PIXEL_SIZE 28*28
#define IMAGE_NUMBERS 60000
#define TEST_NUMBERS 10000
#define FALSE 0
#define TRUE 1
#define NUM_HIDDEN 100
#define LOOP_TIMES 1000
#define OUTPUT_NUMBERS 10

typedef struct MNIST_IMAGE_t {
	double Data[PIXEL_SIZE];
	int Label[OUTPUT_NUMBERS];
	int idealLabel;
	int solveLabel;
	bool bRet;
}MNIST_IMAGE;
