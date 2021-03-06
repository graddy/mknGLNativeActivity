/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#define _CPO_DANGOMUSHI_
#define CFG_DRAW_CHARACTOR

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <math.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/asset_manager.h>
#include "png_loader.h"
#include "testcodes.h"
#include "clsCharactors.h"
#ifdef _CPO_DANGOMUSHI_
#include "clsDangomushi.h"
#endif

/* デバッグ用メッセージ */
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "mknGLnative-activity", __VA_ARGS__))	//! Infomation
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "mknGLnative-activity", __VA_ARGS__))	//! Warnning
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "mknGLnative-activity", __VA_ARGS__))	//! Error

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

/**
 * Shared state for our app.
 * アプリケーション内共通データ
 */
struct engine {
	/* Androidアプリの情報 */
    struct android_app* app;
    /* センサー系の情報 */
    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;				//! アニメーション中
    /* OpenGLESの情報*/
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    /* 画面サイズ */
    int32_t width;
    int32_t height;
    struct saved_state state;

    float angle[2];

    long tickTime;
};


#if 0	//別定義
void gluPerspective(double fovy, double aspect, double zNear, double zFar) {
    GLfloat xmin, xmax, ymin, ymax;
    ymax = zNear * tan(fovy * M_PI / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}
#endif
#define LENGTH (15)
short triangleBuffer[] = {
/*        X                Y          Z */
-LENGTH / 2, -LENGTH / 2, 0, LENGTH - LENGTH / 2, -LENGTH / 2, 0, -LENGTH
        / 2, LENGTH - LENGTH / 2, 0, LENGTH - LENGTH / 2, LENGTH - LENGTH
        / 2, 0, };

float colorBuffer[] = {
/*   R    G    B    A  */
1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.5, 0.5, 0.0,
        1.0, };
///////////////

#ifdef _CPO_DANGOMUSHI_
clsDangomushi* pTestChara = NULL;
#else
clsCharactor* pTestChara = NULL;
#endif
void initializeScene(struct android_app* state);
int initializeTextures(struct android_app* state, GLuint* pTexName, int* pWidth, int* pHeight);
void initializeCharactors(struct android_app* state);
clsCharactor* createCharactor( int texNum, GLuint* pTexName, int width, int height);
void mknTest(struct android_app* state);

/**
 * 表示の初期化
 */
void initBox(struct engine* engine) {

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glClearColor(.7f, .7f, .9f, 1.f);
    glShadeModel(GL_SMOOTH);

    float ratio = (float) engine->width / (float) engine->height;
    glViewport(0, 0, (int) engine->width, (int) engine->height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, ratio, 0.1, 100);
}

static float angle = 0;
void drawBox(void) {
    // 表示の初期化(毎フレーム)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -50.0);

    // 四角を回転
    angle = (angle + 2);
    if (angle > 360)
        angle = 0;
    glRotatef(angle, angle, 0, 1.0f);

    // 描画する
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_SHORT, 0, triangleBuffer);
    glColorPointer(4, GL_FLOAT, 0, colorBuffer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

}
///////////////
/**
 * Initialize an EGL context for the current display.
 * EGL初期化
 */
static int engine_init_display(struct engine* engine) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     * 有効にするEGLパラメータ
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;
    /* ディスプレイを取得し初期化 */
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    LOGI("ScreenSize: width=%d height=%d", w, h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;
    engine->tickTime = 0;
#if 0	/* 2d */
    // Initialize GL state.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
#endif
#if 0	//box
    //mkn add
    // ボックス表示の初期化
    initBox(engine);
#endif
#ifdef CFG_DRAW_CUBE			//cube
    initCube((engine->app));
#endif
    mknTest((engine->app));
#ifdef CFG_DRAW_CHARACTOR
    initializeScene((engine->app));
    initializeCharactors((engine->app));
#endif
    return 0;
}

/**
 * Just the current frame in the display.
 * フレーム描画
 */
static void engine_draw_frame(struct engine* engine) {
    if (engine->display == NULL) {
        // No display.
        return;
    }
    /* 時間の取得 */
	struct timespec now;
	long nowTick;
	clock_gettime(CLOCK_MONOTONIC, &now);
	nowTick = (long)(now.tv_sec*1000000000LL + now.tv_nsec);

#if 0	//box

    // Just fill the screen with a color.
    glClearColor(((float)engine->state.x)/engine->width, engine->state.angle,
            ((float)engine->state.y)/engine->height, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    //mkn add
    drawBox();
#endif
#ifdef CFG_DRAW_CUBE			//cube
    prepareFrame(engine->width, engine->height);
    drawCube(&(engine->state.angle));
    eglSwapBuffers(engine->display, engine->surface);
#endif

#ifdef CFG_DRAW_CHARACTOR
    long tickTime;
    tickTime = nowTick - engine->tickTime;
    /* 描画準備 */
    prepareFrame(engine->width, engine->height);


    /* MoveとDraw分けるべしt.b.d */

    pTestChara->Touch(engine->state.x, engine->state.y);

    pTestChara->MoveFrame(tickTime);

    /* 描画 */
    pTestChara->Draw();

    eglSwapBuffers(engine->display, engine->surface);
    engine->tickTime = nowTick;

    LOGI("Draw %d, %d", engine->state.x, engine->state.y);

#endif

}

/**
 * Tear down the EGL context currently associated with the display.
 * Displayの破棄
 */
static void engine_term_display(struct engine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 * 入力イベントの処理
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
    	/* タッチ入力 */
        if(engine->animating == 0){
        	engine->animating = 1;
        }
        else{
        	engine->animating = 0;
        }
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);

        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:		//状態保存要求
        	LOGI("APP_CMD_SAVE_STATE");
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:		//ウィンドウ初期化
        	LOGI("APP_CMD_INIT_WINDOW");
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:		//ウインドウ破棄
        	LOGI("APP_CMD_TERM_WINDOW");
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:		//アプリがフォーカス取得
        	LOGI("APP_CMD_GAINED_FOCUS");
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
            break;
        case APP_CMD_LOST_FOCUS:		//アプリがフォーカスを失ったとき
        	LOGI("APP_CMD_LOST_FOCUS");
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            engine_draw_frame(engine);
            break;
    }
}
/*
 * リソースファイルの読み込みテスト
 */
