/*
 * nnuinounours.c
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */
#include "nnuinounours.h"

NNUINounours *nnuinounours_new() {
	NNUINounours *uinounours = malloc(sizeof(NNUINounours));
	uinounours->display = XOpenDisplay(0);
	uinounours->screen_number = DefaultScreen(uinounours->display);
	return uinounours;
}
void nnuinounours_free(NNUINounours *uinounours) {
	free(uinounours);
}
