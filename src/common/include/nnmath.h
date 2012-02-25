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
#endif /* NNMATH_H_ */
