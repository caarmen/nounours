/*
 *  Created on: Mar 12, 2012
 *      Author: Carmen Alvarez
 */

#ifndef NNCLIENTMESSAGE_H_
#define NNCLIENTMESSAGE_H_

#include <X11/Xlib.h>
#include "nnuinounoursapp.h"
#include "nnnounours.h"
#include "nnuiimage.h"

void nnclientmessage_setup();
void nnclientmessage_init(XClientMessageEvent *event,
		NNUINounoursApp *uiapp, Atom atom, int format);
void nnclientmessage_send(XClientMessageEvent *event,
		NNUINounoursApp *uiapp);
void nnclientmessage_write(XClientMessageEvent *event,
		struct NNNounours *nounours, struct NNUIImage *uiimage);
void nnclientmessage_read(XClientMessageEvent *event,
		struct NNNounours **nounours, struct NNUIImage **uiimage);

#endif /* NNCLIENTMESSAGE_H_ */
