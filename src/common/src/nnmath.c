/*
 *  Created on: Feb 25, 2012
 *      Author: Carmen Alvarez
 */
#include <math.h>
#include "nnmath.h"

int nnmath_get_distance(int x1, int y1, int x2, int y2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

nnbool nnmath_is_faster(float v1, float v2) {
	if (v2 <= 0 && v1 <= v2)
		return NNTRUE;
	if (v2 >= 0 && v1 >= v2)
		return NNTRUE;
	return NNFALSE;
}

nnbool nnmath_point_is_in_square(int point_x, int point_y, int square_x,
		int square_y, int square_width, int square_height) {
	if (point_x >= square_x && point_x <= (square_x + square_width)
			&& point_y >= square_y && point_y <= (square_y + square_height))
		return NNTRUE;
	return NNFALSE;
}

void nnmath_translate(int window_x, int window_y, int window_width,
		int window_height, int image_width, int image_height, int *image_x,
		int *image_y) {
	float height_ratio = (float) window_height / image_height;
	float width_ratio = (float) window_width / image_width;
	float ratio_to_use =
			height_ratio > width_ratio ? width_ratio : height_ratio;
	int offset_x = 0;
	int offset_y = 0;
	if (height_ratio > width_ratio) {
		offset_y = (int) ((window_height - ratio_to_use * image_height) / 2);
	} else {
		offset_x = (int) ((window_width - ratio_to_use * image_width) / 2);
	}
	*image_x = (int) ((window_x - offset_x) / ratio_to_use);
	*image_y = (int) ((window_y - offset_y) / ratio_to_use);
	printf("translate: window %dx%d @ %dx%d, image %dx%d @ %dx%d, result=%dx%d\n",
			window_width, window_height, window_x, window_y, image_width, image_height, offset_x, offset_y, *image_x, *image_y);
}

