/*
 * nnanimation.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNANIMATION_H_
#define NNANIMATION_H_

#include "nnimage.h"
#include "nnnounours.h"

typedef struct NNAnimationImage {
	NNImage *image;
	float duration;
} NNAnimationImage;

typedef struct NNAnimation {
	NNNounours *nounours;
	char *id;
	char *label;
	int num_images;
	NNAnimationImage **images;
	int interval;
	int repeat;
} NNAnimation;

typedef struct NNAnimationFling {
	int x;
	int y;
	int width;
	int height;
	float min_vel_x;
	float min_vel_y;
	NNAnimation *animation;
} NNAnimationFling;

NNAnimation * nnanimation_new(NNNounours *nounours, char *id, char *label, int interval, int repeat);
void nnanimation_show(NNAnimation *animation);
NNAnimationImage * nnanimation_image_new(NNImage *image, float duration);
NNAnimationFling * nnanimation_fling_new(int x, int y, int width, int height, float min_vel_x, float min_vel_y, NNAnimation *animation);
void nnanimation_add_image(NNAnimation *animation, NNImage *image, float duration);
void nnanimation_free(NNAnimation *animation);

#endif /* NNANIMATION_H_ */
