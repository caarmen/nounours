/*
 * nnnounours.h
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#ifndef NNNOUNOURS_H_
#define NNNOUNOURS_H_

#include "nnuinounours.h"

struct NNTheme;

typedef struct NNNounours {
	int fling_factor;
	int fling_precision;
	int shake_factor;
	int idle_time;
	int idle_ping_interval;
	NNUINounours *uinounours;
	struct NNTheme *cur_theme;
} NNNounours;

NNNounours * nnnounours_new();
void nnnounours_use_theme(NNNounours *nounours, struct NNTheme *theme);
void nnnounours_free(NNNounours *nounours);

#endif /* NNNOUNOURS_H_ */
