/*
 * nnuinounours.c
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */
#include <stdlib.h>
#include <X11/Xlib.h>
#include "nnuinounours.h"

NNUINounours *nnuinounours_new() {
	NNUINounours *uinounours = malloc(sizeof(NNUINounours));
	uinounours->display = XOpenDisplay(0);
	uinounours->screen_number = DefaultScreen(uinounours->display);
	uinounours->root_window = DefaultRootWindow(uinounours->display);
	int black_color = BlackPixel(uinounours->display, uinounours->screen_number);
	uinounours->window = XCreateSimpleWindow(uinounours->display, uinounours->root_window, 0, 0, 50,
			50, 0, black_color, black_color);
	XMapWindow(uinounours->display, uinounours->window);
	uinounours->gc = XCreateGC(uinounours->display, uinounours->window, 0, NULL);

	return uinounours;
}
void nnuinounours_resize(NNUINounours *uinounours, int width, int height) {
	XMoveResizeWindow(uinounours->display, uinounours->window, 0, 0, width, height);
}

void nnuinounours_free(NNUINounours *uinounours) {
	free(uinounours);
}
