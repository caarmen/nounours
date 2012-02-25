/*
 * nnnounours.c
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#include <stdlib.h>
#include "nnnounours.h"
#include "nntheme.h"
#include "nnpropertiesreader.h"

NNNounours * nnnounours_new() {
	NNNounours *nounours = malloc(sizeof(NNNounours));
	nounours->fling_factor = 0;
	nounours->shake_factor = 0;
	nounours->fling_precision = 0;
	nounours->idle_time = 0;
	nounours->idle_ping_interval = 0;
	nounours->uinounours = nnuinounours_new(nounours);
	nounours->cur_theme = 0;
	nounours->cur_image = 0;
	nounours->cur_feature = 0;
	nnread_nounours_properties_file(nounours);
	nnuinounours_start_loop(nounours->uinounours);

	return nounours;
}
void nnnounours_use_theme(NNNounours *nounours, NNTheme *theme) {
	nounours->cur_theme = theme;
	nnuinounours_resize(nounours->uinounours, theme->width, theme->height);
	nnnounours_show_image(nounours, theme->default_image);
}

void nnnounours_show_image(NNNounours *nounours, NNImage *image) {
	if(nounours->cur_image == image)
		return;
	nounours->cur_image = image;
	nnuinounours_notify(nounours->uinounours, image->uiimage);
}
void nnnounours_on_press(NNNounours *nounours, int x, int y) {
	nounours->cur_feature = nnimage_find_closest_feature(nounours->cur_image, x, y);
	if(nounours->cur_feature != 0) {
		NNAdjacentImages *adjacent_images = nnimage_find_adjacent_images(nounours->cur_image, nounours->cur_feature);
		if(adjacent_images == 0) {
			nounours->cur_image = nounours->cur_theme->default_image;
		}
	}
}
void nnnounours_on_move(NNNounours *nounours, int x, int y) {
	NNImage *image = nnimage_find_adjacent_image(nounours->cur_image, nounours->cur_feature, x, y);
	nnnounours_show_image(nounours, image);
}
void nnnounours_on_release(NNNounours *nounours, int x, int y) {
	if(nounours->cur_image->release != 0)
		nnnounours_show_image(nounours, nounours->cur_image->release);
}

void nnnounours_free(NNNounours *nounours) {
	free(nounours);
}
