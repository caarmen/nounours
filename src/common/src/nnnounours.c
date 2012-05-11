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

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "nnnounours.h"
#include "nntheme.h"
#include "nnpropertiesreader.h"
#include "nnmath.h"

static void nnnounours_ping(NNNounours *nounours);

static void * nnnounours_ping_thread(void *data) {
	NNNounours * nounours = (NNNounours*) data;
	while (1) {
		sleep(nounours->app->config.idle_time_for_auto_move_ms / 1000);
		nnnounours_ping(nounours);
	}
	return NULL;
}

static void nnnounours_reset_idle(NNNounours *nounours) {
	struct timeval now_tv;
	gettimeofday(&now_tv, NULL);
	nounours->state.last_action_time_us = now_tv.tv_sec * 1000000
			+ now_tv.tv_usec;
}

NNNounours * nnnounours_new(char *id, NNNounoursApp *app, const char *path,
		nnbool screensaver_mode, int window_id) {
	NNNounours *nounours = malloc(sizeof(NNNounours));
	nounours->app = app;
	nounours->id = id;

	nounours->state.is_doing_animation = NNFALSE;
	nounours->state.cur_image = 0;
	nounours->state.cur_feature = 0;
	nounours->state.cur_animation = 0;
	nounours->state.last_x = -1;
	nounours->state.last_y = -1;
	nounours->state.last_motion_event_time_us = 0;

	nounours->uinounours = nnuinounours_new(app->ui, nounours, window_id);

	pthread_mutex_init(&nounours->animation_mutex, 0);
	pthread_cond_init(&nounours->animation_cond, 0);
	pthread_create(&nounours->ping_thread, NULL, nnnounours_ping_thread,
			nounours);

	// Mark the last activity as happening now.
	nnnounours_reset_idle(nounours);
	return nounours;
}

void nnnounours_show_image(NNNounours *nounours, NNImage *image) {
	if (nounours->state.cur_image == image)
		return;
	nounours->state.cur_image = image;
	nnuinounours_show_image(nounours->uinounours, image->uiimage);
}

static void *nnnounours_animation_thread(void *data) {
	NNNounours *nounours = (NNNounours*) data;
	if (nounours->state.is_doing_animation)
		return NULL;
	NNAnimation *animation = nounours->state.cur_animation;
	if (animation == NULL)
		return NULL;
	nounours->state.is_doing_animation = NNTRUE;
	nnanimation_start(nounours, animation);
	nounours->state.is_doing_animation = NNFALSE;
	nounours->state.cur_animation = 0;
	// Special case if this is the "sleep" animation:
	// If we don't mark the end of this animation as
	// an action (resetting the idle timeout), then
	// nounours will sleep forever.
	if (animation == nounours->app->config.theme->animation_idle)
		nnnounours_reset_idle(nounours);
	return (void*) NULL;
}

void nnnounours_start_animation(NNNounours *nounours, NNAnimation *animation) {
	if (animation == NULL)
		return;
	if (nounours->state.is_doing_animation)
		nnnounours_stop_animation(nounours);
	nounours->state.cur_animation = animation;
	pthread_create(&nounours->animation_thread, NULL,
			nnnounours_animation_thread, nounours);
}

void nnnounours_stop_animation(NNNounours *nounours) {
	if (!nounours->state.is_doing_animation)
		return;
	nounours->state.is_doing_animation = NNFALSE;
	nounours->state.cur_animation = 0;
	pthread_cond_signal(&nounours->animation_cond);
}

