/*
 * main.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */
#include <stdio.h>
#include <X11/Xlib.h>
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

	int i;

	printf("Theme %s\n", theme->id);
	for (i = 0; i < theme->num_animations; i++) {
		NNAnimation *animation = theme->animations[i];
		printf("Animation: %s, %s, interval=%d, repeat=%d\n", animation->id,
				animation->label, animation->interval, animation->repeat);
		int j;
		for (j = 0; j < animation->num_images; j++) {
			NNAnimationImage *animation_image = animation->images[j];
			NNImage *image = animation_image->image;
			printf("  Image %s with duration %.2f\n", image->filename,
					animation_image->duration);
		}
	}
	for (i=0; i < theme->num_images; i++) {
		NNImage *image = theme->images[i];
		int k;
		printf(" %s:\n", image->id);
		printf("  Image Features:\n");
		for (k = 0; k < image->num_image_features; k++) {
			NNImageFeature *image_feature = image->image_features[k];
			printf("   feature %s at %d x %d\n", image_feature->feature->id,
					image_feature->x, image_feature->y);
		}
		printf("  Adjacent images:\n");
		for (k = 0; k < image->num_adjacent_images; k++) {
			NNAdjacentImages *adjacent_images = image->adjacent_images[k];
			NNFeature *feature = adjacent_images->feature;
			int l;
			for(l=0; l < adjacent_images->num_adjacent_images; l++) {
				NNImage *adjacent_image = adjacent_images->adjacent_images[l];
				printf("   adjacent image %s->%s\n",
						feature->id,
						adjacent_image->id);
			}
		}
	}
	printf("Flings\n");
	for(i=0; i < theme->num_animation_flings; i++) {
		NNAnimationFling *fling = theme->animation_flings[i];
		printf("%s velx=%f, vely=%f\n", fling->animation->label, fling->min_vel_x, fling->min_vel_y);
	}

	printf("Properties:\n");
	printf("  idle ping interval=%d\n", nounours->idle_ping_interval);
	nnnounours_use_theme(nounours, theme);


	for (i=0; i < theme->num_images; i++) {
		NNImage *image = theme->images[i];
		nnuiimage_show(nounours->uinounours, image->uiimage);
		usleep(250000);

	}
	return 0;
}
