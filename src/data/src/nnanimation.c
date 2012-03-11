/*
 *  Created on: Feb 19, 2012
 *      Author: Carmen Alvarez
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

NNAnimation * nnanimation_new(char *id, char *label,
		int interval, int repeat) {
	NNAnimation *animation = malloc(sizeof(NNAnimation));
	animation->images = malloc(
			sizeof(NNAnimationImage*) * NN_INITIAL_LIST_CAPACITY);
	animation->id = strdup(id);
	animation->label = strdup(label);
	animation->interval_ms = interval;
	animation->repeat = repeat;
	animation->num_images = 0;
	animation->is_preset = NNFALSE;

	return animation;
}
NNAnimation * nnanimation_create_random(NNNounours *nounours) {
	// every now and then, do one of our preset animations.
	int r = random() % 5;
	if (r == 0) {
		int preset_animation_number = random()
				% nounours->app->config.theme->num_animations;
		return nounours->app->config.theme->animations[preset_animation_number];
	}
	// Random base duration for each frame, from 100 to 500ms
	int interval = 100 + (random() % 400);
	int repeat = 1;

	// Random number of frames, from 2 to 10
	int num_frames = 2 + (random() % 8);
	char *id = strdup("random");
	char *label = strdup("label");

	NNAnimation * animation = nnanimation_new(id, label, interval,
			repeat);
	animation->is_preset = NNFALSE;
	NNImage *image = nounours->state.cur_image;
	int i;
	for (i = 0; i < num_frames; i++) {
		float duration = 0.5f + ((float) random() / RAND_MAX) * 2;
		nnanimation_add_image(animation, image, duration);
		NNImage *next_image = nnimage_get_random_adjacent_image(image);

		image = next_image;
	}
	return animation;
}

void nnanimation_start(NNNounours *nounours, NNAnimation *animation) {
	int i;
	for (i = 0; i < animation->repeat && nounours->state.is_doing_animation;
			i++) {
		int j;
		for (j = 0;
				j < animation->num_images && nounours->state.is_doing_animation;
				j++) {
			// Show this frame
			nnnounours_show_image(nounours, animation->images[j]->image);

			// Now wait for the duration of this frame, before
			// showing the next frame.
			long sleep_duration_ns = (animation->images[j]->duration
					* animation->interval_ms) * 1000000;

			// Create a timespec for X milliseconds in the future
			// (where X is the duration of this frame).
			struct timeval tv;
			struct timespec ts;
			gettimeofday(&tv, NULL);

			ts.tv_sec = tv.tv_sec;
			ts.tv_nsec = tv.tv_usec * 1000;
			long secs_to_add = (sleep_duration_ns + ts.tv_nsec) / 1000000000;
			long nsecs_to_add = sleep_duration_ns + ts.tv_nsec
					- secs_to_add * 1000000000;
			ts.tv_sec += secs_to_add;
			ts.tv_nsec = nsecs_to_add;
			// Wait for the duration of this frame, or until
			// the animation was interrupted.
			pthread_cond_timedwait(&nounours->animation_cond,
					&nounours->animation_mutex, &ts);
		}
	}
	// Saw a problem on cygwin (not ubuntu or mac)
	// if we don't unlock the mutex here.  In subsequent
	// animations, pthread_cond_timedwait was always
	// exiting immediately.
	pthread_mutex_unlock(&nounours->animation_mutex);

	nnnounours_show_image(nounours,
			nounours->app->config.theme->default_image);
	if (!animation->is_preset)
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

