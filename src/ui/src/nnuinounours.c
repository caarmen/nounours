/*
 *  Created on: Feb 20, 2012
 *      Author: Carmen Alvarez
 */
#include <stdlib.h>
#include <pthread.h>
#include <syslog.h>
#include "nnuinounours.h"
#include "nnuinounoursapp.h"

NNUINounours *nnuinounours_new(NNUINounoursApp *uiapp, NNNounours *nounours,
		int window_id) {
	NNUINounours *uinounours = malloc(sizeof(NNUINounours));
	uinounours->nounours = nounours;
	uinounours->uiapp = uiapp;
	uinounours->window_x = 0;
	uinounours->window_y = 0;
	return uinounours;
}

void nnuinounours_translate(NNUINounours *uinounours, int window_x,
		int window_y, int *image_x, int *image_y) {
	NNTheme *theme = uinounours->nounours->app->config.theme;
	int offset_x, offset_y, image_width, image_height;
	nnwindow_get_dimensions(uinounours->uiapp, &offset_x, &offset_y,
			&image_width, &image_height);
	int tile_x = (window_x - offset_x) % image_width;
	int tile_y = (window_y - offset_y) % image_height;
	nnmath_translate(tile_x, tile_y, image_width, image_height, theme->width,
			theme->height, image_x, image_y);
}
void nnuinounours_free(NNUINounours *uinounours) {
	free(uinounours);
}

void nnuinounours_show_image(NNUINounours *uinounours, NNUIImage *uiimage) {
	pthread_t mythread = pthread_self();
	if (mythread == uinounours->uiapp->ui_thread) {
		nnuiimage_show(uinounours, uiimage);
	} else {
		XClientMessageEvent notify_message_event;
		nnclientmessage_init(&notify_message_event, uinounours->uiapp,
				uinounours->uiapp->atom_set_image, 8);
		nnclientmessage_write(&notify_message_event, uinounours->nounours,
				uiimage);
		nnclientmessage_send(&notify_message_event, uinounours->uiapp);
	}
}

