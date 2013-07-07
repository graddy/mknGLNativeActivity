/*
 * png_loader.h
 *
 *  Created on: 2013/07/07
 *      Author: graddy
 */

#ifndef PNG_LOADER_H_
#define PNG_LOADER_H_
int png_loadimage(AAssetManager* pAssetManager, const char* pFilename,
		png_uint_32* outWidth, png_uint_32* outHeight, GLint *type, u_char **outData);

#endif /* PNG_LOADER_H_ */
