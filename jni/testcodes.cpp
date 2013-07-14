/*
 * testcoes.cpp
 *
 *  Created on: 2013/07/14
 *      Author: graddy
 */

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <math.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
#include "image.h"
#include "png_loader.h"

/* デバッグ用メッセージ */
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "testcodes", __VA_ARGS__))	//! Infomation
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "testcodes", __VA_ARGS__))	//! Warnning
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "testcodes", __VA_ARGS__))	//! Error

/*
 * リソースファイルの読み込みテスト
 */
void testcodes_assetResource_Test(struct android_app* state)
{
	LOGI("mknTest");
	AAsset *file = AAssetManager_open(state->activity->assetManager, "test.png", AASSET_MODE_UNKNOWN);
	if(file == NULL)
	{
		LOGI("test.png = FAIL");
	}
	else
	{
		LOGI("test.png = SUCCESS");
		AAsset_close(file);
	}

	AAsset *file2 = AAssetManager_open(state->activity->assetManager, "test2.png", AASSET_MODE_UNKNOWN);
	if(file2== NULL)
	{
		LOGI("test2.png = FAIL");
	}
	else
	{
		LOGI("test2.png = SUCCESS");
		AAsset_close(file2);
	}

	png_uint_32 width,height;
	GLint type;
	GLubyte* textureImage;
	png_loadimage(state->activity->assetManager, "test.png", &width, &height,&type,&textureImage);
	/* textureImageのDelete必要 */

}
