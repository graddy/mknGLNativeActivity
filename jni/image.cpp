/*
 * PNG.cpp
 *
 *  Created on: 2013/06/30
 *      Author: graddy
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <image.h>
#include <android/log.h>
#define LOG_TAG ("PNG")
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
/*************************************************************************
 * 	PNG コンストラクタ
 * 	ファイル名から、PNG読み込み
 * 	引数		：　file_name　： ファイル名
 *************************************************************************/
PNG::PNG(const std::string& file_name) :
	file_name_(file_name), data_(NULL),
	width_(0), height_(0),
	bit_depth_(0), color_type_(0), interlace_type_(0),
	compression_type_(0), filter_method_(0)
{
	FILE* fp = fopen(file_name_.c_str(), "rb");
	if (fp == NULL) {
		LOGE("%s is not found.", file_name_.c_str());
		fclose(fp);
		return;
	}

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png == NULL) {
		LOGE("png_create_read_struct error.");
		fclose(fp);
		return;
	}

	png_infop info = png_create_info_struct(png);
	if (info == NULL) {
		LOGE("png_create_info_struct error.");
		png_destroy_read_struct(&png, NULL, NULL);
		fclose(fp);
		return;
	}

	if (setjmp(png_jmpbuf(png))) {
		LOGE("png_jmpbuf error.");
		png_destroy_read_struct(&png, &info, NULL);
		fclose(fp);
		return;
	}

	png_init_io(png, fp);

	unsigned int sig_bytes = 0;
	png_set_sig_bytes(png, sig_bytes);

	png_read_png(png, info, (PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND), NULL);
	png_get_IHDR(png, info, &width_, &height_, &bit_depth_, &color_type_, &interlace_type_, &compression_type_, &filter_method_);

	unsigned int row_bytes = png_get_rowbytes(png, info);
	if (data_ != NULL) {
		delete[] data_;
	}
	data_ = new unsigned char[row_bytes * height_];

	png_bytepp rows = png_get_rows(png, info);
	for (int i = 0; i < height_; ++i) {
		memcpy(data_ + (row_bytes * i), rows[i], row_bytes);
	}

	png_destroy_read_struct(&png, &info, NULL);
}

/*************************************************************************
 * 	PNG コンストラクタ
 * 	ファイル名とAssetManagerから、PNG読み込み
 * 	引数		：　file_name　： ファイル名
 * 			： 　pAssetManager ：AssetManagerのポインタ
 *************************************************************************/
PNG::PNG(const std::string& file_name, AAssetManager* pAssetManager) :
	file_name_(file_name)
{
	AAsset	*assetFile;
	int size;
	/* ファイル読み込み */
	assetFile = AAssetManager_open(pAssetManager, file_name.c_str(), AASSET_MODE_UNKNOWN);
	if(assetFile == NULL)
	{
		LOGE("%s is not found.", file_name_.c_str());
		return;
	}
	/* ファイルサイズ取得 */
	size = AAsset_getLength(assetFile);
	/* メモリ取得 */
	u_char* buf = (u_char*)malloc(size);
	if(buf == NULL)
	{
		LOGE("Buffer Lock Error!!");
		AAsset_close(assetFile);
		return;
	}
	/* 読み出し */
	AAsset_read(assetFile, buf ,size);
	AAsset_close(assetFile);

	/* PNG ファイルかチェック */
	if(png_sig_cmp(buf,0,8) != 0)
	{
		/* Pngファイルではない */
		free(buf);
		return;
	}
	/* 読み込みメモリ作成 */
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png == NULL) {
		LOGE("png_create_read_struct error.");
		free(buf);
		return;
	}
	/* 情報メモリ作成 */
	png_infop info = png_create_info_struct(png);
	if (info == NULL) {
		LOGE("png_create_info_struct error.");
		png_destroy_read_struct(&png, NULL, NULL);
		free(buf);
		return;
	}
	/* Read時の処理関数登録 */
	png_set_read_fn(png, buf, image_callback_AssetRead);

	if (setjmp(png_jmpbuf(png))) {
		LOGE("png_jmpbuf error.");
		png_destroy_read_struct(&png, &info, NULL);
		free(buf);
		return;
	}
	LOGI("try png read.");
	/* PNGの識別エリアスキップ */
	unsigned int sig_bytes = 8;
	png_set_sig_bytes(png, sig_bytes);
	/* PNGの読み込み	 */
	png_read_png(png, info, (PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND), NULL);

	/* 画像情報取得 */
	/* .... */

	/* メモリ開放 */
	png_destroy_read_struct(&png, &info, NULL);
	free(buf);
}

PNG::~PNG()
{
	if (data_) delete[] data_;
}

unsigned int PNG::get_width()
{
	return width_;
}

unsigned int PNG::get_height()
{
	return height_;
}

unsigned char* PNG::get_data()
{
	return data_;
}

bool PNG::has_alpha()
{
	if (color_type_ == PNG_COLOR_TYPE_RGBA) {
		return true;
	}
	return false;
}
void image_callback_AssetRead(png_structp pPng, png_bytep buf, png_size_t size)
{
	static int imgOffset= 0;
	LOGI("callback_AssetRead");
//	u_char* p = (u_char*)png_get_io_ptr(pPng);
	LOGI("png_get_io_ptr %d",imgOffset);
//	memcpy(buf, p+imgOffset, size);
	imgOffset += size;

}
