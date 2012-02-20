/*
 * nnuinounours.h
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#ifndef NNUINOUNOURS_H_
#define NNUINOUNOURS_H_
#include <X11/Xlib.h>

typedef struct NNUINounours {
	Display *display;
	int screen_number;
} NNUINounours;

NNUINounours *nnuinounours_new();
void nnuinounours_free(NNUINounours *nounours);

#endif /* NNUINOUNOURS_H_ */
