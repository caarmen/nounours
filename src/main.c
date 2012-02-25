/*
 * main.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */
#include <stdio.h>
#include "nnnounours.h"
#include "nnimage.h"
#include "nnfeature.h"
#include "nnanimation.h"
#include "nntheme.h"
#include "nncsvreader.h"
#include "nnpropertiesreader.h"
int main(int argc, char **argv) {

	NNNounours *nounours = nnnounours_new();

	NNTheme *theme = nntheme_new(nounours, "1");
	nnnounours_use_theme(nounours, theme);
	int i;
	for (i = 0; i < theme->num_animations; i++) {
		NNAnimation *animation = theme->animations[i];
		printf("Animation: %s, %s, interval=%d, repeat=%d\n", animation->id,
				animation->label, animation->interval, animation->repeat);
		nnanimation_show(animation);
	}
	for (i = 0; i < theme->num_images; i++) {
		NNImage *image = theme->images[i];
		nnnounours_show_image(nounours, image);
		usleep(2500000);
	}

	return 0;
}
