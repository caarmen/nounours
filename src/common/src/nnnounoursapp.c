/*
 *  Created on: Mar 11, 2012
 *      Author: Carmen Alvarez
 */

#include <stdlib.h>
#include "nnnounoursapp.h"
#include "nncommon.h"
#include "nnnounours.h"

static NNNounoursGrid* nnnounoursgrid_new(NNNounoursApp *app,int width, int height, char *path,
		nnbool is_screensaver_mode, int window_id) {
	NNNounoursGrid * grid = malloc(sizeof(NNNounoursGrid));
	grid->width = width;
	grid->height = height;
	grid->nounoursen = malloc(width * height * sizeof(NNNounours**));
	int i, j;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			grid->nounoursen[i][j] = nnnounours_new(app, path,
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

	app->grid = nnnounoursgrid_new(app, width, height, path, is_screensaver_mode, window_id);

	return app;
}

static void nnnounoursgrid_free(NNNounoursGrid *grid) {
	int i, j;
	for (i = 0; i < grid->width; i++) {
		int j;
		for (j = 0; j < grid->height; j++) {
			nnnounours_free(grid->nounoursen[i][j]);
		}
	}
	free(grid);
}

void nnnounoursapp_free(NNNounoursApp *app) {
	nnnounoursgrid_free(app->grid);
	free(app);
}
