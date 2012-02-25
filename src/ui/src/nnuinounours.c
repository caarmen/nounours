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
#include <stdio.h>
#include "nnuinounours.h"

NNUINounours *nnuinounours_new(NNNounours *nounours) {
	NNUINounours *uinounours = malloc(sizeof(NNUINounours));
	uinounours->background_display = XOpenDisplay(0);
	uinounours->nounours = nounours;
	uinounours->is_running = 0;
	XSetErrorHandler(nnuinounours_error_handler);
	return uinounours;
}
void nnuinounours_resize(NNUINounours *uinounours, int width, int height) {
	XMoveResizeWindow(uinounours->background_display, uinounours->window, 0, 0,
			width, height);
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
void nnuinounours_notify(NNUINounours *uinounours, NNUIImage *image) {
	XClientMessageEvent notify_message_event;
	memset(&notify_message_event, 0, sizeof(XClientMessageEvent));
	notify_message_event.type = ClientMessage;
	notify_message_event.window = uinounours->window;
	notify_message_event.format = 8; // doesn't really matter since we use memcpy to pass the pointer
	memcpy(notify_message_event.data.l, &image, sizeof(image));
	XSendEvent(uinounours->background_display, uinounours->window, 0, 0,
			(XEvent*) &notify_message_event);
	XFlush(uinounours->background_display);
}

static void *nnuinounours_loop(void *data) {
	NNUINounours *uinounours = (NNUINounours*) data;
	uinounours->ui_display = XOpenDisplay(0);
	uinounours->screen_number = DefaultScreen(uinounours->ui_display);
	uinounours->root_window = DefaultRootWindow(uinounours->ui_display);
	int black_color =
			BlackPixel(uinounours->ui_display, uinounours->screen_number);
	uinounours->window = XCreateSimpleWindow(uinounours->ui_display,
			uinounours->root_window, 0, 0, 50, 50, 0, black_color, black_color);
	XMapWindow(uinounours->ui_display, uinounours->window);

	// wait for the notify event.
	long event_mask = StructureNotifyMask;
	XSelectInput(uinounours->ui_display, uinounours->window, event_mask);
	XEvent event;
	do {
		XNextEvent(uinounours->ui_display, &event);
	} while (event.type != MapNotify);
	uinounours->gc = XCreateGC(uinounours->ui_display, uinounours->window, 0,
			NULL);


	XEvent xevent;
	event_mask = ExposureMask | ClientMessage;
	XSelectInput(uinounours->ui_display, uinounours->window, event_mask);
	uinounours->is_running = 1;
	pthread_cond_signal(&uinounours->cond);
	while (uinounours->is_running) {
		XNextEvent(uinounours->ui_display, &xevent);
		printf("got event %d\n", xevent.type);
		if (xevent.type == ClientMessage) {
			XClientMessageEvent client_message_event = xevent.xclient;
			NNUIImage *uiimage;
			memcpy(&uiimage, client_message_event.data.l, sizeof(NNUIImage*));
			nnuiimage_show(uinounours, uiimage);
		} else if (xevent.type == Expose) {
			printf("expose\n");
			if (uinounours->nounours->cur_image != 0)
				nnuiimage_show(uinounours,
						uinounours->nounours->cur_image->uiimage);
		}
	}
	return (void*) 0;
}

void nnuinounours_start_loop(NNUINounours *uinounours) {
	pthread_create(&uinounours->ui_thread, NULL, nnuinounours_loop, uinounours);
	while(!uinounours->is_running)
		pthread_cond_wait(&uinounours->cond, &uinounours->mutex);
}

void nnuinounours_stop_loop(NNUINounours *uinounours) {
	uinounours->is_running = 0;
	pthread_join(uinounours->ui_thread, NULL);
}

int nnuinounours_error_handler(Display *display, XErrorEvent *error_event) {
	fprintf(
			stderr,
			"Error on display %p: type=%d, resourceid=%lu, serial=%lu, error_code=%d, request_code=%d, minor_code=%d\n",
			display, error_event->type, error_event->resourceid, error_event->serial,
			error_event->error_code, error_event->request_code,
			error_event->minor_code);
}
