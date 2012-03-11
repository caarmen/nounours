/*
 *  Created on: Mar 11, 2012
 *      Author: Carmen Alvarez
 */
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "nnuinounoursapp.h"
#include "nncommon.h"

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
	uiapp->background_display = XOpenDisplay(0);
	uiapp->window = window_id;
	uiapp->is_running = 0;
	XSetErrorHandler(nnuinounoursapp_error_handler);
	uiapp->last_window_x = -1;
	uiapp->last_window_y = -1;
	uiapp->last_window_move_time_us = 0;
	pthread_cond_init(&uiapp->cond, 0);
	pthread_mutex_init(&uiapp->mutex, 0);
	uiapp->atom_my_window = XInternAtom(uiapp->background_display,
			"NN_ATOM_MY_WINDOW", False);
	uiapp->atom_set_image = XInternAtom(uiapp->background_display,
			"NN_ATOM_SET_IMAGE", False);
	uiapp->window_width = 0;
	uiapp->window_height = 0;
	return uiapp;
}

void nnuinounoursapp_init_client_message_event(XClientMessageEvent *event,
		NNUINounoursApp *uiapp, Atom atom, int format) {
	memset(event, 0, sizeof(XClientMessageEvent));
	event->type = ClientMessage;
	event->window = uiapp->window;
	event->format = format; // doesn't really matter since we use memcpy to pass the pointer
	event->message_type = atom;
}
void nnuinounoursapp_send_client_message_event(XClientMessageEvent *event,
		NNUINounoursApp *uiapp) {
	long event_mask = NoEventMask;
	if (uiapp->app->config.is_in_screensaver_mode)
		event_mask = ExposureMask; // TODO other applications may receive this event!
	XSendEvent(uiapp->background_display, uiapp->window, 0, event_mask,
			(XEvent*) event);
	XFlush(uiapp->background_display);

}

void nnuinounoursapp_write_client_event_data(XClientMessageEvent *event,
		NNNounours *nounours, NNUIImage *uiimage) {
	void *ptr = event->data.l;
	memcpy(ptr, &uiimage, sizeof(NNUIImage*));
	ptr += sizeof(NNUIImage*);
	memcpy(ptr, &nounours, sizeof(NNNounours*));
}

void nnuinounoursapp_read_client_event_data(XClientMessageEvent *event,
		NNNounours **nounours, NNUIImage **uiimage) {
	void *ptr = event->data.l;
	memcpy(uiimage, ptr, sizeof(NNUIImage*));
	ptr += sizeof(NNUIImage*);
	memcpy(nounours, ptr, sizeof(NNNounours*));
}

/**
 * Search the child windows of the given window for the given property.
 * @return the child windows having the given property.
 */
static Window * nnuinounoursapp_find_child_windows_for_property(
		Display *display, Window window, const char *property_name,
		int *num_results) {
	*num_results = 0;
	Window *windows = (Window*) malloc(
			NN_INITIAL_LIST_CAPACITY * sizeof(Window));
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
		if (result == Success && prop_return != 0) {
			windows = nnresize_if_needed(windows, *num_results);
			windows[*num_results] = children_return[i];
			*num_results = *num_results + 1;
		}
	}
	XFree(children_return);
	return windows;
}
/**
 * Find or create a window in which nounours will run.
 */
static void nnuinounoursapp_setup_window(NNUINounoursApp *uiapp) {

	// We need to be able to open the display.
	uiapp->ui_display = XOpenDisplay(0);
	if (uiapp->ui_display == 0) {
		syslog(LOG_ERR, "Could not open display");
		exit(-1);
	}
	uiapp->screen_number = DefaultScreen(uiapp->ui_display);
	uiapp->root_window = DefaultRootWindow(uiapp->ui_display);

	// Find or create the window if we weren't given a specific window
	// on the command line.
	if (uiapp->window == 0) {
		// In screensaver mode, we need to draw to the "root" window.
		if (uiapp->app->config.is_in_screensaver_mode) {
			// If we want to draw directly to the root window (the case of
			// the screensaver mode), we need to
			// make sure we use the right window.
			// http://developer.gnome.org/wm-spec/#id2550868
			// The right window could be a child of the root window which
			// has a property "__SWM_VROOT" or "_NET_VIRTUAL_ROOTS".  During
			// tests only __SWM_VROOT was found.
			int num_windows;
			Window *windows = nnuinounoursapp_find_child_windows_for_property(
					uiapp->ui_display, uiapp->root_window, "__SWM_VROOT",
					&num_windows);
			if (num_windows > 0) {
				uiapp->window = windows[0];
				XClientMessageEvent event;
				nnuinounoursapp_init_client_message_event(&event, uiapp,
						uiapp->atom_my_window, 32);
				event.data.l[0] = (long) uiapp->window;
				nnuinounoursapp_send_client_message_event(&event, uiapp);
			} else {
				uiapp->window = uiapp->root_window;
			}
		}
		// We're not in screensaver mode.  Let's create a window.
		else {
			int black_color =
					BlackPixel(uiapp->ui_display, uiapp->screen_number);
			uiapp->window = XCreateSimpleWindow(uiapp->ui_display,
					uiapp->root_window, 0, 0, 1, 1, 0, black_color,
					black_color);
			XMapWindow(uiapp->ui_display, uiapp->window);
		}
	}
}

