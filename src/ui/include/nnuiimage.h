/*
 *  Created on: Feb 20, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNUIIMAGE_H_
#define NNUIIMAGE_H_
#include <X11/Xlib.h>
#include "nnuinounours.h"
#include "nnuinounoursapp.h"

struct NNUINounours;
struct NNUINounoursApp;

typedef struct NNUIImage {
	XImage *ximage;
} NNUIImage;

NNUIImage *nnuiimage_new(struct NNUINounoursApp *uiapp, const char *filename);
void nnuiimage_resize(struct NNUINounoursApp *uiapp, NNUIImage *uiimage, int dest_width, int dest_height);
void nnuiimage_show(struct NNUINounours *uinounours, NNUIImage *uiimage);
void nnuiimage_free(NNUIImage *uiimage);

#endif /* NNUIIMAGE_H_ */
