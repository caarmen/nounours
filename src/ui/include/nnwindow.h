/*
 *  Created on: Mar 12, 2012
 *      Author: Carmen Alvarez
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
