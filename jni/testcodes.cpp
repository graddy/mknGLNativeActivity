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
#include "glu.h"

/* デバッグ用メッセージ */
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "testcodes", __VA_ARGS__))	//! Infomation
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "testcodes", __VA_ARGS__))	//! Warnning
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "testcodes", __VA_ARGS__))	//! Error


/*
 *
 ************************************************************************/

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
/***********************************************************************
 * テクスチャ貼り付け用のテストコード
 ***********************************************************************/
/*
 * 四角形の頂点リスト
 */
static const GLfloat cubeVertices[] = {
	-1.0f,	-1.0f,	 1.0f,		1.0f,	-1.0f,	1.0f,
	 1.0f,	 1.0f,	 1.0f,		-1.0f,	1.0f,	1.0f,
	-1.0f,	-1.0f,	-1.0f,		1.0f,	-1.0f,	-1.0f,
	 1.0f,	 1.0f,	-1.0f,		-1.0f,	1.0f,	-1.0f,
	 1.0f,	-1.0f,	1.0f,		1.0f,	-1.0f,	-1.0f,
	 1.0f,	 1.0f,	-1.0f,		1.0f,	1.0f,	1.0f,
	-1.0f,	-1.0f,	-1.0f,		-1.0f,	-1.0f,	1.0f,
	-1.0f,	 1.0f,	 1.0f,		-1.0f,	1.0f,	-1.0f,
	-1.0f,	 1.0f,	 1.0f,		1.0f,	1.0f,	1.0f,
	 1.0f,	 1.0f,	-1.0f,		-1.0f,	1.0f,	-1.0f,
	 1.0f,	-1.0f,	 1.0f,		-1.0f,	-1.0f,	1.0f,
	-1.0f,	-1.0f,	-1.0f,		1.0f,	-1.0f,	-1.0f,
};
/*
 * 頂点描画順
 */
static const GLushort cubeIndices[] ={
	0,	 1,	 2,		2,	 3,	 0,
	5,	 4,	 7,		7,	 6,	 5,
	8,	 9,	10,		10,	11,	 8,
	12,	13,	14,		14,	15,	12,
	16,	17,	18,		18,	19,	16,
	20,	21,	22,		22,	23,	20
};
/*
 * テクスチャ位置
 */
/*
const GLfloat cubeTexCoords[] = {
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1
};
*/
const GLfloat cubeTexCoords[] = {
		0,0.834, 0.25,0.834, 0.25,1.00, 0,1.00,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1
};
/*
 * 描画前処理
 */

void prepareFrame(    int32_t width, int32_t height)
{

	glViewport(0,0,width, height);

	glClearColor(.7f, .7f, .9f ,1.f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, (float)width/ height, 0.5f,500 );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


}

/*
 * テクスチャの透明化（アルファブレンドの有効化）
 */
void testcodeTextureAlphaBrend(void)
{
	//ブレンドモードの設定
	//アルファブレンド
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#if 0	/* ほかの設定 参考 */
	//反転
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);

	//加算
	glBlendFunc(GL_ONE, GL_ONE);

	//加算+アルファ(PhotoShop的スクリーン)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//スクリーン(PhotoShop的 比較(明))
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);

	//乗算
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	//乗算+アルファ
	//dst = dst * src * alpha
	//= (dst * src) * alpha
	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
#endif
	//ブレンド有効
	glEnable(GL_BLEND);
}


/*
 * 四角形の初期化
 */
void initCube(struct android_app* state)
{
	LOGI("initCube");
	GLuint texName[2];
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //アルファブレンド
    testcodeTextureAlphaBrend();

    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texName[0]);
    glBindTexture(GL_TEXTURE_2D, texName[0]);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    png_uint_32 width, height;
    GLint type;
    GLubyte *textureImage;
	png_loadimage(state->activity->assetManager, "test.png", &width, &height,&type,&textureImage);
	/* textureImageのDelete必要 */

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, textureImage);
}
void drawCube(float* angle)
{
	LOGI("drawCube %f", *angle);
	//カメラ位置
	gluLookAt(0,0,10, 0,0,-100,0,1,0);

	//回転
    glRotatef(*angle, 1.0f, 0, 0.5f);

    //頂点リスト
    glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
    //頂点リスト有効化
    glEnableClientState(GL_VERTEX_ARRAY);
    //テクスチャの指定
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, cubeTexCoords);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, cubeIndices);


}
/*************************************************************************
 * 	png_loadimage
 * 	ファイル名とAssetManagerから、ファイル読み込み
 * 	引数		：　pFilename　： ファイル名
 * 			： 　pAssetManager ：AssetManagerのポインタ
 *************************************************************************/
