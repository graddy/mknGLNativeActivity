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
 * 	�t�@�C������AssetManager����APNG�ǂݍ���
 * 	����		�F�@pFilename�@�F �t�@�C����
 * 			�F �@pAssetManager �FAssetManager�̃|�C���^
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
	/* �t�@�C���ǂݍ��� */
	assetFile = AAssetManager_open(pAssetManager, pFilename, AASSET_MODE_RANDOM);
	if(assetFile == NULL)
	{
		LOGE("%s is not found.", pFilename);
		return -1;
	}
	/* �t�@�C���T�C�Y�擾 */
	size = AAsset_getLength(assetFile);
	/* �������擾 */
	u_char* buf = (u_char*)malloc(size);
	if(buf == NULL)
	{
		LOGE("Buffer Lock Error!!");
		AAsset_close(assetFile);
		return -1;
	}
	/* �ǂݏo�� */
	AAsset_read(assetFile, buf ,size);
	AAsset_close(assetFile);

	/* PNG �t�@�C�����`�F�b�N */
	if(png_sig_cmp(buf,0,8) != 0)
	{
		/* Png�t�@�C���ł͂Ȃ� */
		free(buf);
		return -1;
	}
	/* �ǂݍ��݃������쐬 */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		LOGE("png_create_read_struct error.");
		free(buf);
		return -1;
	}
	/* ��񃁃����쐬 */
	info_ptr = png_create_info_struct(	png_ptr);
	if (info_ptr == NULL) {
		LOGE("png_create_info_struct error.");
		png_destroy_read_struct(&	png_ptr, NULL, NULL);
		free(buf);
		return -1;
	}
	/* Read���̏����֐��o�^ */
	png_set_read_fn(png_ptr, buf, callback_AssetRead);
	offset = 8;
	if (setjmp(png_jmpbuf(png_ptr))) {
		LOGE("png_jmpbuf error.");
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		free(buf);
		return -1;
	}
	LOGI("try png read.");
	/* PNG�̎��ʃG���A�X�L�b�v */
	unsigned int sig_bytes = 8;
	png_set_sig_bytes(png_ptr, sig_bytes);
	/* PNG�̓ǂݍ���	 */
	png_read_png(png_ptr, info_ptr, (PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND), NULL);

	/* �摜���擾 */
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
	/* �摜1Line�̃������m�� */
	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	*outData = (unsigned char*)malloc(row_bytes * (*outHeight));

	/* �摜���s�P�ʂŎ擾 */
	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	/* �摜���㉺���]���Ď擾 */
	/* for OpenGL�p */
	int i;
	for(i = 0; i<*outHeight;i++){
		memcpy(*outData+(row_bytes * (*outHeight -1-i)), row_pointers[i], row_bytes);
	}

	/* �������J�� */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	free(buf);
	return 0;
}
/*************************************************************************
 * 	callback_AssetRead
 * 	png�ǂݍ��݁A�f�[�^�ǂݏo��CallBack
 *************************************************************************/

void callback_AssetRead(png_structp pPng, png_bytep buf, png_size_t size)
{
	LOGI("callback_AssetRead %d", size);
	u_char* p = (u_char*)png_get_io_ptr(pPng);
	LOGI("png_get_io_ptr %d",offset);
	memcpy(buf, p+offset, size);
	offset += size;

}
