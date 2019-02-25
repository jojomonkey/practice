#pragma once
#include "stdafx.h"
#include "MNISTtype.h"

bool Perceptron(MNIST_IMAGE *MnistImage, double w[][PIXEL_SIZE], double v[][NUM_HIDDEN]);
void TestWeight(MNIST_IMAGE *TestImage, double w[][PIXEL_SIZE], double v[][NUM_HIDDEN]);
