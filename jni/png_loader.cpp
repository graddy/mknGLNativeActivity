/*
 * png_loader.c
 *
 *  Created on: 2013/07/07
 *      Author: graddy
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <png.h>
#include <jni.h>
#include <android/asset_manager.h>
#include <android/log.h>
#define LOG_TAG ("png_loder")
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

int offset = 0;
void callback_AssetRead(png_structp pPng, png_bytep buf, png_size_t size);

/*************************************************************************
 * 	png_loadimage
 * 	ファイル名とAssetManagerから、PNG読み込み
 * 	引数		：　pFilename　： ファイル名
 * 			： 　pAssetManager ：AssetManagerのポインタ
 *************************************************************************/
int png_loadimage(AAssetManager* pAssetManager, const char* pFilename,
		png_uint_32* outWidth, png_uint_32* outHeight, GLint *type, u_char **outData)
{
	png_structp png_ptr;
	png_infop info_ptr;
	int depth;
	int color_type;
	AAsset *assetFile;
	int size;
	/* ファイル読み込み */
	assetFile = AAssetManager_open(pAssetManager, pFilename, AASSET_MODE_RANDOM);
	if(assetFile == NULL)
	{
		LOGE("%s is not found.", pFilename);
		return -1;
	}
	/* ファイルサイズ取得 */
	size = AAsset_getLength(assetFile);
	/* メモリ取得 */
	u_char* buf = (u_char*)malloc(size);
	if(buf == NULL)
	{
		LOGE("Buffer Lock Error!!");
		AAsset_close(assetFile);
		return -1;
	}
	/* 読み出し */
	AAsset_read(assetFile, buf ,size);
	AAsset_close(assetFile);

	/* PNG ファイルかチェック */
	if(png_sig_cmp(buf,0,8) != 0)
	{
		/* Pngファイルではない */
		free(buf);
		return -1;
	}
	/* 読み込みメモリ作成 */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		LOGE("png_create_read_struct error.");
		free(buf);
		return -1;
	}
	/* 情報メモリ作成 */
	info_ptr = png_create_info_struct(	png_ptr);
	if (info_ptr == NULL) {
		LOGE("png_create_info_struct error.");
		png_destroy_read_struct(&	png_ptr, NULL, NULL);
		free(buf);
		return -1;
	}
	/* Read時の処理関数登録 */
	png_set_read_fn(png_ptr, buf, callback_AssetRead);
	offset = 8;
	if (setjmp(png_jmpbuf(png_ptr))) {
		LOGE("png_jmpbuf error.");
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		free(buf);
		return -1;
	}
	LOGI("try png read.");
	/* PNGの識別エリアスキップ */
	unsigned int sig_bytes = 8;
	png_set_sig_bytes(png_ptr, sig_bytes);
	/* PNGの読み込み	 */
	png_read_png(png_ptr, info_ptr, (PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND), NULL);

	/* 画像情報取得 */
	*outWidth = info_ptr->width;
	*outHeight =info_ptr->height;
	color_type = info_ptr->color_type;
	depth = info_ptr->bit_depth;

	switch(color_type)
	{
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(png_ptr);
		*type = GL_RGB;
		break;
	case PNG_COLOR_TYPE_RGB:
		*type = GL_RGB;
		break;
	case PNG_COLOR_TYPE_RGBA:
		*type = GL_RGBA;
		break;
	default:
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		free(buf);
		return -1;
	}
	/* 画像1Lineのメモリ確保 */
	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	*outData = (unsigned char*)malloc(row_bytes * (*outHeight));

	/* 画像を行単位で取得 */
	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	/* 画像を上下反転して取得 */
	/* for OpenGL用 */
	int i;
	for(i = 0; i<*outHeight;i++){
		memcpy(*outData+(row_bytes * (*outHeight -1-i)), row_pointers[i], row_bytes);
	}

	/* メモリ開放 */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	free(buf);
	return 0;
}
/*************************************************************************
 * 	callback_AssetRead
 * 	png読み込み、データ読み出しCallBack
 *************************************************************************/

void callback_AssetRead(png_structp pPng, png_bytep buf, png_size_t size)
{
	LOGI("callback_AssetRead %d", size);
	u_char* p = (u_char*)png_get_io_ptr(pPng);
	LOGI("png_get_io_ptr %d",offset);
	memcpy(buf, p+offset, size);
	offset += size;

}
