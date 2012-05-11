/*
 * Copyright 2012 Carmen Alvarez
 *
 * This file is part of Nounours.
 *
 * Nounours is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Nounours is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nounours.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <string.h>
#include "nnuiimage.h"
#include "nnio.h"

static XImage * nnuiimage_jpeg_to_Ximage(Display *display, int window,
		const char *filename);
static XImage * scale_image(Display *display, Visual *visual, XImage *in_image,
		int dest_width, int dest_height);

NNUIImage *nnuiimage_new(NNUINounoursApp *uiapp, const char *filename) {
	NNUIImage *uiimage = malloc(sizeof(NNUIImage));
	uiimage->ximage = nnuiimage_jpeg_to_Ximage(uiapp->background_display,
			uiapp->window, filename);
	return uiimage;
}

static XImage * scale_image(Display *display, Visual *visual, XImage *in_image,
		int dest_width, int dest_height) {
	float scale_x = (float) dest_width / in_image->width;
	float scale_y = (float) dest_height / in_image->height;
	// Allocate the memory for the dest image data.
	size_t image_size = in_image->bitmap_pad * dest_width * dest_height;
	char *data = (char*) malloc(image_size);

	// Create the image we'll return
	XImage *out_image = XCreateImage(display, visual, in_image->depth, ZPixmap, /*format*/
	0, /*offset*/
	data, dest_width, dest_height, in_image->bitmap_pad, 0/*bytes_per_line*/);

	// Using the simple nearest-neighbor algorithm.  Not pretty, but easy to code :)
	int x_dest;
	for (x_dest = 0; x_dest < dest_width; x_dest++) {
		int y_dest;
		for (y_dest = 0; y_dest < dest_height; y_dest++) {
			int x_source = x_dest / scale_x;
			int y_source = y_dest / scale_y;
			unsigned long pixel = XGetPixel(in_image, x_source, y_source);
			XPutPixel(out_image, x_dest, y_dest, pixel);
		}
	}
	return out_image;
}
static XImage * nnuiimage_jpeg_to_Ximage(Display *display, int window,
		const char *filename) {
	// Refer to libjpeg.txt file distributed with jpeglib, which explains
	// how to read in jpeg header and data from a file.
	// Initialize the jpeg structs
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	FILE * file = nnio_open_file(0, filename, "rb");

	// Read the jpeg header info
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	// Allocate an array to read all the bytes for a single row
	// in the image.
	// cinfo.output_components is 3 for color, or 1 for grayscale.
	JDIMENSION numrows = 1;
	JDIMENSION samplesperrow = cinfo.output_width * cinfo.output_components;

	JSAMPARRAY scanlines = cinfo.mem->alloc_sarray((j_common_ptr) &cinfo,
			JPOOL_IMAGE, samplesperrow, numrows);

	// Figure out how many bytes each pixel will have, based
	// on the screen depth (max bits per pixel the display
	// can support).
	// For an Ximage ZPixmap, we must use 32 bits, 16 bits, or 8 bits.
	XWindowAttributes window_attributes;
	XGetWindowAttributes(display, window, &window_attributes);
	int depth = window_attributes.depth;
	int image_bits_per_pixel;

	// The device supports at least 17 bits (3 bytes minimum required) per
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
	Visual *visual = window_attributes.visual;
	int max_byte_value = 0xFF;
	int i;
	int ptr = 0;
	for (i = 0; i < cinfo.output_height; i++) {
		// Read one row of the picture.  For a color picture (we are not
		// supporting grayscale now), this will have the width * 3 (for RGB).
		jpeg_read_scanlines((j_decompress_ptr) &cinfo, scanlines, numrows);
		int j;
		// Iterate for each pixel in this row:
		for (j = 0; j < cinfo.output_width; j++) {
			// Extract the red, green, and blue bytes.
			// "Shift" them by multiplying them by the mask.
			// Thanks to George Peter Staplin and his tutorial for pointing this out:
			// http://user.xmission.com/~georgeps/documentation/tutorials/Xlib_JPEG_Tutorial-5.html
			u_int32_t red = scanlines[0][3 * j] * visual->red_mask
					/ max_byte_value;
			u_int32_t green = scanlines[0][3 * j + 1] * visual->green_mask
					/ max_byte_value;
			u_int32_t blue = scanlines[0][3 * j + 2] * visual->blue_mask
					/ max_byte_value;
			// If we don't & the color components with their masks,
			// the value of the red pixel will be mixed with the blue
			// and/or green components.
			// From  my understanding, this is actually not a floating point issue as indicated by
			// Mr Staplin, since in my implementation we are not using any floats. This is
			// actually a problem of the uneven bit-depths for RGB with 16-bit depth.
			//
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
void nnuiimage_resize(struct NNUINounoursApp *uiapp, NNUIImage *uiimage,
		int dest_width, int dest_height) {
	if (uiimage->ximage->width == dest_width
			&& uiimage->ximage->height == dest_height)
		return;

	XWindowAttributes window_attributes;
	XGetWindowAttributes(uiapp->background_display, uiapp->window,
			&window_attributes);
	XImage *scaled_image = scale_image(uiapp->background_display,
			window_attributes.visual, uiimage->ximage, dest_width, dest_height);
	XDestroyImage(uiimage->ximage);
	uiimage->ximage = scaled_image;
}

void nnuiimage_show(NNUINounours *uinounours, NNUIImage *uiimage) {

	int dest_x = uinounours->window_x;
	int dest_y = uinounours->window_y;
	XPutImage(uinounours->uiapp->ui_display, uinounours->uiapp->window,
			uinounours->uiapp->gc, uiimage->ximage, 0, 0, dest_x, dest_y,
			uiimage->ximage->width, uiimage->ximage->height);
	XFlush(uinounours->uiapp->ui_display);
}

void nnuiimage_free(NNUIImage *uiimage) {
	XDestroyImage(uiimage->ximage);
	free(uiimage);
}
