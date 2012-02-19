/*
 * nnanimation.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNANIMATION_H_
#define NNANIMATION_H_

#include "nnimage.h"

typedef struct NNAnimationImage {
	NNImage *image;
	float duration;
} NNAnimationImage;

typedef struct NNAnimation {
	const char *id;
	const char *label;
	NNAnimationImage **images;
	int interval;
	int repeat;
} NNAnimation;

NNAnimation * nnanimation_new(const char *id, const char *label, int interval, int repeat);
void nnanimation_free(NNAnimation *animation);

#endif /* NNANIMATION_H_ */
