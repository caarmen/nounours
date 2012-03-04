/*
 * nnmath.h
 *
 *  Created on: Feb 25, 2012
 *      Author: calvarez
 */

#ifndef NNMATH_H_
#define NNMATH_H_

int nnmath_get_distance(int x1, int y1, int x2, int y2);
int nnmath_is_faster(float v1, float v2);
int nnmath_point_is_in_square(int point_x, int point_y, int square_x, int square_y,
		int square_width, int square_height);
void nnmath_translate(int window_x, int window_y, int window_width, int window_height, int image_width, int image_height, int *image_x, int *image_y);
#endif /* NNMATH_H_ */
