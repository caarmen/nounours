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
int main(int argc, char **argv) {
	NNImage *image1 = nnimage_new("123", "sittingdown.jpg");
	NNImage *image2 = nnimage_new("124", "lookingup.jpg");

	NNFeature *feature = nnfeature_new("1");
	nnimage_add_feature(image1, feature, 34, 23);
	nnimage_add_adjacent_image(image1, image2);
	NNAnimation *animation = nnanimation_new("anim1", "jumping", 20, 0);
	nnanimation_add_image(animation, image1, 1.0f);
	nnanimation_add_image(animation, image2, 1.5f);
	int i;
	printf("Animation: %s, %s, interval=%d, repeat=%d\n",
			animation->id, animation->label, animation->interval, animation->repeat);
	for(i=0; i < animation->num_images; i++) {
		NNAnimationImage *animation_image = animation->images[i];
		NNImage *image = animation_image->image;
		printf("Image %s with duration %.2f\n", image->filename, animation_image->duration);

		printf("  Features:\n");
		int j;
		for(j=0; j < image->num_features; j++) {
			printf("   feature %s\n", image->features[j]->id);
		}
		printf("  Image Features:\n");
		for(j=0; j < image->num_image_features; j++) {
			NNImageFeature *image_feature = image->image_features[j];
			printf("   feature %s at %d x %d\n",
					image_feature->feature->id,
					image_feature->x,
					image_feature->y);
		}
		printf("  Adjacent images:\n");
		for(j=0; j < image->num_adjacent_images; j++) {
			printf("   adjacent image %s\n", image->adjacent_images[j]->id);
		}

	}
	return 0;
}
