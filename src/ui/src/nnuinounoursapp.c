/*
 *  Created on: Mar 11, 2012
 *      Author: Carmen Alvarez
 */
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include "nnuinounoursapp.h"

static int nnuinounoursapp_error_handler(Display *display,
		XErrorEvent *error_event) {
	char error_message[128];
	XGetErrorText(display, error_event->error_code, error_message, 128);
	syslog(
			LOG_ERR,
			"Error on display %p: type=%d, resourceid=%lx, serial=%lu, error_code=%d (%s), request_code=%d, minor_code=%d\n",
			display, error_event->type, error_event->resourceid,
			error_event->serial, error_event->error_code, error_message,
			error_event->request_code, error_event->minor_code);
}

NNUINounoursApp *nnuinounoursapp_new(struct NNNounoursApp *app, int window_id) {
	NNUINounoursApp *uiapp = malloc(sizeof(NNUINounoursApp));
	uiapp->app = app;
	// Make sure we can get the display.
	XSetErrorHandler(nnuinounoursapp_error_handler);
	uiapp->background_display = XOpenDisplay(0);
	if (uiapp->background_display == 0) {
		fprintf(stderr, "Cannot open display\n");
		exit(-1);
	}
	uiapp->window = window_id;
	uiapp->is_running = NNFALSE;
	uiapp->window_width = 0;
	uiapp->window_height = 0;
	uiapp->last_window_x = -1;
	uiapp->last_window_y = -1;
	uiapp->last_window_move_time_us = 0;
	pthread_cond_init(&uiapp->cond, 0);
	pthread_mutex_init(&uiapp->mutex, 0);
	uiapp->atom_my_window = XInternAtom(uiapp->background_display,
			"NN_ATOM_MY_WINDOW", False);
	uiapp->atom_set_image = XInternAtom(uiapp->background_display,
			"NN_ATOM_SET_IMAGE", False);
	return uiapp;
}

static NNNounours * nnuinounoursapp_find_nounours(NNUINounoursApp *uiapp,
		int window_x, int window_y) {
	NNNounoursGrid *grid = uiapp->app->grid;
	int offset_x, offset_y, image_width, image_height;
	nnwindow_get_dimensions(uiapp, &offset_x, &offset_y, &image_width,
			&image_height);
	if (window_x < offset_x || window_y < offset_y)
		return 0;
	int index_x = (window_x - offset_x) / image_width;
	int index_y = (window_y - offset_y) / image_height;
	if (index_x < 0 || index_y < 0 || index_x >= grid->width
			|| index_y >= grid->height) {
		return 0;
	}
	NNNounours *nounours = uiapp->app->grid->nounoursen[index_x][index_y];
	return nounours;
}

static nnbool nnuinounoursapp_is_my_nounours(NNUINounoursApp *uiapp,
		NNNounours *nounours) {
	int i, j;
	NNNounoursGrid *grid = uiapp->app->grid;
	for (i = 0; i < grid->width; i++) {
		for (j = 0; j < grid->height; j++) {
			if (nounours == grid->nounoursen[i][j])
				return NNTRUE;
		}
	}
	return NNFALSE;
}

