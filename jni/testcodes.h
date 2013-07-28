/*
 * testcodes.h
 *
 *  Created on: 2013/07/14
 *      Author: graddy
 */

#ifndef TESTCODES_H_
#define TESTCODES_H_

void prepareFrame(    int32_t width, int32_t height);

void testcodes_assetResource_Test(struct android_app* state);
void initCube(struct android_app* state);
void drawCube(float* angle);

#endif /* TESTCODES_H_ */
