/*
 * nnuinounours.h
 *
 *  Created on: Feb 20, 2012
 *      Author: calvarez
 */

#ifndef NNUINOUNOURS_H_
#define NNUINOUNOURS_H_
#include <X11/Xlib.h>

#include "nnnounours.h"
#include "nnuiimage.h"

struct NNNounours;
struct NNUIImage;

typedef struct NNUINounours {
	Display *ui_display;
	Display *background_display;
	int screen_number;
	Window window;
	Window root_window;
	GC gc;
	struct NNNounours *nounours;
	int is_running;
	pthread_t ui_thread;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int last_window_x;
	int last_window_y;
	long last_window_move_time_us;

} NNUINounours;

NNUINounours *nnuinounours_new(struct NNNounours *nounours);
void nnuinounours_resize(NNUINounours *nounours, int width, int height);
void nnuinounours_free(NNUINounours *nounours);
void nnuinounours_show_image(NNUINounours *nounours, struct NNUIImage *image);
void nnuinounours_start_loop(NNUINounours *uinounours);
void nnuinounours_stop_loop(NNUINounours *uinounours);
int nnuinounours_error_handler(Display *display, XErrorEvent *error_event);

#endif /* NNUINOUNOURS_H_ */
