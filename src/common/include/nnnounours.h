/*
 * nnnounours.h
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#ifndef NNNOUNOURS_H_
#define NNNOUNOURS_H_

#include "nnuinounours.h"
#include "nnimage.h"

struct NNTheme;

typedef struct NNNounours {
	int fling_factor;
	int fling_precision;
	int shake_factor;
	int idle_time;
	int idle_ping_interval;
	struct NNUINounours *uinounours;
	struct NNTheme *cur_theme;
	struct NNImage *cur_image;

} NNNounours;

NNNounours * nnnounours_new();
void nnnounours_use_theme(NNNounours *nounours, struct NNTheme *theme);
void nnnounours_show_image(NNNounours *nounours, struct NNImage *image);
void nnnounours_on_press(NNNounours *nounours, int x, int y);
void nnnounours_on_move(NNNounours *nounours, int x, int y);
void nnnounours_on_release(NNNounours *nounours, int x, int y);
void nnnounours_free(NNNounours *nounours);

#endif /* NNNOUNOURS_H_ */
