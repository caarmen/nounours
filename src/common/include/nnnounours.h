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

#ifndef NNNOUNOURS_H_
#define NNNOUNOURS_H_

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "nnnounoursapp.h"
#include "nncommon.h"
#include "nnuinounours.h"
#include "nnimage.h"
#include "nnanimation.h"

struct NNTheme;
struct NNAnimation;
struct NNNounoursApp;

typedef struct NNNounoursState {
	nnbool is_doing_animation;
	struct NNImage *cur_image;
	struct NNFeature *cur_feature;
	struct NNAnimation *cur_animation;
	int last_x;
	int last_y;
	u_int64_t last_motion_event_time_us;
	u_int64_t last_action_time_us;
} NNNounoursState;

typedef struct NNNounours {
	char *id;
	struct NNNounoursApp *app;
	NNNounoursState state;
	struct NNUINounours *uinounours;
	pthread_t animation_thread;
	pthread_cond_t animation_cond;
	pthread_mutex_t animation_mutex;
	pthread_t ping_thread;
} NNNounours;

/**
 * Create a new NNNounours, read in the properties file, and start the UI thread.
 */
NNNounours * nnnounours_new(char *id, struct NNNounoursApp *app,
		const char *path, nnbool screensaver_mode, int window_id);

/**
 * Sets the current theme, resizes the window, and shows the default image of this theme.
 */
void nnnounours_use_theme(NNNounours *nounours, struct NNTheme *theme);

/**
 * Same as nnnounours_use_theme, except that the window will be scaled by the scale parameter.
 */
void nnnounours_use_theme_scaled(NNNounours *nounours, struct NNTheme *theme,
		float scale);
void nnnounours_show_image(NNNounours *nounours, struct NNImage *image);
void nnnounours_start_animation(NNNounours *nounours,
		struct NNAnimation *animation);
void nnnounours_stop_animation(NNNounours *nounours);
void nnnounours_on_press(NNNounours *nounours, int x, int y);
void nnnounours_on_move(NNNounours *nounours, int x, int y);
void nnnounours_on_release(NNNounours *nounours, int x, int y);
void nnnounours_on_fling(NNNounours *nounours, int x, int y, float vel_x,
		float vel_y);
void nnnounours_on_shake(NNNounours *nounours);
void nnnounours_free(NNNounours *nounours);

#endif /* NNNOUNOURS_H_ */
