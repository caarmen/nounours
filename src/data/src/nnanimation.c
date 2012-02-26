/*
 * nnanimation.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include "nnanimation.h"
#include "nncommon.h"
#include "nntheme.h"

NNAnimation * nnanimation_new(NNNounours *nounours, char *id, char *label,
		int interval, int repeat) {
	NNAnimation *animation = malloc(sizeof(NNAnimation));
	animation->nounours = nounours;
	animation->images = malloc(
			sizeof(NNAnimationImage*) * NN_INITIAL_LIST_CAPACITY);
	animation->id = strdup(id);
	animation->label = strdup(label);
	animation->interval = interval;
	animation->repeat = repeat;
	animation->num_images = 0;
	animation->is_random = 0;

	return animation;
}
NNAnimation * nnanimation_create_random(NNNounours *nounours) {
	int interval = 100 + (random() % 400);
	int repeat = 1;
	int num_frames = 2 + (random() % 8);
	char *id = strdup("random");
	char *label = strdup("label");

	NNAnimation * animation = nnanimation_new(nounours, id, label, interval, repeat);
	animation->is_random = 1;
	NNImage *image = nounours->cur_image;
	int i;
	for (i = 0; i < num_frames; i++) {
		float duration = 0.5f + ((float) random() / RAND_MAX) * 2;
		nnanimation_add_image(animation, image, duration);
		NNImage *next_image = nnimage_get_random_adjacent_image(image);

		image = next_image;
	}
	return animation;
}

void nnanimation_start(NNAnimation *animation) {
	NNNounours *nounours = animation->nounours;
	int i;
	for (i = 0; i < animation->repeat && nounours->is_doing_animation; i++) {
		int j;
		for (j = 0; j < animation->num_images && nounours->is_doing_animation; j++) {
			nnnounours_show_image(nounours,
					animation->images[j]->image);
			long sleep_duration_ns = (animation->images[j]->duration
					* animation->interval)*1000000;

	        struct timeval tv;
	        struct timespec ts;
	        gettimeofday(&tv, NULL);

	        ts.tv_sec = tv.tv_sec;
	        ts.tv_nsec = tv.tv_usec*1000;
	        long secs_to_add = (sleep_duration_ns + ts.tv_nsec)/1000000000;
	        long nsecs_to_add = sleep_duration_ns + ts.tv_nsec - secs_to_add*1000000000;
	        ts.tv_sec += secs_to_add;
	        ts.tv_nsec = nsecs_to_add;
			pthread_cond_timedwait(&nounours->animation_cond, &nounours->animation_mutex, &ts);
		}
	}

	nnnounours_show_image(animation->nounours,
			animation->nounours->cur_theme->default_image);
	if (animation->is_random)
		nnanimation_free(animation);
}

NNAnimationImage * nnanimation_image_new(NNImage *image, float duration) {
	NNAnimationImage *animation_image = malloc(sizeof(NNAnimationImage));
	animation_image->duration = duration;
	animation_image->image = image;
	return animation_image;
}

NNAnimationFling * nnanimation_fling_new(int x, int y, int width, int height,
		float min_vel_x, float min_vel_y, NNAnimation *animation) {
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

void nnanimation_add_image(NNAnimation *animation, NNImage *image,
		float duration) {
	NNAnimationImage *animation_image = nnanimation_image_new(image, duration);
	animation->images = nnresize_if_needed(animation->images,
			animation->num_images);
	animation->images[animation->num_images++] = animation_image;
}

void nnanimation_free(NNAnimation *animation) {
	int i;
	free(animation->id);
	free(animation->label);
	for (i = 0; i < animation->num_images; i++) {
		free(animation->images[i]);
	}
	free(animation);
}

