/*
 * clsCharactors.h
 *
 *  Created on: 2013/08/18
 *      Author: graddy
 */

#ifndef CLSCHARACTORS_H_
#define CLSCHARACTORS_H_
#include "type.h"
#include <EGL/egl.h>
#include <GLES/gl.h>
#include "glu.h"
class clsCharactor
{
#define		C_CLS_CHARA_IMAGE_NUM		(8)		/* このキャラクタに使用できる画像の数最大数 */
#define		C_CLS_CHARA_MATERIAL_HEIGHT			(1.0f)		/* このキャラクタの高さ  */
#define		C_CLS_CHARA_MATERIAL_WIDTH			(1.0f)		/* このキャラクタの幅 */

	public:
	    /* コンストラクタ */
		clsCharactor(void);
		clsCharactor(const char* name, int);
		~clsCharactor(void);
		/* メソッド */
		bool SetMaterialSize(float width, float height);
		bool SetCharSize(long wifth, long height);

		int AddTexture( GLuint texName, int width, int height);
		int AdddAnimate(void);				//t.b.d
		void MoveFrame(long tick);
		int Draw(void);						//t.b.d
	private:
		S_POSITION pos;		/* このキャラの２D位置情報 */
		S_POINT	point;		/* このキャラの3D位置情報 */
		/* このキャラクタ向けのテクスチャ管理 */
		int imageNum;
		long tickTime;				/* このキャラクターの時間 */
		GLubyte* pCharactorImages[C_CLS_CHARA_IMAGE_NUM];		/* 画像 */
		GLuint texNames[C_CLS_CHARA_IMAGE_NUM];					/* テクスチャに対しユニークな番号 */
		/* このキャラクタ向けのマテリアル管理 */
		float charVertices[12];									/* 四角形の頂点リスト */
		GLushort charIndices[6];								/* 頂点描画順 */
		GLfloat charTexCoords[8];

		/* アニメーション用 */
		int animateTick;		/* アニメーション用カウンタ */
		int animatePos;
};


#endif /* CLSCHARACTORS_H_ */
