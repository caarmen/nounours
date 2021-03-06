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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include "nncommon.h"
#include "nnnounours.h"
#include "nnimage.h"
#include "nnfeature.h"
#include "nnanimation.h"
#include "nntheme.h"
#include "nncsvreader.h"
#include "nnpropertiesreader.h"
#include "nnnounoursapp.h"

void help(char *prog_name) {
	printf("Usage:\n");
	printf("%s [OPTIONS]\n", prog_name);
	printf("The following options are supported:\n");
	printf("  -theme <theme>\n");
	printf("  -screensaver\n");
	printf("  -window-id <window id>\n");
	printf("  -sleep-after <time in seconds>\n");
	printf("  -stretch\n");
	printf("  -rows <rows>\n");
	printf("  -cols <cols>\n");
	printf("  -scale <scale factor>\n");
	printf("<theme> may be:\n");
	printf("  /path/to/theme\n");
	printf("  default|rainbow|college|robot|random\n");
	printf("\n");
	printf(
			"To use nounours as a screensaver, add the following line to your ~/.xscreensaver file, in the 'programs:' section:\n");
	printf("nounours -screensaver -stretch \\n\\\n");
	printf(
			"If you don't have a ~/.xscreensaver file, run xscreensaver-demo which will create the file.  Then add the above line.\n");
	exit(1);
}
int main(int argc, char **argv) {
	const char *theme_path = "default";
	int screensaver_mode = 0;
	unsigned long window_id = 0;
	nnbool do_stretch = NNFALSE;
	int i;
	int sleep_time = -1;
	float scale = 1.0f;
	struct timeval now_tv;
	gettimeofday(&now_tv, NULL);
	long now_us = now_tv.tv_sec * 1000000 + now_tv.tv_usec;
	int rows = 1;
	int cols = 1;
	srandom(now_us);

	openlog("nounours", LOG_CONS | LOG_PID, 0);
	syslog(LOG_ERR, "starting");

	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-theme")) {
			if (i == argc) {
				printf("Missing theme path.\n");
				help(argv[0]);
			} else {
				theme_path = argv[++i];
				if (!strcmp(theme_path, "random")) {
					theme_path = nntheme_get_random_theme_id();
					if (theme_path == NULL) {
						fprintf(stderr, "Could not get random theme\n");
						exit(1);
					}
				}
			}
		} else if (!strcmp(argv[i], "-screensaver")) {
			screensaver_mode = 1;
		} else if (!strcmp(argv[i], "-window-id")) {
			if (i == argc) {
				printf("Missing window id.\n");
				help(argv[0]);
			} else {
				char * window_id_str = argv[++i];
				window_id = strtol(window_id_str, NULL, 16);
			}
		} else if (!strcmp(argv[i], "-stretch")) {
			do_stretch = NNTRUE;
		} else if (!strcmp(argv[i], "-sleep-after")) {
			if (i == argc) {
				printf("Missing sleep time.\n");
				help(argv[0]);
			}
			sleep_time = atoi(argv[++i]) * 1000;
		} else if (!strcmp(argv[i], "-scale")) {
			if (i == argc) {
				printf("Missing scale factor.\n");
				help(argv[0]);
			}
			scale = strtof(argv[++i], NULL);
		} else if (!strcmp(argv[i], "-rows")) {
			if (i == argc) {
				printf("Missing number of rows.\n");
				help(argv[0]);
			}
			rows = atoi(argv[++i]);
		} else if (!strcmp(argv[i], "-cols")) {
			if (i == argc) {
				printf("Missing number of cols.\n");
				help(argv[0]);
			}
			cols = atoi(argv[++i]);
		} else {
			printf("Unknown option %s\n", argv[i]);
			help(argv[0]);
		}
	}

	NNNounoursApp *app = nnnounoursapp_new(cols, rows, theme_path,
			screensaver_mode, window_id);
	app->config.do_stretch = do_stretch;
	if (sleep_time > 0)
		app->config.idle_time_for_sleep_ms = sleep_time;
	NNTheme *theme = nntheme_new(app, strdup(theme_path));
	nnnounoursapp_use_theme_scaled(app, theme, scale);
	while (1) {
		sleep(1000000);
	}
	closelog();
	return 0;
}
