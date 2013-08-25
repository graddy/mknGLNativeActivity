/*
 * type.h
 *
 *  Created on: 2013/08/18
 *      Author: graddy
 */

#ifndef TYPE_H_
#define TYPE_H_

/*
 * POSITION�\����
 * �񎟌���̍��W�\���Ɏg�p
 */
typedef struct {
	long x;
	long y;
}S_POSITION;
/*
 * POINT�\����
 * �O������̍��W�\���Ɏg�p
 */
typedef struct {
	float x;
	float y;
	float z;
}S_POINT;

/*
 * SIZE�\����
 *
 */
typedef struct {
	long width;
	long height;
}S_SIZE;
#endif /* TYPE_H_ */