void nnnounours_on_press(NNNounours *nounours, int x, int y) {
	nnbool was_idle = nounours->state.is_doing_animation
			&& nounours->state.cur_animation
					== nounours->app->config.theme->animation_idle;
	nnnounours_stop_animation(nounours);
	nnnounours_reset_idle(nounours);
	if (was_idle) {
		nnnounours_start_animation(nounours,
				nounours->app->config.theme->animation_idle_end);
	} else {
		nounours->state.cur_feature = nnimage_find_closest_feature(
				nounours->state.cur_image, x, y);
		if (nounours->state.cur_feature != 0) {
			NNAdjacentImages *adjacent_images = nnimage_find_adjacent_images(
					nounours->state.cur_image, nounours->state.cur_feature);
			if (adjacent_images == 0) {
				nounours->state.cur_image =
						nounours->app->config.theme->default_image;
			}
		}
	}
}

void nnnounours_on_move(NNNounours *nounours, int x, int y) {
	if (nounours->state.is_doing_animation)
		return;

	// Check if we've moved one of nounours' features far
	// enough that we should change the image.
	if (nounours->state.cur_feature != 0) {
		NNImage *image = nnimage_find_adjacent_image(nounours->state.cur_image,
				nounours->state.cur_feature, x, y);
		nnnounours_show_image(nounours, image);
	}
	struct timeval now_tv;
	errno = 0;
	int res = gettimeofday(&now_tv, NULL);
	u_int64_t now_us = now_tv.tv_sec * 1000000 + now_tv.tv_usec;

	// Check if we've moved fast enough to consider it a fling gesture.
	if (nounours->state.last_x >= 0) {
		u_int64_t time_diff = now_us
				- nounours->state.last_motion_event_time_us;
		if (time_diff > 0) {
			float vel_x = (float) 1000000 * (x - nounours->state.last_x)
					/ time_diff;
			float vel_y = (float) 1000000 * (y - nounours->state.last_y)
					/ time_diff;
			nnnounours_on_fling(nounours, x, y, vel_x, vel_y);
		}
	}
	nounours->state.last_motion_event_time_us = now_us;
	nounours->state.last_x = x;
	nounours->state.last_y = y;
	nnnounours_reset_idle(nounours);
}

void nnnounours_on_release(NNNounours *nounours, int x, int y) {
	if (nounours->state.is_doing_animation)
		return;

	if (nounours->state.cur_image->release != 0)
		nnnounours_show_image(nounours, nounours->state.cur_image->release);
	nnnounours_reset_idle(nounours);
}

void nnnounours_on_fling(NNNounours *nounours, int x, int y, float vel_x,
		float vel_y) {

	int i;
	NNTheme *theme = nounours->app->config.theme;
	// Find an animation that matches this fling:
	// Check the speed in the x and y directions, and check
	// if the pointer was in the right zone.
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
		return;
	}
}

void nnnounours_on_shake(NNNounours *nounours) {
	nnnounours_reset_idle(nounours);
	nnnounours_start_animation(nounours,
			nounours->app->config.theme->shake_animation);
}

static void nnnounours_ping(NNNounours *nounours) {
	// Don't do anything if we're not done loading
	// or if we're in the middle of an animation.
	if (nounours->app->config.theme == 0 || nounours->state.is_doing_animation
			|| nounours->state.cur_image == 0)
		return;
	struct timeval now_tv;
	gettimeofday(&now_tv, NULL);
	long now_us = now_tv.tv_sec * 1000000 + now_tv.tv_usec;
	u_int64_t time_diff = now_us - nounours->state.last_action_time_us;
	// Check if we've been idle long enough to fall asleep
	if (time_diff > nounours->app->config.idle_time_for_sleep_ms * 1000) {
		nnnounours_stop_animation(nounours);
		nnnounours_start_animation(nounours,
				nounours->app->config.theme->animation_idle);
	}
	// Check if we've been idle long enough to start moving on our own.
	else if (time_diff
			> nounours->app->config.idle_time_for_auto_move_ms * 1000) {
		NNAnimation *animation = nnanimation_create_random(nounours);
		nnnounours_start_animation(nounours, animation);
	}
}

void nnnounours_free(NNNounours *nounours) {
	free(nounours->id);
	free(nounours);
}
