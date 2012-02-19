/*
 * nimage.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NIMAGE_H_
#define NIMAGE_H_

typedef struct NImage {
	const char *id;
	const char *filename;
	struct NImage **adjacent_images;
} NImage;

NImage * nimage_new(const char *id, const char *filename);
void nimage_add_adjacent_image(NImage *image, NImage *adjacent_image);
void nimage_free(NImage *image);
#endif /* NIMAGE_H_ */
