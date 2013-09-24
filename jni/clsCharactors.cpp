/*
 * clsCharactors.cpp
 *
 *  Created on: 2013/08/18
 *      Author: graddy
 */

#include <unistd.h>

#include <clsCharactors.h>
#include <android/log.h>
/* デバッグ用メッセージ */
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "testcodes", __VA_ARGS__))	//! Infomation
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "testcodes", __VA_ARGS__))	//! Warnning
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "testcodes", __VA_ARGS__))	//! Error

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
const GLfloat cubeTexCoords[] = {
		0,0.834, 0.25,0.834, 0.25,1.00, 0,1.00,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1,
		0,0,1,0,1,1,0,1
};*/
const GLfloat cubeTexCoords[] = {
		   0,0.834, 0.25,0.834, 0.25,1.00, 0,1.00,
		0.25,0.834, 0.50,0.834, 0.50,1.00, 0.25,1.00,
		0.50,0.834, 0.75,0.834, 0.75,1.00, 0.50,1.00,
		0.75,0.834, 1.00,0.834, 1.00,1.00, 0.75,1.00,

		   0,0.667, 0.25,0.667, 0.25,0.834,    0,0.834,
		0.25,0.667, 0.50,0.667, 0.50,0.834, 0.25,0.834,
		0.50,0.667, 0.75,0.667, 0.75,0.834, 0.50,0.834,
		0.75,0.667, 1.00,0.667, 1.00,0.834, 0.75,0.834,

		   0,0.834, 0.25,0.834, 0.25,1.00, 0,1.00,
		0.25,0.834, 0.50,0.834, 0.50,1.00, 0.25,1.00,
		0.50,0.834, 0.75,0.834, 0.75,1.00, 0.50,1.00,
		0.75,0.834, 1.00,0.834, 1.00,1.00, 0.75,1.00,

		   0,0.667, 0.25,0.667, 0.25,0.834,    0,0.834,
		0.25,0.667, 0.50,0.667, 0.50,0.834, 0.25,0.834,
		0.50,0.667, 0.75,0.667, 0.75,0.834, 0.50,0.834,
		0.75,0.667, 1.00,0.667, 1.00,0.834, 0.75,0.834,
};

const GLfloat cubeTexCoords2[] = {
		   0,0.167, 0.25,0.167, 0.25,0.334, 0,0.334,
		0.25,0.167, 0.50,0.167, 0.50,0.334, 0.25,0.334,
		0.50,0.167, 0.75,0.167, 0.75,0.334, 0.50,0.334,
		0.75,0.167, 1.00,0.167, 1.00,0.334, 0.75,0.334,

		   0,0.000, 0.25,0.000, 0.25,0.167,    0,0.167,
		0.25,0.000, 0.50,0.000, 0.50,0.167, 0.25,0.167,
		0.50,0.000, 0.75,0.000, 0.75,0.167, 0.50,0.167,
		0.75,0.000, 1.00,0.000, 1.00,0.167, 0.75,0.167,

		   0,0.167, 0.25,0.167, 0.25,0.334, 0,0.334,
		0.25,0.167, 0.50,0.167, 0.50,0.334, 0.25,0.334,
		0.50,0.167, 0.75,0.167, 0.75,0.334, 0.50,0.334,
		0.75,0.167, 1.00,0.167, 1.00,0.334, 0.75,0.334,

		   0,0.000, 0.25,0.000, 0.25,0.167,    0,0.167,
		0.25,0.000, 0.50,0.000, 0.50,0.167, 0.25,0.167,
		0.50,0.000, 0.75,0.000, 0.75,0.167, 0.50,0.167,
		0.75,0.000, 1.00,0.000, 1.00,0.167, 0.75,0.167,
};
/*
 * コンストラクタ
 */
