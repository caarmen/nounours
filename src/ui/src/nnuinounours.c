/*
 * nnuinounours.c
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */
#include <stdlib.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <string.h>
#include <pthread.h>
#include "nnuinounours.h"
#include <stdio.h>

pthread_t nnuinounours_thread;
int nnuinounours_running;

NNUINounours *nnuinounours_new(NNNounours *nounours) {
	NNUINounours *uinounours = malloc(sizeof(NNUINounours));
	uinounours->display = XOpenDisplay(0);
	uinounours->screen_number = DefaultScreen(uinounours->display);
	uinounours->root_window = DefaultRootWindow(uinounours->display);
	int black_color = BlackPixel(uinounours->display, uinounours->screen_number);
	uinounours->window = XCreateSimpleWindow(uinounours->display,
			uinounours->root_window, 0, 0, 50, 50, 0, black_color, black_color);
	XMapWindow(uinounours->display, uinounours->window);
	uinounours->gc = XCreateGC(uinounours->display, uinounours->window, 0,
			NULL);
	uinounours->nounours = nounours;
	return uinounours;
}
void nnuinounours_resize(NNUINounours *uinounours, int width, int height) {
	XMoveResizeWindow(uinounours->display, uinounours->window, 0, 0, width,
			height);
}

void nnuinounours_free(NNUINounours *uinounours) {
	free(uinounours);
}
void nnuinounours_notify(NNUINounours *uinounours, NNUIImage *image) {
	Display *display = XOpenDisplay(0);
	XClientMessageEvent notify_message_event;
	memset(&notify_message_event, 0, sizeof(XClientMessageEvent));
	notify_message_event.type = ClientMessage;
	notify_message_event.window = uinounours->window;
	notify_message_event.format = 8;// doesn't really matter since we use memcpy to pass the pointer
	memcpy(notify_message_event.data.l, &image, sizeof(image));
	XSendEvent(display, uinounours->window, 0, 0, (XEvent*)&notify_message_event);
	XFlush(display);
}

static void *nnuinounours_loop(void *data) {
	NNUINounours *uinounours = (NNUINounours*) data;
	XEvent xevent;
	long event_mask = ExposureMask | ClientMessage;
	XSelectInput(uinounours->display, uinounours->window, event_mask);
	nnuinounours_running = 1;
	while (nnuinounours_running) {
		XNextEvent(uinounours->display, &xevent);
		printf("got event %d\n", xevent.type);
		if(xevent.type == ClientMessage) {
			XClientMessageEvent client_message_event = xevent.xclient;
			NNUIImage *uiimage;
			memcpy(&uiimage, client_message_event.data.l, sizeof(NNUIImage*));
			nnuiimage_show(uinounours, uiimage);
		} else if (xevent.type == Expose) {
			printf("expose\n");
			nnuiimage_show(uinounours, uinounours->nounours->cur_image->uiimage);

		}
	}
	return (void*) 0;
}

void nnuinounours_start_loop(NNUINounours *uinounours) {
	pthread_create(&nnuinounours_thread, NULL, nnuinounours_loop, uinounours);
}

void nnuinounours_stop_loop() {
	nnuinounours_running = 0;
	pthread_join(nnuinounours_thread, NULL);
}

