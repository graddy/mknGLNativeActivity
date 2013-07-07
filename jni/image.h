/*
 * image.h
 *
 *  Created on: 2013/06/30
 *      Author: graddy
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <string>
#include <png.h>
#include <android/asset_manager.h>

class PNG
{
public:
	PNG(const std::string& file_name);
	PNG(const std::string& file_name, AAssetManager* pAssetManager);
	~PNG();
	unsigned int get_width();
	unsigned int get_height();
	bool has_alpha();
	unsigned char* get_data();


//	void callback_AssetRead(png_structp ppng, png_bytep buf, png_size_t size);
	static int imgOffset;
private:
	const std::string file_name_;
	unsigned char* data_;
	png_uint_32 width_, height_;
	int bit_depth_, color_type_, interlace_type_;
	int compression_type_, filter_method_;
};

void image_callback_AssetRead(png_structp ppng, png_bytep buf, png_size_t size);

#endif /* IMAGE_H_ */
