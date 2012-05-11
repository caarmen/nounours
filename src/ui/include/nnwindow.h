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

#ifndef NNWINDOW_H_
#define NNWINDOW_H_

#include "nnuinounoursapp.h"

/**
 * Find or create a window in which nounours will run.
 */
void nnwindow_setup(NNUINounoursApp *uiapp);

/**
 * Set the icon for the window.
 */
void nnwindow_set_icon(NNUINounoursApp *uiapp, char *icon_filename);

/**
 * The window we're drawing to is in use.  Find another
 * window to draw to.
 */
void nnwindow_switch(NNUINounoursApp *uiapp);

/**
 * Stretch the images to fill the screen horizontally or vertically.
 */
void nnwindow_stretch(NNUINounoursApp *uiapp);

/**
 * Resize the window to the given size.  Also resize the theme images accordingly.
 */
void nnwindow_resize(NNUINounoursApp *uiapp, int width, int height);

/**
 * Retrieve geometrical information about the app.  The screen may have one or multiple tiles of nounours.
 * If we are in full screen mode (-stretch), then (unless the aspect ratio of the combined tiles
 * is exactly the same as the aspect ratio of the screen) there will be either horizontal or vertical
 * margins.
 * offset_x, offset_y: The distance in pixels to the upper-left corner of the upper-leftmost tile (for -stretch mode).
 *
 * image_width, image_height: The size of the tiles on the screen (may not equal the original size
 * of the jpeg images).
 */
void nnwindow_get_dimensions(NNUINounoursApp *uiapp, int *offset_x,
		int *offset_y, int *image_width, int *image_height);

#endif /* NNWINDOW_H_ */
