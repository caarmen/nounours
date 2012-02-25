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
	nnread_nounours_properties_file(nounours);
	nnuinounours_start_loop(nounours->uinounours);

	return nounours;
}
void nnnounours_use_theme(NNNounours *nounours, NNTheme *theme) {
	nounours->cur_theme = theme;
	nnuinounours_resize(nounours->uinounours, theme->width, theme->height);
}

void nnnounours_show_image(NNNounours *nounours, NNImage *image) {
	nounours->cur_image = image;
	nnuinounours_notify(nounours->uinounours, image->uiimage);
}
void nnnounours_free(NNNounours *nounours) {
	free(nounours);
}
