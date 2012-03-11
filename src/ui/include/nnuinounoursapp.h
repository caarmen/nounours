/*
 *  Created on: Mar 11, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNUINOUNOURSAPP_H_
#define NNUINOUNOURSAPP_H_

#include "nnnounoursapp.h"
#include "nnnounours.h"
#include "nnuiimage.h"
#include "nncommon.h"

struct NNNounoursApp;
struct NNNounours;
struct NNUIImage;

typedef struct NNUINounoursApp {
	struct NNNounoursApp *app;
	Display *ui_display;
	Display *background_display;
	int screen_number;
	Window window;
	Window root_window;
	GC gc;
	pthread_t ui_thread;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	Atom atom_my_window;
	Atom atom_set_image;
	int last_window_x;
	int last_window_y;
	long last_window_move_time_us;
	nnbool is_running;
	int window_width;
	int window_height;

} NNUINounoursApp;

NNUINounoursApp *nnuinounoursapp_new(struct NNNounoursApp *app, int window_id);


void nnuinounoursapp_start_loop(NNUINounoursApp *uiapp);
void nnuinounoursapp_stop_loop(NNUINounoursApp *uiapp);
#endif /* NNUINOUNOURSAPP_H_ */
