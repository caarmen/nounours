/*
 * nnanimation.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include <stdlib.h>
#include "nnanimation.h"
#include "nndatacommon.h"

NNAnimation * nnanimation_new(const char *id, const char *label, int interval, int repeat) {
	NNAnimation *animation = malloc(sizeof(NNAnimation));
	animation->images = malloc(sizeof(NNAnimationImage*)*NN_INITIAL_LIST_CAPACITY);
	animation->id = id;
	animation->label = label;
	animation->interval = interval;
	animation->repeat = repeat;
	animation->num_images = 0;
	return animation;
}

NNAnimationImage * nnanimation_image_new(NNImage *image, float duration) {
	NNAnimationImage *animation_image = malloc(sizeof(NNAnimationImage));
	animation_image->duration = duration;
	animation_image->image = image;
	return animation_image;
}

void nnanimation_add_image(NNAnimation *animation, NNImage *image, float duration) {
	NNAnimationImage *animation_image = nnanimation_image_new(image, duration);
	nnresize_if_needed(animation->images, animation->num_images);
	animation->images[animation->num_images++] = animation_image;
}

void nnanimation_free(NNAnimation *animation) {
	int i;
	for(i=0; i < animation->num_images; i++) {
		free(animation->images[i]);
	}
	free(animation);
}