void mknTest(struct android_app* state)
{
	LOGI("mknTest");
	test_loadFile(state->activity->assetManager, "readTest.txt");
	test_writeDataFile();
#if 0	/*pngload test*/
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
#endif
}
/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    struct engine engine;

    // Make sure glue isn't stripped.
    app_dummy();
    //ユーザーデータ作成
    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    // Prepare to monitor accelerometer
    engine.sensorManager = ASensorManager_getInstance();
    //加速度センサー
    engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
            state->looper, LOOPER_ID_USER, NULL, NULL);
    /* 前回の状態が保存されているなら、復帰させる */
    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }
    /* AssetManagerのリソース読み濃いテスト */
//    testcodes_assetResource_Test(state);



    // loop waiting for stuff to do.
    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER) {
                if (engine.accelerometerSensor != NULL) {
                    ASensorEvent event;
                    while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
                            &event, 1) > 0) {
//                        LOGI("accelerometer: x=%f y=%f z=%f",
  //                              event.acceleration.x, event.acceleration.y,
    //                            event.acceleration.z);
                    }
                }
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
            	//終了要求
                engine_term_display(&engine);
                return;			//終わる
            }
        }
        //アニメーションさせる
        if (engine.animating) {
            // Done with events; draw next animation frame.
            engine.state.angle += 1.0f;
            if (engine.state.angle > 360) {
                engine.state.angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame(&engine);
        }
    }
}
//END_INCLUDE(all)



/*
 * シーンの初期化
 */
void initializeScene(struct android_app* state)
{
	LOGI("initializeScene");
	//OpenGLの食器か
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	//ブレンドモードの設定
	//アルファブレンド
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//ブレンド有効
	glEnable(GL_BLEND);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}
/*
 * テクスチャーの初期化
 */
int initializeTextures(struct android_app* state, GLuint* pTexName, int* pWidth, int* pHeight)
{
    png_uint_32 width, height;
    GLint type;
    GLuint texName[1];
    int textureNum;
    GLubyte* pTextureImage[1];
    /* テクスチャ番号の取得 */
    glGenTextures(1, &texName[0]);
    for(textureNum = 0; textureNum < 1; textureNum++)
    {
		*pTexName = texName[textureNum];
		glBindTexture(GL_TEXTURE_2D, texName[textureNum]);
		/* テクスチャ読み込み */
		png_loadimage(state->activity->assetManager, "test.png", &width, &height,&type, &pTextureImage[0]);
		*pWidth = width;
		*pHeight = height;
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pTextureImage[0]);
		free(pTextureImage[0]);
    }
	return textureNum;
}
/*
 * キャラクタの初期化
 */
void initializeCharactors(struct android_app* state)
{
	int width[1], height[1];
	GLuint texName[1];
    int textureNum = 1;
    /* テクスチャを展開する */
    textureNum = initializeTextures(state, texName, width, height);
#ifdef _CPO_DANGOMUSHI_
    pTestChara = (clsDangomushi*)createCharactor(textureNum, texName, width[1], height[1]);
#else
    pTestChara = createCharactor(textureNum, texName, width[1], height[1]);
#endif
}
/*
  *	createCharactor
 *	キャラクタを作成する
 *	引数	：
 */
clsCharactor* createCharactor( int texNum, GLuint* pTexName, int width, int height)
{
	clsCharactor* pChara;

	/* クラス作成 */
	pChara = new clsCharactor();

	/* 初期化  */
	pChara->Initialize();
	/* モデル作成 t.b.d*/
//	pChara->createModel();
	/* テクスチャの読み込みと設定 */
	pChara->AddTexture( *pTexName, width, height);

	return pChara;
}
