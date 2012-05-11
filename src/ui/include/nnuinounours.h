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

#ifndef NNUINOUNOURS_H_
#define NNUINOUNOURS_H_

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
