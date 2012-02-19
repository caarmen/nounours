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
	int num_images;
	NNAnimationImage **images;
	int interval;
	int repeat;
} NNAnimation;

NNAnimation * nnanimation_new(const char *id, const char *label, int interval, int repeat);
NNAnimationImage * nnanimation_image_new(NNImage *image, float duration);
void nnanimation_add_image(NNAnimation *animation, NNImage *image, float duration);
void nnanimation_free(NNAnimation *animation);

#endif /* NNANIMATION_H_ */
