/*
 * type.h
 *
 *  Created on: 2013/08/18
 *      Author: graddy
 */

#ifndef TYPE_H_
#define TYPE_H_

/*
 * POSITION構造体
 * 二次元上の座標表現に使用
 */
typedef struct {
	long x;
	long y;
}S_POSITION;
/*
 * POINT構造体
 * 三次元上の座標表現に使用
 */
typedef struct {
	float x;
	float y;
	float z;
}S_POINT;

/*
 * SIZE構造体
 *
 */
typedef struct {
	long width;
	long height;
}S_SIZE;
#endif /* TYPE_H_ */
