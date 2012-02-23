/*
 * nnuiimage.h
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#ifndef NNUIIMAGE_H_
#define NNUIIMAGE_H_
#include "nnuinounours.h"
#include <X11/Xlib.h>

struct NNUINounours;

typedef struct NNUIImage {
	XImage *ximage;
} NNUIImage;

NNUIImage *nnuiimage_new(struct NNUINounours *uinounours, const char *filename);
void nnuiimage_show(struct NNUINounours *uinounours, NNUIImage *image);
void nnuiimage_free(NNUIImage *uiimage);

#endif /* NNUIIMAGE_H_ */
