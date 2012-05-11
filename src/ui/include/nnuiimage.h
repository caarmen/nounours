/*
 * Copyright 2012 Carmen Alvarez
 *
 * This file is part of Nounours.
 *
 * Nounours is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Nounours is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nounours.  If not, see <http://www.gnu.org/licenses/>.
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
void nnuiimage_resize(struct NNUINounoursApp *uiapp, NNUIImage *uiimage,
		int dest_width, int dest_height);
void nnuiimage_show(struct NNUINounours *uinounours, NNUIImage *uiimage);
void nnuiimage_free(NNUIImage *uiimage);

#endif /* NNUIIMAGE_H_ */
