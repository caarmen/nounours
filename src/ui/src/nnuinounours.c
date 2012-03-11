/*
 *  Created on: Feb 20, 2012
 *      Author: Carmen Alvarez
 */
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <string.h>
#include <pthread.h>
#include <syslog.h>
#include <stdio.h>
#include "nnuinounours.h"
#include "nntheme.h"
#include "nncommon.h"
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
	nnmath_translate(window_x, window_y, uinounours->window_width,
			uinounours->window_height, theme->width, theme->height, image_x,
			image_y);
	*image_x = *image_x + uinounours->window_x;
	*image_y = *image_y + uinounours->window_y;
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
		nnuinounoursapp_init_client_message_event(&notify_message_event,
				uinounours->uiapp, uinounours->uiapp->atom_set_image, 8);
		nnuinounoursapp_write_client_event_data(&notify_message_event,
				uinounours->nounours, uiimage);
		nnuinounoursapp_send_client_message_event(&notify_message_event,
				uinounours->uiapp);
	}
}

