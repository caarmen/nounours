/*
 * nnuiimage.c
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <unistd.h>
#include <string.h>
#include "nnuiimage.h"

static XImage * nnuiimage_jpeg_to_Ximage(Display *display, int screen_number, const char *filename);

NNUIImage *nnuiimage_new(NNUINounours *uinounours, const char *filename) {
	NNUIImage *uiimage = malloc(sizeof(NNUIImage));
	uiimage->ximage = nnuiimage_jpeg_to_Ximage(uinounours->background_display, uinounours->screen_number, filename);
	return uiimage;
}

static XImage * nnuiimage_jpeg_to_Ximage(Display *display, int screen_number,
		const char *filename) {
	// Initialize the jpeg structs
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	FILE * file = fopen(filename, "rb");

	// Read the jpeg header info
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	// Allocate an array to read all the bytes for a single row
	// in the image.
	// cinfo.output_components is 3 for color, or 1 for grayscale.
	JDIMENSION numrows = 1;
	JDIMENSION samplesperrow = cinfo.output_width * cinfo.output_components;

	JSAMPARRAY scanlines = cinfo.mem->alloc_sarray((j_common_ptr) & cinfo,
			JPOOL_IMAGE, samplesperrow, numrows);

	// Figure out how many bytes each pixel will have, based
	// on the screen depth (max bits per pixel the display
	// can support).
	// For an Ximage ZPixmap, we must use 32 bits, 16 bits, or 8 bits.
	int depth = DefaultDepth(display, screen_number);
	int image_bits_per_pixel;

	// The device supports at least 17 bits (3 bytes minimulm required) per
	// pixel.  We will use 32 bits (4 bytes).
	if (depth > 16)
		image_bits_per_pixel = 32;
	// The device supports between 9 and 16 bits (2 bytes
	// required).  We will use 16 bits (2 bytes).
	else if (depth > 9)
		image_bits_per_pixel = 16;
	// The device supports 8 or fewer bits. We will use 8 bits (1 byte)
	else
		image_bits_per_pixel = 8;
	int image_bytes_per_pixel = image_bits_per_pixel / 8;

	// Allocate the data for the Ximage, based on the image size
	// and the number bytes per pixel we will use.
	size_t image_size = image_bytes_per_pixel * cinfo.output_width
			* cinfo.output_height;
	char *data = (char*) malloc(image_size);
	Visual *visual = DefaultVisual(display, screen_number);
	int max_byte_value = 255;
	int i;
	int ptr = 0;
	for (i = 0; i < cinfo.output_height; i++) {
		// Read one row of the picture.  For a color picture (we are not
		// supporting grayscale now), this will have the width * 3 (for RGB).
		jpeg_read_scanlines((j_decompress_ptr) & cinfo, scanlines, numrows);
		int j;
		// Iterate for each pixel in this row:
		for (j = 0; j < cinfo.output_width; j++) {
			// Extract the red, green, and blue bytes.
			// "Shift" them by multiplying them by the mask.
			u_int32_t red = scanlines[0][3 * j] * visual->red_mask
					/ max_byte_value;
			u_int32_t green = scanlines[0][3 * j + 1] * visual->green_mask
					/ max_byte_value;
			u_int32_t blue = scanlines[0][3 * j + 2] * visual->blue_mask
					/ max_byte_value;
			// If we don't & the color components with their masks,
			// the value of the red pixel will be mixed with the blue
			// and/or green components.
			// We don't see this problem with 24-bit depth, but we
			// do see it with 16-bit depth.
			// For 16 bit depth, the number of bits for RGB is an uneven 5,6,5,
			// and just multiplying by the mask is not equivalent to
			// shifting.
			red &= visual->red_mask;
			green &= visual->green_mask;
			blue &= visual->blue_mask;
			u_int32_t pixel32 = red | green | blue;
			// Copy this converted pixel to the image data.
			if (depth >= 24)
				memcpy(&data[ptr], &pixel32, image_bytes_per_pixel);
			else if (depth >= 16) {
				u_int16_t pixel16 = pixel32;
				memcpy(&data[ptr], &pixel16, image_bytes_per_pixel);
			} else {
				u_int8_t pixel8 = pixel32;
				memcpy(&data[ptr], &pixel8, image_bytes_per_pixel);
			}
			ptr += image_bytes_per_pixel;
		}
	}

	// Create the image we'll return
	XImage *image = XCreateImage(display, visual, depth, ZPixmap, /*format*/
	0, /*offset*/
	data, cinfo.output_width, cinfo.output_height, image_bits_per_pixel, /*bitmap_pad*/
	0/*bytes_per_line*/);

	// Cleanup the jpeg data.
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(file);
	return image;
}

void nnuiimage_show(NNUINounours *uinounours, NNUIImage *image) {
	//int depth = DefaultDepth(uinounours->display, uinounours->screen_number);
	//Pixmap pixmap = XCreatePixmap(uinounours->display, uinounours->window, image->ximage->width, image->ximage->height, depth);
	XPutImage(uinounours->ui_display, uinounours->window, uinounours->gc, image->ximage, 0, 0, 0, 0, image->ximage->width, image->ximage->height);
	//XPutImage(uinounours->display, pixmap, uinounours->gc, image->ximage, 0, 0, 0, 0, image->ximage->width, image->ximage->height);
	//XCopyArea(uinounours->display, pixmap, uinounours->window, uinounours->gc, 0, 0, image->ximage->width, image->ximage->height, 0, 0);
	XFlush(uinounours->ui_display);
}

void nnuiimage_free(NNUIImage *uiimage) {
	XDestroyImage(uiimage->ximage);
	free(uiimage);
}