clsCharactor::clsCharactor(void)
{
	int i;
	tickTime = 0;
	imageNum = 0;

	charIndices[0] = 0;
	charIndices[1] = 1;
	charIndices[2] = 2;
	charIndices[3] = 2;
	charIndices[4] = 3;
	charIndices[5] = 0;

	charTexCoords[0] = 0.0f;
	charTexCoords[1] = 0.0f;
	charTexCoords[2] = 1.0f;
	charTexCoords[3] = 0.0f;
	charTexCoords[4] = 1.0f;
	charTexCoords[5] = 1.0f;
	charTexCoords[6] = 0.0f;
	charTexCoords[7] = 1.0f;
	SetMaterialSize(1.0f, 1.0f);


	/* カウンタ初期化 */
	animateTick = 0;
	animatePos = 0;
}
/*
 * コンストラクタ
 * 引数		： name ：キャラクタ名
 * 			：id		：ID
 */
clsCharactor::clsCharactor(const char* name, int id)
{
	clsCharactor();
}
/*
 * デストラクタ
 */
clsCharactor::~clsCharactor(void)
{

}
/*
 *	SetMaterialSize
 *	キャラクタのマテリアルのサイズを設定する
 *	引数	：	width	：幅
 *		：	height	：高さ
 */
bool clsCharactor::SetMaterialSize(float width, float height)
{
	charVertices[0] = 0.0;
	charVertices[1] = 0.0;
	charVertices[2] = 0.0;
	charVertices[3] = width;
	charVertices[4] = 0.0;
	charVertices[5] = 0.0;
	charVertices[6] = width;
	charVertices[7] = height;
	charVertices[8] = 0.0;
	charVertices[9] = 0.0;
	charVertices[10] = height;
	charVertices[11] = 0.0;


	return true;

}
/*
 *	SetCharSize
 *	キャラクタの二次元のサイズを設定する
 *	引数	：	width	：幅
 *		：	height	：高さ
 */
bool clsCharactor::SetCharSize(long wifth, long height)
{

}
/*
 *	AddTexture
 *	テクスチャを追加する
 *	 引数	：	pImage：	テクスチャイメージへのポインタ
 */
int clsCharactor::AddTexture(GLuint texName, int width, int height)
{
	if(imageNum >= C_CLS_CHARA_IMAGE_NUM)
	{
		return false;
	}
	texNames[imageNum] = texName;
	imageNum ++;
	return true;
}
/*
 *	AdddAnimate
 *	アニメーションの追加
 *	引数	：	T.B.D
 */
int clsCharactor::AdddAnimate(void)
{

}
/*
 *	フレーム移動
 *	引数：	経過時間の差分(NanoSec単位)
 */
void clsCharactor::MoveFrame(long tick)
{
	long oldTick = tickTime;
	LOGI("MoveFrame%d", tick);
	tickTime += tick;
	animateTick += tick;
	if(animateTick >= 33333333)
	{
		animatePos ++;
		if(animatePos >= 8)
		{
			animatePos = 0;
		}
		animateTick = 0;
	}
}
/*
 *	Draw
 *	描画
 *	引数：	t.b.d
 */
int clsCharactor::Draw(void)
{

	LOGI("Draw");
	//カメラ位置
	gluLookAt(0,0,10, 0,0,-100,0,1,0);
#if 0

	/* テクスチャ割り当て */
    glBindTexture(GL_TEXTURE_2D, texNames[0]);
	//回転
    glRotatef(0.0f, 1.0f, 0, 0.5f);

    //頂点リスト
    glVertexPointer(3, GL_FLOAT, 0, charVertices);

    //頂点リスト有効化
    glEnableClientState(GL_VERTEX_ARRAY);
    //テクスチャの指定
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, charTexCoords);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, charIndices);

#endif
    static float angle;

    angle +=1.0f;
    if(angle >=360)
    {
    	angle = 0;
    }
    int dm = texNames[0];
    LOGI("Draw = %x", animatePos);
	/* テクスチャ割り当て */
    glBindTexture(GL_TEXTURE_2D, texNames[0]);
	//回転
    glRotatef(angle, 1.0f, 0.25f, 0.5f);

    //頂点リスト
    glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
    //頂点リスト有効化
    glEnableClientState(GL_VERTEX_ARRAY);
    //テクスチャの指定
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, &cubeTexCoords2[8*animatePos]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, cubeIndices);


}
