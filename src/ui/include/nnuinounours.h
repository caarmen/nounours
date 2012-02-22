/*
 * nnuinounours.h
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#ifndef NNUINOUNOURS_H_
#define NNUINOUNOURS_H_
#include <X11/Xlib.h>

#include "nnnounours.h"

struct NNNounours;

typedef struct NNUINounours {
	Display *display;
	int screen_number;
	Window window;
	Window root_window;
	GC gc;
	struct NNNounours *nounours;
} NNUINounours;

NNUINounours *nnuinounours_new(struct NNNounours *nounours);
void nnuinounours_resize(NNUINounours *nounours, int width, int height);
void nnuinounours_free(NNUINounours *nounours);
void nnuinounours_notify(NNUINounours *nounours);
void nnuinounours_start_loop(NNUINounours *uinounours);
void nnuinounours_stop_loop();

#endif /* NNUINOUNOURS_H_ */
