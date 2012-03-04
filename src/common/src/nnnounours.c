/*
 * nnnounours.c
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#include <stdlib.h>
#include <pthread.h>
#include "nnnounours.h"
#include "nntheme.h"
#include "nnpropertiesreader.h"
#include "nnmath.h"

static void * nnnounours_ping_thread(void *data) {
	NNNounours * nounours = (NNNounours*) data;
	while (1) {
		sleep(nounours->idle_ping_interval / 1000);
		nnnounours_ping(nounours);
	}
}

static void nnnounours_reset_idle(NNNounours *nounours) {
	struct timeval now_tv;
	gettimeofday(&now_tv, NULL);
	nounours->last_action_time_us = now_tv.tv_sec * 1000000 + now_tv.tv_usec;
}

NNNounours * nnnounours_new(const char *path, int screensaver_mode, int window_id) {
	NNNounours *nounours = malloc(sizeof(NNNounours));
	nounours->screensaver_mode = screensaver_mode;
	nounours->do_stretch = 0;
	nounours->is_doing_animation = 0;
	nounours->fling_factor = 0;
	nounours->shake_factor = 0;
	nounours->fling_precision = 0;
	nounours->idle_time = 0;
	nounours->idle_ping_interval = 0;
	nounours->uinounours = nnuinounours_new(nounours, window_id);
	nounours->cur_theme = 0;
	nounours->cur_image = 0;
	nounours->cur_feature = 0;
	nounours->cur_animation = 0;
	nounours->last_x = -1;
	nounours->last_y = -1;
	nounours->last_motion_event_time_us = 0;
	nnread_nounours_properties_file(nounours, path);
	nnuinounours_start_loop(nounours->uinounours);
	pthread_create(&nounours->ping_thread, NULL, nnnounours_ping_thread,
			nounours);
	pthread_mutex_init(&nounours->animation_mutex, 0);
	pthread_cond_init(&nounours->animation_cond, 0);
	nnnounours_reset_idle(nounours);
	return nounours;
}

void nnnounours_use_theme(NNNounours *nounours, NNTheme *theme) {
	nounours->cur_theme = theme;
	if(nounours->do_stretch)
		nnuinounours_stretch(nounours->uinounours);
	else
		nnuinounours_resize(nounours->uinounours, theme->width, theme->height);
	nnnounours_show_image(nounours, theme->default_image);
}

void nnnounours_show_image(NNNounours *nounours, NNImage *image) {
	if (nounours->cur_image == image)
		return;
	nounours->cur_image = image;
	nnuinounours_show_image(nounours->uinounours, image->uiimage);
}

static void *nnnounours_animation_thread(void *data) {
	NNAnimation *animation = (NNAnimation*) data;
	NNNounours *nounours = animation->nounours;
	if (nounours->is_doing_animation)
		return;
	nounours->cur_animation = animation;
	nounours->is_doing_animation = 1;
	if (!animation->is_random)
		nnnounours_reset_idle(nounours);
	nnanimation_start(animation);
	nounours->is_doing_animation = 0;
	nounours->cur_animation = 0;
	return (void*) NULL;
}
void nnnounours_start_animation(NNNounours *nounours, NNAnimation *animation) {
	if (animation == NULL)
		return;
	if (nounours->is_doing_animation)
		nnnounours_stop_animation(nounours);
	pthread_create(&nounours->animation_thread, NULL,
			nnnounours_animation_thread, animation);
}
void nnnounours_stop_animation(NNNounours *nounours) {
	if (!nounours->is_doing_animation)
		return;
	nounours->is_doing_animation = 0;
	nounours->cur_animation = 0;
	pthread_cond_signal(&nounours->animation_cond);
}
void nnnounours_on_press(NNNounours *nounours, int x, int y) {
	int was_idle = nounours->is_doing_animation
			&& nounours->cur_animation == nounours->cur_theme->animation_idle;
	nnnounours_stop_animation(nounours);
	nnnounours_reset_idle(nounours);
	if (was_idle) {
		nnnounours_start_animation(nounours,
				nounours->cur_theme->animation_idle_end);
	} else {
		nounours->cur_feature = nnimage_find_closest_feature(
				nounours->cur_image, x, y);
		if (nounours->cur_feature != 0) {
			NNAdjacentImages *adjacent_images = nnimage_find_adjacent_images(
					nounours->cur_image, nounours->cur_feature);
			if (adjacent_images == 0) {
				nounours->cur_image = nounours->cur_theme->default_image;
			}
		}
	}
}
void nnnounours_on_move(NNNounours *nounours, int x, int y) {
	if (nounours->is_doing_animation)
		return;

	NNImage *image = nnimage_find_adjacent_image(nounours->cur_image,
			nounours->cur_feature, x, y);
	nnnounours_show_image(nounours, image);
	struct timeval now_tv;
	gettimeofday(&now_tv, NULL);
	long now_us = now_tv.tv_sec * 1000000 + now_tv.tv_usec;
	if (nounours->last_x >= 0) {
		long time_diff = now_us - nounours->last_motion_event_time_us;
		if(time_diff == 0)
			time_diff = 1;
		float vel_x = 1000000 * (x - nounours->last_x) / time_diff;
		float vel_y = 1000000 * (y - nounours->last_y) / time_diff;
		nnnounours_on_fling(nounours, x, y, vel_x, vel_y);
	}
	nounours->last_motion_event_time_us = now_us;
	nounours->last_x = x;
	nounours->last_y = y;
	nnnounours_reset_idle(nounours);
}
void nnnounours_on_release(NNNounours *nounours, int x, int y) {
	if (nounours->is_doing_animation)
		return;

	if (nounours->cur_image->release != 0)
		nnnounours_show_image(nounours, nounours->cur_image->release);
	nnnounours_reset_idle(nounours);
}
void nnnounours_on_fling(NNNounours *nounours, int x, int y, float vel_x,
		float vel_y) {
	int i;
	NNTheme *theme = nounours->cur_theme;
	for (i = 0; i < theme->num_animation_flings; i++) {
		NNAnimationFling *fling = theme->animation_flings[i];
		if (!nnmath_is_faster(vel_x, fling->min_vel_x))
			continue;
		if (!nnmath_is_faster(vel_y, fling->min_vel_y))
			continue;
		if (!nnmath_point_is_in_square(x, y, fling->x, fling->y, fling->width,
				fling->height))
			continue;
		nnnounours_start_animation(nounours, fling->animation);
	}
}
void nnnounours_on_shake(NNNounours *nounours) {
	nnnounours_start_animation(nounours, nounours->cur_theme->shake_animation);
}

void nnnounours_ping(NNNounours *nounours) {
	if (nounours->cur_theme == 0 || nounours->is_doing_animation || nounours->cur_image == 0)
		return;
	struct timeval now_tv;
	gettimeofday(&now_tv, NULL);
	long now_us = now_tv.tv_sec * 1000000 + now_tv.tv_usec;
	long time_diff = now_us - nounours->last_action_time_us;
	if (time_diff > nounours->idle_time * 1000) {
		nnnounours_stop_animation(nounours);
		nnnounours_start_animation(nounours,
				nounours->cur_theme->animation_idle);
	} else if (time_diff > nounours->idle_ping_interval * 1000) {
		NNAnimation *animation = nnanimation_create_random(nounours);
		nnnounours_start_animation(nounours, animation);
	}
}

void nnnounours_free(NNNounours *nounours) {
	free(nounours);
}
