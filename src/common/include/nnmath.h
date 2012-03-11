/*
 *  Created on: Feb 25, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNMATH_H_
#define NNMATH_H_

#include "nncommon.h"

/**
 * @return the distance between the two points;
 */
int nnmath_get_distance(int x1, int y1, int x2, int y2);

/**
 * @return true if v1 has the same sign as v2 and abs(v1) > abs(v2). Return false otherwise.
 */
nnbool nnmath_is_faster(float v1, float v2);

/**
 * @return true if the point at point_x,point_y is inside the square with origin square_x,
 * square_y, and with the given size. Return false if the point is outside the square.
 */
nnbool nnmath_point_is_in_square(int point_x, int point_y, int square_x,
		int square_y, int square_width, int square_height);

/**
 * Given a point with coordinates in a window containing an image, find
 * the coordinates of that point relative to the upper-left corner of the image.
 * The coordinates relative to the image will be written to image_x and image_y.
 */
void nnmath_translate(int window_x, int window_y, int window_width,
		int window_height, int image_width, int image_height, int *image_x,
		int *image_y);
#endif /* NNMATH_H_ */
