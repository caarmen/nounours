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
	struct NNImage *image;
	float duration;
} NNAnimationImage;

typedef struct NNAnimation {
	struct NNNounours *nounours;
	char *id;
	char *label;
	int num_images;
	NNAnimationImage **images;
	int interval;
	int repeat;
	int is_random;
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

NNAnimation * nnanimation_new(struct NNNounours *nounours, char *id, char *label, int interval, int repeat);
NNAnimation * nnanimation_create_random(struct NNNounours *nounours);
void nnanimation_start(NNAnimation *animation);
void nnanimation_stop(NNAnimation *animation);
NNAnimationImage * nnanimation_image_new(struct NNImage *image, float duration);
NNAnimationFling * nnanimation_fling_new(int x, int y, int width, int height, float min_vel_x, float min_vel_y, NNAnimation *animation);
void nnanimation_add_image(NNAnimation *animation, struct NNImage *image, float duration);
void nnanimation_free(NNAnimation *animation);

#endif /* NNANIMATION_H_ */
