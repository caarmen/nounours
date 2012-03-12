/*
 *  Created on: Mar 12, 2012
 *      Author: Carmen Alvarez
 */
#include <pthread.h>
#include <string.h>
#include "nnclientmessage.h"

pthread_mutex_t nnclientmessage_mutex;

void nnclientmessage_setup() {
	pthread_mutex_init(&nnclientmessage_mutex, 0);
}
void nnclientmessage_init(XClientMessageEvent *event, NNUINounoursApp *uiapp,
		Atom atom, int format) {
	memset(event, 0, sizeof(XClientMessageEvent));
	event->type = ClientMessage;
	event->window = uiapp->window;
	event->format = format; // doesn't really matter since we use memcpy to pass the pointer
	event->message_type = atom;
}
void nnclientmessage_send(XClientMessageEvent *event, NNUINounoursApp *uiapp) {
	long event_mask = NoEventMask;
	if (uiapp->app->config.is_in_screensaver_mode)
		event_mask = ExposureMask; // TODO other applications may receive this event!
	pthread_mutex_lock(&nnclientmessage_mutex);
	XSendEvent(uiapp->background_display, uiapp->window, 0, event_mask,
			(XEvent*) event);
	XFlush(uiapp->background_display);
	pthread_mutex_unlock(&nnclientmessage_mutex);
}

void nnclientmessage_write(XClientMessageEvent *event, NNNounours *nounours,
		NNUIImage *uiimage) {
	void *ptr = event->data.l;
	memcpy(ptr, &uiimage, sizeof(NNUIImage*));
	ptr += sizeof(NNUIImage*);
	memcpy(ptr, &nounours, sizeof(NNNounours*));
}

void nnclientmessage_read(XClientMessageEvent *event, NNNounours **nounours,
		NNUIImage **uiimage) {
	void *ptr = event->data.l;
	memcpy(uiimage, ptr, sizeof(NNUIImage*));
	ptr += sizeof(NNUIImage*);
	memcpy(nounours, ptr, sizeof(NNNounours*));
}
