/*
 *  Created on: Feb 19, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNANIMATION_H_
#define NNANIMATION_H_
#include "nnimage.h"
#include "nnnounours.h"
#include "nncommon.h"

struct NNNounours;

/**
 * One frame in the animation.
 */
typedef struct NNAnimationImage {
	struct NNImage *image;
	float duration;
} NNAnimationImage;

typedef struct NNAnimation {
	char *id;
	char *label; // Not used yet.  Maybe for version 2 :)
	int num_images;
	NNAnimationImage **images;
	int interval_ms; // base duration for each frame
	int repeat;
	nnbool is_preset; // one of our preconfigured animations
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

NNAnimation * nnanimation_new(char *id, char *label, int interval, int repeat);
NNAnimationImage * nnanimation_image_new(struct NNImage *image, float duration);
NNAnimationFling * nnanimation_fling_new(int x, int y, int width, int height, float min_vel_x, float min_vel_y, NNAnimation *animation);
NNAnimation * nnanimation_create_random(struct NNNounours *nounours);
void nnanimation_add_image(NNAnimation *animation, struct NNImage *image, float duration);
void nnanimation_start(struct NNNounours *nounours, NNAnimation *animation);
void nnanimation_stop(NNAnimation *animation);
void nnanimation_free(NNAnimation *animation);

#endif /* NNANIMATION_H_ */