static void nnuinounoursapp_pointer_event(NNUINounoursApp *uiapp, int type,
		int window_x, int window_y) {
	NNNounours *nounours = nnuinounoursapp_find_nounours(uiapp, window_x,
			window_y);
	if (nounours == 0)
		return;
	int local_x, local_y;
	nnuinounours_translate(nounours->uinounours, window_x, window_y, &local_x,
			&local_y);
	if (type == ButtonPress) {
		nnnounours_on_press(nounours, local_x, local_y);
	} else if (type == MotionNotify) {
		nnnounours_on_move(nounours, local_x, local_y);
	} else if (type == ButtonRelease) {
		nnnounours_on_release(nounours, local_x, local_y);
	}
}
static void *nnuinounoursapp_loop(void *data) {
	NNUINounoursApp *uiapp = (NNUINounoursApp*) data;

	nnwindow_setup(uiapp);

	long event_mask = StructureNotifyMask;
	XEvent xevent;
	if (!uiapp->app->config.is_in_screensaver_mode) {
		// wait for the notify event.
		XSelectInput(uiapp->ui_display, uiapp->window, event_mask);
		do {
			XNextEvent(uiapp->ui_display, &xevent);
		} while (xevent.type != MapNotify);
	}
	uiapp->gc = XCreateGC(uiapp->ui_display, uiapp->window, 0, NULL);

	event_mask = ExposureMask | StructureNotifyMask;
	if (!uiapp->app->config.is_in_screensaver_mode)
		event_mask |= ButtonPressMask | ButtonReleaseMask | ButtonMotionMask;
	XSelectInput(uiapp->ui_display, uiapp->window, event_mask);
	uiapp->is_running = 1;
	pthread_cond_signal(&uiapp->cond);
	while (uiapp->is_running) {
		XNextEvent(uiapp->ui_display, &xevent);
		if (xevent.type == ClientMessage) {
			XClientMessageEvent client_message_event = xevent.xclient;
			NNUIImage *uiimage;
			NNNounours *event_nounours;
			nnclientmessage_read(&client_message_event, &event_nounours,
					&uiimage);
			nnbool is_my_nounours = nnuinounoursapp_is_my_nounours(uiapp,
					event_nounours);
			if (client_message_event.message_type == uiapp->atom_set_image) {
				if (!is_my_nounours) {
					//Too verbose, but why are we still receiving
					// events from the other nounours after changing the
					// select input? syslog(LOG_DEBUG, "Ignoring event from another nounours");
				} else {
					nnuiimage_show(event_nounours->uinounours, uiimage);
				}
			} else if (client_message_event.message_type
					== uiapp->atom_my_window) {
				if (!is_my_nounours) {
					Window window_of_other_nounours =
							(Window) client_message_event.data.l[0];
					if (window_of_other_nounours == uiapp->window) {
						syslog(LOG_DEBUG, "other nounours is using my window!");
						nnwindow_switch(uiapp);
						XSelectInput(uiapp->ui_display, uiapp->window,
								event_mask);
					}
				}
			}
		} else if (xevent.type == Expose) {
			syslog(LOG_DEBUG, "expose");
			NNNounoursGrid *grid = uiapp->app->grid;
			int i, j;
			for (i = 0; i < grid->width; i++) {
				for (j = 0; j < grid->height; j++) {
					NNUINounours *uinounours =
							grid->nounoursen[i][j]->uinounours;
					if (uinounours->nounours->state.cur_image != 0)
						nnuiimage_show(uinounours,
								uinounours->nounours->state.cur_image->uiimage);
				}
			}
		} else if (xevent.type == ButtonPress || xevent.type == ButtonRelease) {
			nnuinounoursapp_pointer_event(uiapp, xevent.type, xevent.xbutton.x,
					xevent.xbutton.y);
		} else if (xevent.type == MotionNotify) {
			nnuinounoursapp_pointer_event(uiapp, xevent.type, xevent.xmotion.x,
					xevent.xmotion.y);
		} else if (xevent.type == ConfigureNotify) {
			XConfigureEvent conf_event = xevent.xconfigure;
			struct timeval now_tv;
			gettimeofday(&now_tv, NULL);
			long now_us = now_tv.tv_sec * 1000000 + now_tv.tv_usec;
			if (uiapp->last_window_x >= 0) {
				long time_diff = now_us - uiapp->last_window_move_time_us;
				if (time_diff == 0)
					time_diff = 1;
				long distance = nnmath_get_distance(conf_event.x, conf_event.y,
						uiapp->last_window_x, uiapp->last_window_y);
				long speed = (distance * 1000000) / time_diff;
				if (speed > uiapp->app->config.shake_factor)
					nnnounoursgrid_on_shake(uiapp->app->grid);
			}
			uiapp->last_window_move_time_us = now_us;
			uiapp->last_window_x = conf_event.x;
			uiapp->last_window_y = conf_event.y;
		}

	}
	return (void*) 0;
}

void nnuinounoursapp_start_loop(NNUINounoursApp *uiapp) {
	pthread_create(&uiapp->ui_thread, NULL, nnuinounoursapp_loop, uiapp);
	while (!uiapp->is_running)
		pthread_cond_wait(&uiapp->cond, &uiapp->mutex);
}

void nnuinounoursapp_stop_loop(NNUINounoursApp *uiapp) {
	uiapp->is_running = 0;
	pthread_join(uiapp->ui_thread, NULL);
}

void nnuinounoursapp_free(NNUINounoursApp *uiapp) {
	XCloseDisplay(uiapp->background_display);
	XCloseDisplay(uiapp->ui_display);
	free(uiapp);
}
