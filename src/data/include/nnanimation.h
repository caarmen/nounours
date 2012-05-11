/*
 * Copyright 2012 Carmen Alvarez
 *
 * This file is part of Nounours.
 *
 * Nounours is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Nounours is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nounours.  If not, see <http://www.gnu.org/licenses/>.
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
NNAnimationFling * nnanimation_fling_new(int x, int y, int width, int height,
		float min_vel_x, float min_vel_y, NNAnimation *animation);
NNAnimation * nnanimation_create_random(struct NNNounours *nounours);
void nnanimation_add_image(NNAnimation *animation, struct NNImage *image,
		float duration);
void nnanimation_start(struct NNNounours *nounours, NNAnimation *animation);
void nnanimation_stop(NNAnimation *animation);
void nnanimation_free(NNAnimation *animation);

#endif /* NNANIMATION_H_ */
