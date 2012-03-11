/*
 *  Created on: Feb 20, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNUINOUNOURS_H_
#define NNUINOUNOURS_H_
#include <X11/Xlib.h>
#include <X11/X.h>

#include "nnnounours.h"
#include "nnuiimage.h"
#include "nnuinounoursapp.h"

struct NNNounours;
struct NNUIImage;
struct NNUINounoursApp;

typedef struct NNUINounours {
	struct NNNounours *nounours;
	struct NNUINounoursApp *uiapp;
	int window_width;
	int window_height;
	int window_x;
	int window_y;

} NNUINounours;

NNUINounours *nnuinounours_new(struct NNUINounoursApp *app,
		struct NNNounours *nounours, int window_id);
void nnuinounours_translate(NNUINounours *uinounours, int window_x,
		int window_y, int *image_x, int *image_y);
void nnuinounours_free(NNUINounours *nounours);
void nnuinounours_show_image(NNUINounours *nounours, struct NNUIImage *image);

#endif /* NNUINOUNOURS_H_ */
