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
#define		C_CLS_CHARA_IMAGE_NUM		(8)		/* ���̃L�����N�^�Ɏg�p�ł���摜�̐��ő吔 */
#define		C_CLS_CHARA_MATERIAL_HEIGHT			(1.0f)		/* ���̃L�����N�^�̍���  */
#define		C_CLS_CHARA_MATERIAL_WIDTH			(1.0f)		/* ���̃L�����N�^�̕� */

	public:
	    /* �R���X�g���N�^ */
		clsCharactor(void);
		clsCharactor(const char* name, int);
		~clsCharactor(void);
		/* ���\�b�h */
		bool SetMaterialSize(float width, float height);
		bool SetCharSize(long wifth, long height);

		int AddTexture( GLuint texName, int width, int height);
		int AdddAnimate(void);				//t.b.d
		void MoveFrame(long tick);
		int Draw(void);						//t.b.d
	private:
		S_POSITION pos;		/* ���̃L�����̂QD�ʒu��� */
		S_POINT	point;		/* ���̃L������3D�ʒu��� */
		/* ���̃L�����N�^�����̃e�N�X�`���Ǘ� */
		int imageNum;
		long tickTime;				/* ���̃L�����N�^�[�̎��� */
		GLubyte* pCharactorImages[C_CLS_CHARA_IMAGE_NUM];		/* �摜 */
		GLuint texNames[C_CLS_CHARA_IMAGE_NUM];					/* �e�N�X�`���ɑ΂����j�[�N�Ȕԍ� */
		/* ���̃L�����N�^�����̃}�e���A���Ǘ� */
		float charVertices[12];									/* �l�p�`�̒��_���X�g */
		GLushort charIndices[6];								/* ���_�`�揇 */
		GLfloat charTexCoords[8];

		/* �A�j���[�V�����p */
		int animateTick;		/* �A�j���[�V�����p�J�E���^ */
		int animatePos;
};


#endif /* CLSCHARACTORS_H_ */
