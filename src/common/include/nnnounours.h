/*
 *  Created on: Feb 20, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNNOUNOURS_H_
#define NNNOUNOURS_H_

#include <sys/time.h>
#include <sys/types.h>
#include "nncommon.h"
#include "nnuinounours.h"
#include "nnimage.h"
#include "nnanimation.h"

struct NNTheme;
struct NNAnimation;

typedef struct NNNounoursConfig {
	int shake_factor; // The speed in pixels/second which will trigger the shake animation
	int idle_time_for_sleep_ms;
	int idle_time_for_auto_move_ms;
	nnbool is_in_screensaver_mode;
	nnbool do_stretch;
} NNNounoursConfig;

typedef struct NNNounoursState {
	nnbool is_doing_animation;
	struct NNTheme *cur_theme;
	struct NNImage *cur_image;
	struct NNFeature *cur_feature;
	struct NNAnimation *cur_animation;
	int last_x;
	int last_y;
	long last_motion_event_time_us;
	long last_action_time_us;
} NNNounoursState;

typedef struct NNNounours {
	NNNounoursConfig config;
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
NNNounours * nnnounours_new(const char *path, int screensaver_mode,
		int window_id);

/**
 * Sets the current theme, resizes the window, and shows the default image of this theme.
 */
void nnnounours_use_theme(NNNounours *nounours, struct NNTheme *theme);
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