int test_loadFile(AAssetManager* pAssetManager, const char* pFilename)
{
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
	LOGI("test_loadFile：size %d", size);
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
	for(int i = 0; i < size; i++)
	{
		LOGI("%x,", buf[i]);
	}


	free(buf);
}
/*************************************************************************
 * 	test_writeDataFile
 * 	書き込み可能なデータファイルのオープン
 * 	/data/data/パッケージ名/ファイル名
 * 	引数		：
 *************************************************************************/
#include <fcntl.h>

#include <iostream>
using namespace std;
void test_writeDataFile()
{
	FILE *fp;
	 /* 普通のFOPEN */
	 fp = fopen( "/data/data/com.example.mknGLNativeActivity/file1.txt", "w+" );
	 if( fp == NULL ) {
		 LOGI("fopen: File cannot open error.");
	 }
	 else
	 {
		 LOGI("fopen: success%x.", fp);
		 fputs("HELLO WORLD!\n", fp);
         fflush(fp);
		 fclose(fp);
	 }
	 /* open関数 パーミッション指定なし */
	 int fd = open("/data/data/com.example.mknGLNativeActivity/file2.txt",
	        O_CREAT | O_RDWR);
	 if (fd < 0)
	 {
		 LOGI("open: File cannot open error");
	 }
	 else
	 {
		 LOGI("open: success%x.", fd);
		 write(fd, "HELLO WORLD2!\n", 15);
		 fsync(fd);
		 close(fd);
	 }


	 /* open関数 パーミッション指定=666 */
	 fd = open("/data/data/com.example.mknGLNativeActivity/file3.txt",
	        O_CREAT | O_RDWR, 0666);
	 if (fd < 0)
	 {
		 LOGI("open: 666 File cannot open error");
	 }
	 else
	 {
		 LOGI("open 666: success%x.", fd);
		 write(fd, "HELLO WORLD3!\n", 15);
		 fsync(fd);
		 close(fd);
	 }
	 /* open関数 パーミッション指定=666 */
	 fd = open("/data/data/com.example.mknGLNativeActivity/file4.txt",
	        O_CREAT | O_RDWR, 0777);
	 if (fd < 0)
	 {
		 LOGI("open: 777 File cannot open error");
	 }
	 else
	 {
		 LOGI("open 777: success%x.", fd);
		 write(fd, "HELLO WORLD4!\n", 15);
		 fsync(fd);
		 close(fd);
	 }
}

/*******************************************************************
 * 以下、SampleのDemo.cから取得
 ********************************************************************/
/***********************************************************************
 * glu.h/cがないため、サンプルコードから取得
 */
#define PI 3.1415926535897932f
static void gluLookAt2(GLfloat eyex, GLfloat eyey, GLfloat eyez,
	              GLfloat centerx, GLfloat centery, GLfloat centerz,
	              GLfloat upx, GLfloat upy, GLfloat upz);
static void gluPerspective2(GLfloat fovy, GLfloat aspect,
                           GLfloat zNear, GLfloat zFar);
static void gluPerspective2(GLfloat fovy, GLfloat aspect,
                           GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;

    ymax = zNear * (GLfloat)tan(fovy * PI / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustumx((GLfixed)(xmin * 65536), (GLfixed)(xmax * 65536),
               (GLfixed)(ymin * 65536), (GLfixed)(ymax * 65536),
               (GLfixed)(zNear * 65536), (GLfixed)(zFar * 65536));
}


/* Following gluLookAt implementation is adapted from the
 * Mesa 3D Graphics library. http://www.mesa3d.org
 */
static void gluLookAt2(GLfloat eyex, GLfloat eyey, GLfloat eyez,
	              GLfloat centerx, GLfloat centery, GLfloat centerz,
	              GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;

    /* Make rotation matrix */

    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = (float)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {			/* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }

    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;

    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];

    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];

    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */

    mag = (float)sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }

    mag = (float)sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }

#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    {
        int a;
        GLfixed fixedM[16];
        for (a = 0; a < 16; ++a)
            fixedM[a] = (GLfixed)(m[a] * 65536);
        glMultMatrixx(fixedM);
    }

    /* Translate Eye to Origin */
    glTranslatex((GLfixed)(-eyex * 65536),
                 (GLfixed)(-eyey * 65536),
                 (GLfixed)(-eyez * 65536));
}


