/*
 * Copyright 2012 Carmen Alvarez
 *
 * This file is part of Nounours.
 *
 * Nounours is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Nounours is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nounours.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NNNOUNOURSAPP_H_
#define NNNOUNOURSAPP_H_

#include "nnnounours.h"
#include "nntheme.h"
#include "nncommon.h"
#include "nnuinounoursapp.h"

struct NNNounours;
struct NNTheme;
struct NNUINounoursApp;

typedef struct NNNounoursConfig {
	int shake_factor; // The speed in pixels/second which will trigger the shake animation
	int idle_time_for_sleep_ms;
	int idle_time_for_auto_move_ms;
	nnbool is_in_screensaver_mode;
	nnbool do_stretch;
	struct NNTheme *theme;
} NNNounoursConfig;

typedef struct NNNounoursGrid {
	int width;
	int height;
	struct NNNounours ***nounoursen;
} NNNounoursGrid;

typedef struct NNNounoursApp {
	NNNounoursConfig config;
	NNNounoursGrid *grid;
	struct NNUINounoursApp *ui;
} NNNounoursApp;

NNNounoursApp * nnnounoursapp_new(int width, int height, const char *path,
		nnbool is_screensaver_mode, int window_id);
void nnnounoursapp_use_theme_scaled(NNNounoursApp *app, struct NNTheme *theme,
		float scale);
void nnnounoursgrid_on_shake(NNNounoursGrid *grid);
void nnnounoursapp_free(NNNounoursApp *app);

#endif /* NNNOUNOURSAPP_H_ */
