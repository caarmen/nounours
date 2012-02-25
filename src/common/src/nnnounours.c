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
	nnnounours_show_image(nounours, theme->default_image);
}

void nnnounours_show_image(NNNounours *nounours, NNImage *image) {
	nounours->cur_image = image;
	nnuinounours_notify(nounours->uinounours, image->uiimage);
}
void nnnounours_on_press(NNNounours *nounours, int x, int y) {
	printf("on press %dx%d\n", x, y);
	NNFeature *feature = nnimage_find_closest_feature(nounours->cur_image, x, y);
	printf("%s\n", feature->id);
}
void nnnounours_on_move(NNNounours *nounours, int x, int y) {
	printf("on move %dx%d\n", x, y);

}
void nnnounours_on_release(NNNounours *nounours, int x, int y) {
	printf("on release %dx%d\n", x, y);

}

void nnnounours_free(NNNounours *nounours) {
	free(nounours);
}
