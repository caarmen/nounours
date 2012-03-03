/*
 * nnuinounours.c
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>
#include <pthread.h>
#include <syslog.h>
#include <stdio.h>
#include "nnuinounours.h"

NNUINounours *nnuinounours_new(NNNounours *nounours, int window_id) {
	NNUINounours *uinounours = malloc(sizeof(NNUINounours));
	uinounours->background_display = XOpenDisplay(0);
	uinounours->window = window_id;
	uinounours->nounours = nounours;
	uinounours->is_running = 0;
	XSetErrorHandler(nnuinounours_error_handler);
	uinounours->last_window_x = -1;
	uinounours->last_window_y = -1;
	uinounours->last_window_move_time_us = 0;
	pthread_cond_init(&uinounours->cond,0);
	pthread_mutex_init(&uinounours->mutex,0);
	return uinounours;
}
void nnuinounours_resize(NNUINounours *uinounours, int width, int height) {
	int screen_width =
			DisplayWidth(uinounours->ui_display, uinounours->screen_number);
	int screen_height =
			DisplayHeight(uinounours->ui_display, uinounours->screen_number);

	XMoveResizeWindow(uinounours->background_display, uinounours->window,
			(screen_width - width) / 2, (screen_height - height) / 2, width,
			height);
	XSizeHints* size_hints = XAllocSizeHints();
	size_hints->flags = PMinSize | PMaxSize;
	size_hints->min_width = width;
	size_hints->min_height = height;
	size_hints->max_width = width;
	size_hints->max_height = height;
	XSetWMNormalHints(uinounours->background_display, uinounours->window,
			size_hints);
	XFree(size_hints);
}

void nnuinounours_free(NNUINounours *uinounours) {
	XCloseDisplay(uinounours->background_display);
	XCloseDisplay(uinounours->ui_display);
	free(uinounours);
}
void nnuinounours_show_image(NNUINounours *uinounours, NNUIImage *uiimage) {
	pthread_t mythread = pthread_self();
	if (mythread == uinounours->ui_thread) {
		nnuiimage_show(uinounours, uiimage);
	} else {
		XClientMessageEvent notify_message_event;
		memset(&notify_message_event, 0, sizeof(XClientMessageEvent));
		notify_message_event.type = ClientMessage;
		notify_message_event.window = uinounours->window;
		notify_message_event.format = 8; // doesn't really matter since we use memcpy to pass the pointer
		memcpy(notify_message_event.data.l, &uiimage, sizeof(uiimage));
		long event_mask = NoEventMask;
		if(uinounours->nounours->screensaver_mode)
			event_mask = ButtonReleaseMask; // TODO other applications may receive this event!
		XSendEvent(uinounours->background_display, uinounours->window, 0, event_mask,
				(XEvent*) &notify_message_event);
		XFlush(uinounours->background_display);
	}
}

static void *nnuinounours_loop(void *data) {
	NNUINounours *uinounours = (NNUINounours*) data;

	uinounours->ui_display = XOpenDisplay(0);
	if (uinounours->ui_display == 0) {
		syslog(LOG_ERR, "Could not open display");
		exit(-1);
	}
	uinounours->screen_number = DefaultScreen(uinounours->ui_display);
	uinounours->root_window = DefaultRootWindow(uinounours->ui_display);
	int black_color =
			BlackPixel(uinounours->ui_display, uinounours->screen_number);
	if (uinounours->window == -1) {
		if (uinounours->nounours->screensaver_mode) {
			uinounours->window = uinounours->root_window;
		} else {
			uinounours->window = XCreateSimpleWindow(uinounours->ui_display,
					uinounours->root_window, 0, 0, 1, 1, 0, black_color,
					black_color);
			XMapWindow(uinounours->ui_display, uinounours->window);
		}
	}

	long event_mask = StructureNotifyMask;
	XEvent xevent;
	if (!uinounours->nounours->screensaver_mode) {
		// wait for the notify event.
		XSelectInput(uinounours->ui_display, uinounours->window, event_mask);
		do {
			XNextEvent(uinounours->ui_display, &xevent);
		} while (xevent.type != MapNotify);
	}
	uinounours->gc = XCreateGC(uinounours->ui_display, uinounours->window, 0,
			NULL);

	event_mask = ExposureMask | ButtonPressMask
			| ButtonReleaseMask | ButtonMotionMask | StructureNotifyMask;
	XSelectInput(uinounours->ui_display, uinounours->window, event_mask);
	uinounours->is_running = 1;
	pthread_cond_signal(&uinounours->cond);
	while (uinounours->is_running) {
		XNextEvent(uinounours->ui_display, &xevent);
		if (xevent.type == ClientMessage) {
			XClientMessageEvent client_message_event = xevent.xclient;
			NNUIImage *uiimage;
			memcpy(&uiimage, client_message_event.data.l, sizeof(NNUIImage*));
			nnuiimage_show(uinounours, uiimage);
		} else if (xevent.type == Expose) {
			syslog(LOG_DEBUG, "expose");
			if (uinounours->nounours->cur_image != 0)
				nnuiimage_show(uinounours,
						uinounours->nounours->cur_image->uiimage);
		} else if (xevent.type == ButtonPress) {
			XButtonPressedEvent bp_event = xevent.xbutton;
			nnnounours_on_press(uinounours->nounours, bp_event.x, bp_event.y);

		} else if (xevent.type == MotionNotify) {
			XMotionEvent motion_event = xevent.xmotion;
			nnnounours_on_move(uinounours->nounours, motion_event.x,
					motion_event.y);
		} else if (xevent.type == ButtonRelease) {
			XButtonReleasedEvent bp_event = xevent.xbutton;
			nnnounours_on_release(uinounours->nounours, bp_event.x, bp_event.y);
		} else if (xevent.type == ConfigureNotify) {
			XConfigureEvent conf_event = xevent.xconfigure;
			struct timeval now_tv;
			gettimeofday(&now_tv, NULL);
			long now_us = now_tv.tv_sec * 1000000 + now_tv.tv_usec;
			if (uinounours->last_window_x >= 0) {
				long time_diff = now_us - uinounours->last_window_move_time_us;
				long distance = nnmath_get_distance(conf_event.x, conf_event.y,
						uinounours->last_window_x, uinounours->last_window_y);
				long speed = (distance * 1000000) / time_diff;
				if (speed > uinounours->nounours->shake_factor)
					nnnounours_on_shake(uinounours->nounours);
			}
			uinounours->last_window_move_time_us = now_us;
			uinounours->last_window_x = conf_event.x;
			uinounours->last_window_y = conf_event.y;
		}

	}
	return (void*) 0;
}

void nnuinounours_start_loop(NNUINounours *uinounours) {
	pthread_create(&uinounours->ui_thread, NULL, nnuinounours_loop, uinounours);
	while (!uinounours->is_running)
		pthread_cond_wait(&uinounours->cond, &uinounours->mutex);
}

void nnuinounours_stop_loop(NNUINounours *uinounours) {
	uinounours->is_running = 0;
	pthread_join(uinounours->ui_thread, NULL);
}

int nnuinounours_error_handler(Display *display, XErrorEvent *error_event) {
	char error_message[128];
	XGetErrorText(display, error_event->error_code, error_message, 128);
	syslog(
			LOG_ERR,
			"Error on display %p: type=%d, resourceid=%lu, serial=%lu, error_code=%d (%s), request_code=%d, minor_code=%d\n",
			display, error_event->type, error_event->resourceid,
			error_event->serial, error_event->error_code, error_message,
			error_event->request_code, error_event->minor_code);
}
