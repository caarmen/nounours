/*
 * main.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */
#include <stdio.h>
#include "nnimage.h"
#include "nnfeature.h"

int main(int argc, char **argv) {
	NNImage *image = nnimage_new("123", "sittingdown.jpg");
	NNImage *image2 = nnimage_new("124", "lookingup.jpg");

	NNFeature *feature = nnfeature_new("1");
	nnimage_add_feature(image, feature, 34, 23);
	nnimage_add_adjacent_image(image, image2);
	printf("Hello %s\n", image->filename);
	int i;
	printf("Features:\n");
	for(i=0; i < image->num_features; i++) {
		printf(" feature %s\n", image->features[i]->id);
	}
	printf("Image Features:\n");
	for(i=0; i < image->num_image_features; i++) {
		NNImageFeature *image_feature = image->image_features[i];
		printf(" feature %s at %d x %d\n",
				image_feature->feature->id,
				image_feature->x,
				image_feature->y);
	}
	printf("Adjacent images:\n");
	for(i=0; i < image->num_adjacent_images; i++) {
		printf(" adjacent image %s\n", image->adjacent_images[i]->id);
	}
	return 0;
}
