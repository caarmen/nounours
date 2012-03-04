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
#include <X11/Xatom.h>
#include <string.h>
#include <pthread.h>
#include <syslog.h>
#include <stdio.h>
#include "nnuinounours.h"
#include "nntheme.h"

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
	pthread_cond_init(&uinounours->cond, 0);
	pthread_mutex_init(&uinounours->mutex, 0);
	return uinounours;
}

/**
 * Search the child windows of the given window for the given property.
 * Return the value of the property from the first child window which has a value.
 */
static unsigned char *nnuinounours_find_first_property_in_child_windows(
		Display *display, Window window, const char *property_name) {
	// Some variables which are filled in by X methods, which we do not need, but are required to pass to these methods.
	Window window_not_used;
	int int_not_used;
	unsigned long ulong_not_used;
	Atom atom_not_used;

	// Get all the child windows of the given window.
	Window *children_return;
	unsigned int nchildren_return;
	Status status = XQueryTree(display, window, &window_not_used,
			&window_not_used, &children_return, &nchildren_return);
	if (nchildren_return == 0)
		return 0;

	// Create an atom for the property to find.
	Atom property = XInternAtom(display, property_name, False);
	unsigned char *prop_return = 0;

	int i;
	for (i = 0; i < nchildren_return; i++) {
		// See if this child window has this property.
		int result = XGetWindowProperty(display, children_return[i], property,
				0, 1, False, XA_WINDOW, &atom_not_used, &int_not_used,
				&ulong_not_used, &ulong_not_used, &prop_return);
		// We found a non-null value for this property.  Return it.
		if (result == Success && prop_return != 0)
			break;
	}
	XFree(children_return);
	return prop_return;
}

/**
 * Find or create a window in which nounours will run.
 */
static void nnuinounours_setup_window(NNUINounours *uinounours) {

	// We need to be able to open the display.
	uinounours->ui_display = XOpenDisplay(0);
	if (uinounours->ui_display == 0) {
		syslog(LOG_ERR, "Could not open display");
		exit(-1);
	}
	uinounours->screen_number = DefaultScreen(uinounours->ui_display);
	uinounours->root_window = DefaultRootWindow(uinounours->ui_display);

	// Find or create the window if we weren't given a specific window
	// on the command line.
	if (uinounours->window == 0) {
		// In screensaver mode, we need to draw to the "root" window.
		if (uinounours->nounours->screensaver_mode) {
			// If we want to draw directly to the root window (the case of
			// the screensaver mode), we need to
			// make sure we use the right window.
			// http://developer.gnome.org/wm-spec/#id2550868
			// The right window could be a child of the root window which
			// has a property "__SWM_VROOT" or "_NET_VIRTUAL_ROOTS".  During
			// tests only __SWM_VROOT was found.
			unsigned char *vroot_char =
					nnuinounours_find_first_property_in_child_windows(
							uinounours->ui_display, uinounours->root_window,
							"__SWM_VROOT");
			if (vroot_char != 0)
				uinounours->window = (Window) ((unsigned long*) vroot_char)[0];
			else
				uinounours->window = uinounours->root_window;
		}
		// We're not in screensaver mode.  Let's create a window.
		else {
			int black_color =
					BlackPixel(uinounours->ui_display, uinounours->screen_number);
			uinounours->window = XCreateSimpleWindow(uinounours->ui_display,
					uinounours->root_window, 0, 0, 1, 1, 0, black_color,
					black_color);
			XMapWindow(uinounours->ui_display, uinounours->window);
		}
	}

}
void nnuinounours_stretch(NNUINounours *uinounours) {
	int screen_width =
			DisplayWidth(uinounours->background_display, uinounours->screen_number);
	int screen_height =
			DisplayHeight(uinounours->background_display, uinounours->screen_number);
	NNTheme *theme = uinounours->nounours->cur_theme;
	float width_ratio = (float) screen_width / theme->width;
	float height_ratio = (float) screen_height / theme->height;
	float ratio_to_use =
			width_ratio > height_ratio ? height_ratio : width_ratio;
	int dest_height = ratio_to_use * theme->height;
	int dest_width = ratio_to_use * theme->width;
	nnuinounours_resize(uinounours, dest_width, dest_height);
}

void nnuinounours_resize(NNUINounours *uinounours, int width, int height) {
	int screen_width =
			DisplayWidth(uinounours->background_display, uinounours->screen_number);
	int screen_height =
			DisplayHeight(uinounours->background_display, uinounours->screen_number);

	XMoveResizeWindow(uinounours->background_display, uinounours->window,
			(screen_width - width) / 2, (screen_height - height) / 2, width,
			height);
	XSizeHints* size_hints = XAllocSizeHints();
	size_hints->flags = PMinSize | PMaxSize;
	size_hints->min_width = width;
	size_hints->min_height = height;
	size_hints->max_width = width;
	size_hints->max_height = height;
	uinounours->window_width = width;
	uinounours->window_height = height;
	XSetWMNormalHints(uinounours->background_display, uinounours->window,
			size_hints);
	XFree(size_hints);
	if (uinounours->nounours->do_stretch) {
		NNTheme *theme = uinounours->nounours->cur_theme;
		int i;
		for (i = 0; i < theme->num_images; i++) {
			nnuiimage_resize(uinounours, theme->images[i]->uiimage, width,
					height);
		}
	}
}

void nnuinounours_translate(NNUINounours *uinounours, int window_x,
		int window_y, int *image_x, int *image_y) {
	NNTheme *theme = uinounours->nounours->cur_theme;
	nnmath_translate(window_x, window_y, uinounours->window_width,
			uinounours->window_height, theme->width, theme->height, image_x,
			image_y);
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
		if (uinounours->nounours->screensaver_mode)
			event_mask = ExposureMask; // TODO other applications may receive this event!
		XSendEvent(uinounours->background_display, uinounours->window, 0,
				event_mask, (XEvent*) &notify_message_event);
		XFlush(uinounours->background_display);
	}
}

static void *nnuinounours_loop(void *data) {
	NNUINounours *uinounours = (NNUINounours*) data;

	nnuinounours_setup_window(uinounours);

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

	event_mask = ExposureMask | StructureNotifyMask;
	if (!uinounours->nounours->screensaver_mode)
		event_mask |= ButtonPressMask | ButtonReleaseMask | ButtonMotionMask;
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
			int x, y;
			nnuinounours_translate(uinounours, bp_event.x, bp_event.y, &x, &y);
			nnnounours_on_press(uinounours->nounours, x, y);
		} else if (xevent.type == MotionNotify) {
			XMotionEvent motion_event = xevent.xmotion;
			int x, y;
			nnuinounours_translate(uinounours, motion_event.x, motion_event.y,
					&x, &y);
			nnnounours_on_move(uinounours->nounours, x, y);
		} else if (xevent.type == ButtonRelease) {
			XButtonReleasedEvent bp_event = xevent.xbutton;
			int x, y;
			nnuinounours_translate(uinounours, bp_event.x, bp_event.y, &x, &y);
			nnnounours_on_release(uinounours->nounours, x, y);
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
			"Error on display %p: type=%d, resourceid=%lx, serial=%lu, error_code=%d (%s), request_code=%d, minor_code=%d\n",
			display, error_event->type, error_event->resourceid,
			error_event->serial, error_event->error_code, error_message,
			error_event->request_code, error_event->minor_code);
}
