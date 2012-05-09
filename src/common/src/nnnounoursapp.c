/*
 *  Created on: Mar 11, 2012
 *      Author: Carmen Alvarez
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nnnounoursapp.h"
#include "nncommon.h"
#include "nnnounours.h"
#include "nntheme.h"

static NNNounoursGrid* nnnounoursgrid_new(NNNounoursApp *app,int width, int height, const char *path,
		nnbool is_screensaver_mode, int window_id) {
	NNNounoursGrid * grid = malloc(sizeof(NNNounoursGrid));
	grid->width = width;
	grid->height = height;
	grid->nounoursen = malloc(width * sizeof(NNNounours**));
	int i, j;
	for (i = 0; i < width; i++) {
		grid->nounoursen[i] = malloc(height * sizeof(NNNounours*));
		for (j = 0; j < height; j++) {
			char id[16];
			sprintf(id, "(%03d,%03d)", i, j);
			grid->nounoursen[i][j] = nnnounours_new(strdup(id), app, path,
					is_screensaver_mode, window_id);
		}
	}
	return grid;
}

NNNounoursApp * nnnounoursapp_new(int width, int height, const char *path, nnbool is_screensaver_mode, int window_id) {
	NNNounoursApp *app = malloc(sizeof(NNNounoursApp));
	app->config.is_in_screensaver_mode = is_screensaver_mode;
	app->config.do_stretch = NNFALSE;
	app->config.shake_factor = 0;
	app->config.idle_time_for_sleep_ms = 0;
	app->config.idle_time_for_auto_move_ms = 0;
	app->config.theme = 0;
	// Read in our config
	nnread_nounours_properties_file(&app->config, path);
	app->ui = nnuinounoursapp_new(app, window_id);

	app->grid = nnnounoursgrid_new(app, width, height, path, is_screensaver_mode, window_id);
	// Start the nounours UI loop in a separate thread
	nnuinounoursapp_start_loop(app->ui);

	return app;
}

void nnnounoursapp_use_theme_scaled(NNNounoursApp *app, NNTheme *theme, float scale) {
	app->config.theme = theme;

	if (app->config.do_stretch)
		nnwindow_stretch(app->ui);
	else
		nnwindow_resize(app->ui, (int) (scale*theme->width*app->grid->width), (int) (scale*theme->height*app->grid->height));

	if(!app->config.is_in_screensaver_mode) {
		char icon_filename[1024];
		sprintf(icon_filename,
				"%s/nounours/data/themes/%s/images/nounours.xpm",
				__DATAROOT_DIR__,
				theme->path);
		nnwindow_set_icon(app->ui, icon_filename);
	}
	int i, j;
	for(i=0; i < app->grid->width; i++) {
		for(j=0; j < app->grid->height; j++) {
			nnnounours_show_image(app->grid->nounoursen[i][j], theme->default_image);
		}
	}
}
void nnnounoursgrid_on_shake(NNNounoursGrid *grid) {
	int i, j;
	for(i=0; i < grid->width; i++) {
		for(j=0; j < grid->height; j++) {
			nnnounours_on_shake(grid->nounoursen[i][j]);
		}
	}
}
static void nnnounoursgrid_free(NNNounoursGrid *grid) {
	int i, j;
	for (i = 0; i < grid->width; i++) {
		int j;
		for (j = 0; j < grid->height; j++) {
			nnnounours_free(grid->nounoursen[i][j]);
		}
		free(grid->nounoursen[i]);
	}
	free(grid->nounoursen);
	free(grid);
}

void nnnounoursapp_free(NNNounoursApp *app) {
	nnuinounoursapp_stop_loop(app->ui);
	nnnounoursgrid_free(app->grid);
	free(app);
}
