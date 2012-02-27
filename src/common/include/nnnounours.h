/*
 * nnnounours.h
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#ifndef NNNOUNOURS_H_
#define NNNOUNOURS_H_

#include <sys/time.h>
#include "nnuinounours.h"
#include "nnimage.h"
#include "nnanimation.h"

struct NNTheme;
struct NNAnimation;

typedef struct NNNounours {
	int is_doing_animation;
	int fling_factor;
	int fling_precision;
	int shake_factor;
	int idle_time;
	int idle_ping_interval;
	struct NNUINounours *uinounours;
	struct NNTheme *cur_theme;
	struct NNImage *cur_image;
	struct NNFeature *cur_feature;
	struct NNAnimation *cur_animation;
	int last_x;
	int last_y;
	long last_motion_event_time_us;
	long last_action_time_us;
	pthread_t animation_thread;
	pthread_cond_t animation_cond;
	pthread_mutex_t animation_mutex;
	pthread_t ping_thread;
	int screensaver_mode;
} NNNounours;

NNNounours * nnnounours_new(const char *path, int screensaver_mode, int window_id);
void nnnounours_use_theme(NNNounours *nounours, struct NNTheme *theme);
void nnnounours_show_image(NNNounours *nounours, struct NNImage *image);
void nnnounours_start_animation(NNNounours *nounours, struct NNAnimation *animation);
void nnnounours_stop_animation(NNNounours *nounours);
void nnnounours_on_press(NNNounours *nounours, int x, int y);
void nnnounours_on_move(NNNounours *nounours, int x, int y);
void nnnounours_on_release(NNNounours *nounours, int x, int y);
void nnnounours_on_fling(NNNounours *nounours, int x, int y, float vel_x, float vel_y);
void nnnounours_on_shake(NNNounours *nounours);
void nnnounours_ping(NNNounours *nounours);
void nnnounours_free(NNNounours *nounours);

#endif /* NNNOUNOURS_H_ */