static void nnuinounoursapp_switch_window(NNUINounoursApp *uiapp) {
	int num_windows;
	Window *windows = nnuinounoursapp_find_child_windows_for_property(
			uiapp->ui_display, uiapp->root_window, "__SWM_VROOT", &num_windows);
	int i;
	for (i = 0; i < num_windows; i++) {
		if (windows[i] != uiapp->window) {
			syslog(LOG_DEBUG, "switching from %lx to %lx", uiapp->window,
					windows[i]);
			uiapp->window = windows[i];
		}
	}
}

static void nnuinounoursapp_get_display_size(NNUINounoursApp *uiapp, int *width,
		int *height) {
	if (uiapp->app->config.is_in_screensaver_mode) {
		XWindowAttributes window_attributes;
		XGetWindowAttributes(uiapp->background_display, uiapp->window,
				&window_attributes);
		*width = window_attributes.width;
		*height = window_attributes.height;
	} else {
		*width = DisplayWidth(uiapp->background_display, uiapp->screen_number);
		*height = DisplayHeight(uiapp->background_display, uiapp->screen_number);
	}
}

void nnuinounoursapp_stretch(NNUINounoursApp *uiapp) {
	XWindowAttributes window_attributes;
	XGetWindowAttributes(uiapp->background_display, uiapp->window,
			&window_attributes);
	int display_width, display_height;
	nnuinounoursapp_get_display_size(uiapp, &display_width, &display_height);
	nnuinounoursapp_resize(uiapp, display_width, display_height);
}
static void nnuinounoursapp_set_background_color(NNUINounoursApp *uiapp) {
	char *background_color = uiapp->app->config.theme->background_color;
	if (background_color != 0) {
		unsigned long color_pixel = 0;
		if (!strcmp(background_color, "white")) {
			color_pixel =
					WhitePixel(uiapp->background_display, uiapp->screen_number);
		} else if (!strcmp(background_color, "black")) {
			color_pixel =
					BlackPixel(uiapp->background_display, uiapp->screen_number);
		}
		if (color_pixel != 0)
			XSetWindowBackground(uiapp->background_display, uiapp->window,
					color_pixel);

	}
}
static void nnuinounoursapp_resize_images(NNUINounoursApp *uiapp, int width,
		int height) {
	NNTheme *theme = uiapp->app->config.theme;

	float width_ratio = (float) width / theme->width;
	float height_ratio = (float) height / theme->height;
	float ratio_to_use =
			width_ratio > height_ratio ? height_ratio : width_ratio;
	int image_dest_height = ratio_to_use * theme->height;
	int image_dest_width = ratio_to_use * theme->width;

	nnbool size_changed = width != theme->width || height != theme->height;
	int i, j, k;
	NNNounoursGrid *grid = uiapp->app->grid;
	for (i = 0; i < grid->width; i++) {
		for (j = 0; j < grid->height; j++) {
			NNUINounours *uinounours = grid->nounoursen[i][j]->uinounours;
			uinounours->window_x = i * width;
			uinounours->window_y = j * height;
			uinounours->window_width = width;
			uinounours->window_height = height;
			if (size_changed) {
				for (k = 0; k < theme->num_images; k++) {
					nnuiimage_resize(uiapp, theme->images[k]->uiimage,
							image_dest_width, image_dest_height);
				}
			}
		}
	}
}
static void nnuinounoursapp_fix_window(NNUINounoursApp *uiapp, int width,
		int height) {
	XSizeHints* size_hints = XAllocSizeHints();
	size_hints->flags = PMinSize | PMaxSize;
	size_hints->min_width = width;
	size_hints->min_height = height;
	size_hints->max_width = width;
	size_hints->max_height = height;
	uiapp->window_width = width;
	uiapp->window_height = height;
	XSetWMNormalHints(uiapp->background_display, uiapp->window, size_hints);
	XFree(size_hints);
}

void nnuinounoursapp_resize(NNUINounoursApp *uiapp, int width, int height) {
	int display_width, display_height;
	nnuinounoursapp_get_display_size(uiapp, &display_width, &display_height);

	int offset_x = 0;
	int offset_y = 0;
	if (uiapp->app->config.is_in_screensaver_mode) {
		XWindowAttributes window_attributes;
		XGetWindowAttributes(uiapp->background_display, uiapp->window,
				&window_attributes);
		offset_x = window_attributes.x;
		offset_y = window_attributes.y;
	}
	XMoveResizeWindow(uiapp->background_display, uiapp->window,
			offset_x + ((display_width - width) / 2),
			offset_y + ((display_height - height) / 2), width, height);
	nnuinounoursapp_set_background_color(uiapp);
	nnuinounoursapp_fix_window(uiapp, width, height);
	nnuinounoursapp_resize_images(uiapp, width, height);
}

static NNNounours * nnuinounoursapp_find_nounours(NNUINounoursApp *uiapp,
		int window_x, int window_y) {
	return uiapp->app->grid->nounoursen[0][0];
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

	nnuinounoursapp_setup_window(uiapp);

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
			nnuinounoursapp_read_client_event_data(&client_message_event,
					&event_nounours, &uiimage);
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
						nnuinounoursapp_switch_window(uiapp);
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
