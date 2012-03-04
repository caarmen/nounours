/*
 * nncommon.h
 *
 *  Created on: Feb 19, 2012
 *      Author: calvarez
 */

#ifndef NNCOMMON_H_
#define NNCOMMON_H_

#define NN_INITIAL_LIST_CAPACITY 10

#ifndef __DATAROOT_DIR__
#define __DATAROOT_DIR__ "/usr/local/share"
#endif

void * nnresize_if_needed(void *ptr, int current_size);
#endif /* NNCOMMON_H_ */
