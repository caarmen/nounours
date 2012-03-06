/*
 * main.c
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include "nnnounours.h"
#include "nnimage.h"
#include "nnfeature.h"
#include "nnanimation.h"
#include "nntheme.h"
#include "nncsvreader.h"
#include "nnpropertiesreader.h"

void help(char *prog_name) {
	printf("Usage:\n");
	printf(
			"%s [-theme <path/to/theme>] [-screensaver] [-window-id <window id>] [-stretch] [-sleep-after <time in seconds>]\n",
			prog_name);
	exit(1);
}
int main(int argc, char **argv) {
	const char *theme_path = "data/themes/1";
	int screensaver_mode = 0;
	unsigned long window_id = 0;
	int do_stretch = 0;
	int i;
	int sleep_time = -1;
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-theme")) {
			if (i == argc) {
				printf("Missing theme path.\n");
				help(argv[0]);
			} else {
				theme_path = argv[++i];
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
			do_stretch = 1;
		} else if (!strcmp(argv[i], "-sleep-after")) {
			if(i==argc) {
				printf("Missing sleep time.\n");
				help(argv[0]);
			}
			sleep_time = atoi(argv[++i]) * 1000;
		} else {
			printf("Unknown option %s\n", argv[i]);
			help(argv[0]);
		}
	}
	struct timeval now_tv;
	gettimeofday(&now_tv, NULL);
	long now_us = now_tv.tv_sec * 1000000 + now_tv.tv_usec;
	srandom(now_us);

	openlog("nounours", LOG_CONS | LOG_PID, 0);
	syslog(LOG_ERR, "starting");

	NNNounours *nounours = nnnounours_new(theme_path, screensaver_mode,
			window_id);
	if(sleep_time >0)
		nounours->idle_time = sleep_time;
	nounours->do_stretch = do_stretch;
	NNTheme *theme = nntheme_new(nounours, strdup(theme_path));
	nnnounours_use_theme(nounours, theme);
	while (1) {
		sleep(1000000);
	}
	closelog();
	return 0;
}
