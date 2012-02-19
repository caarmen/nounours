/*
 * main.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */
#include <stdio.h>
#include "nnimage.h"
#include "nnfeature.h"
#include "nnanimation.h"
#include "nntheme.h"
#include "nncsvreader.h"
int main(int argc, char **argv) {
	NNImage *image1 = nnimage_new("123", "sittingdown.jpg");
	NNImage *image2 = nnimage_new("124", "lookingup.jpg");

	NNFeature *feature = nnfeature_new("1");
	nnimage_add_feature(image1, feature, 34, 23);
	nnimage_add_adjacent_image(image1, image2);
	NNAnimation *animation1 = nnanimation_new("anim1", "jumping", 20, 0);
	nnanimation_add_image(animation1, image1, 1.0f);
	nnanimation_add_image(animation1, image2, 1.5f);
	NNTheme *theme = nntheme_new("theme1");
	nntheme_add_animation(theme, animation1);
	nnread_feature_file(theme, "data/themes/1/feature.csv");
	nnread_image_file(theme, "data/themes/1/image.csv");
	nnread_animation_file(theme, "data/themes/1/animation.csv");
	nnread_image_feature_file(theme, "data/themes/1/imagefeatureassoc.csv");
	nnread_adjacent_image_file(theme, "data/themes/1/adjacentimage.csv");

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
			printf("Image %s with duration %.2f\n", image->filename,
					animation_image->duration);
			int k;
			printf("  Image Features:\n");
			for (k = 0; k < image->num_image_features; k++) {
				NNImageFeature *image_feature = image->image_features[k];
				printf("   feature %s at %d x %d\n", image_feature->feature->id,
						image_feature->x, image_feature->y);
			}
			printf("  Adjacent images:\n");
			for (k = 0; k < image->num_adjacent_images; k++) {
				printf("   adjacent image %s\n", image->adjacent_images[k]->id);
			}

		}
	}

	return 0;
}
