/*
 *  Created on: Mar 12, 2012
 *      Author: Carmen Alvarez
 */
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/xpm.h>
#include <stdlib.h>
#include <syslog.h>
#include "nnwindow.h"

/**
 * Search the child windows of the given window for the given property.
 * @return the child windows having the given property.
 */
static Window * nnwindow_find_child_windows_for_property(Display *display,
		Window window, const char *property_name, int *num_results) {
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

void nnwindow_setup(NNUINounoursApp *uiapp) {

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
			Window *windows = nnwindow_find_child_windows_for_property(
					uiapp->ui_display, uiapp->root_window, "__SWM_VROOT",
					&num_windows);
			if (num_windows > 0) {
				uiapp->window = windows[0];
				XClientMessageEvent event;
				nnclientmessage_init(&event, uiapp, uiapp->atom_my_window, 32);
				event.data.l[0] = (long) uiapp->window;
				nnclientmessage_send(&event, uiapp);
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
			XStoreName(uiapp->ui_display, uiapp->window, "Nounours");
			Atom icon_property = XInternAtom(uiapp->ui_display, "_NET_WM_ICON",
					0);
			XpmAttributes attributes;
			XImage *icon_image;
			XImage *icon_shape_image;
			char icon_filename[1024];
			sprintf(icon_filename,
					"%s/nounours/data/icons/nounours.xpm",
					__DATAROOT_DIR__);

			XpmReadFileToImage(uiapp->ui_display, icon_filename, &icon_image,
					&icon_shape_image, &attributes);
			int nelements = (icon_image->width * icon_image->height) + 2;
			XChangeProperty(uiapp->ui_display, uiapp->window, icon_property,
					XA_CARDINAL, 32, PropModeReplace, icon_image->data,
					nelements);

			XMapWindow(uiapp->ui_display, uiapp->window);
		}
	}
}

void nnwindow_switch(NNUINounoursApp *uiapp) {
	int num_windows;
	Window *windows = nnwindow_find_child_windows_for_property(
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

static void nnwindow_get_display_size(NNUINounoursApp *uiapp, int *width,
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

void nnwindow_stretch(NNUINounoursApp *uiapp) {
	XWindowAttributes window_attributes;
	XGetWindowAttributes(uiapp->background_display, uiapp->window,
			&window_attributes);
	int display_width, display_height;
	nnwindow_get_display_size(uiapp, &display_width, &display_height);
	nnwindow_resize(uiapp, display_width, display_height);
}

static void nnwindow_set_background_color(NNUINounoursApp *uiapp) {
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

static void nnwindow_resize_images(NNUINounoursApp *uiapp, int window_width,
		int window_height) {
	NNTheme *theme = uiapp->app->config.theme;
	NNNounoursGrid *grid = uiapp->app->grid;

	int images_width = theme->width * grid->width;
	int images_height = theme->height * grid->height;

	float width_ratio = (float) window_width / images_width;
	float height_ratio = (float) window_height / images_height;
	float ratio_to_use =
			width_ratio > height_ratio ? height_ratio : width_ratio;
	int image_dest_height = ratio_to_use * theme->height;
	int image_dest_width = ratio_to_use * theme->width;

	int offset_x = 0;
	int offset_y = 0;
	if (height_ratio > width_ratio) {
		offset_y = (int) ((window_height - ratio_to_use * images_height) / 2);
	} else {
		offset_x = (int) ((window_width - ratio_to_use * images_width) / 2);
	}

	nnbool size_changed = image_dest_width != theme->width
			|| image_dest_height != theme->height;
	int i, j, k;
	for (i = 0; i < grid->width; i++) {
		for (j = 0; j < grid->height; j++) {
			NNUINounours *uinounours = grid->nounoursen[i][j]->uinounours;
			uinounours->window_x = i * image_dest_width + offset_x;
			uinounours->window_y = j * image_dest_height + offset_y;
			uinounours->window_width = image_dest_width;
			uinounours->window_height = image_dest_height;

			if (size_changed) {
				for (k = 0; k < theme->num_images; k++) {
					nnuiimage_resize(uiapp, theme->images[k]->uiimage,
							image_dest_width, image_dest_height);
				}
			}
		}
	}
}
static void nnwindow_fix(NNUINounoursApp *uiapp, int width, int height) {
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

void nnwindow_resize(NNUINounoursApp *uiapp, int width, int height) {
	int display_width, display_height;
	nnwindow_get_display_size(uiapp, &display_width, &display_height);

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
	nnwindow_set_background_color(uiapp);
	nnwindow_fix(uiapp, width, height);
	nnwindow_resize_images(uiapp, width, height);
}

void nnwindow_get_dimensions(NNUINounoursApp *uiapp, int *offset_x,
		int *offset_y, int *image_width, int *image_height) {
	NNNounoursGrid *grid = uiapp->app->grid;
	NNImage *image = uiapp->app->config.theme->images[0];
	*image_width = image->uiimage->ximage->width;
	*image_height = image->uiimage->ximage->height;
	*offset_x = (uiapp->window_width - grid->width * (*image_width)) / 2;
	*offset_y = (uiapp->window_height - grid->height * (*image_height)) / 2;
}
