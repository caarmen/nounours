/*
 * nnuiimage.h
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#ifndef NNUIIMAGE_H_
#define NNUIIMAGE_H_
#include "nnuinounours.h"

typedef struct NNUIImage {
	XImage *ximage;
} NNUIImage;

NNUIImage *nnuiimage_new(NNUINounours *uinounours, const char *filename);

void nnuiimage_free(NNUIImage *uiimage);

#endif /* NNUIIMAGE_H_ */
