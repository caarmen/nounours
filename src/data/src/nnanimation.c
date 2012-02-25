/*
 * nnanimation.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "nnanimation.h"
#include "nncommon.h"

NNAnimation * nnanimation_new(NNNounours *nounours, char *id, char *label, int interval, int repeat) {
	NNAnimation *animation = malloc(sizeof(NNAnimation));
	animation->nounours = nounours;
	animation->images = malloc(sizeof(NNAnimationImage*)*NN_INITIAL_LIST_CAPACITY);
	animation->id = strdup(id);
	animation->label = strdup(label);
	animation->interval = interval;
	animation->repeat = repeat;
	animation->num_images = 0;
	return animation;
}
void nnanimation_show(NNAnimation *animation) {
	int i;
	for(i=0; i < animation->repeat; i++) {
		int j;
		for(j=0; j < animation->num_images; j++) {
			nnnounours_show_image(animation->nounours, animation->images[j]->image);
			long sleep_duration = animation->images[j]->duration * animation->interval * 1000;
			usleep(sleep_duration);
		}
	}
}
NNAnimationImage * nnanimation_image_new(NNImage *image, float duration) {
	NNAnimationImage *animation_image = malloc(sizeof(NNAnimationImage));
	animation_image->duration = duration;
	animation_image->image = image;
	return animation_image;
}

NNAnimationFling * nnanimation_fling_new(int x, int y, int width, int height, float min_vel_x, float min_vel_y, NNAnimation *animation) {
	NNAnimationFling *animation_fling = malloc(sizeof(NNAnimationFling));
	animation_fling->x = x;
	animation_fling->y = y;
	animation_fling->width = width;
	animation_fling->height = height;
	animation_fling->min_vel_x = min_vel_x;
	animation_fling->min_vel_y = min_vel_y;
	animation_fling->animation = animation;
	return animation_fling;
}

void nnanimation_add_image(NNAnimation *animation, NNImage *image, float duration) {
	NNAnimationImage *animation_image = nnanimation_image_new(image, duration);
	animation->images = nnresize_if_needed(animation->images, animation->num_images);
	animation->images[animation->num_images++] = animation_image;
}

void nnanimation_free(NNAnimation *animation) {
	int i;
	free(animation->id);
	free(animation->label);
	for(i=0; i < animation->num_images; i++) {
		free(animation->images[i]);
	}
	free(animation);
}

