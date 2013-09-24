/*
 * clsCharactors.cpp
 *
 *  Created on: 2013/08/18
 *      Author: graddy
 */

#include <unistd.h>

#include <clsCharactors.h>
#include <android/log.h>
/* �f�o�b�O�p���b�Z�[�W */
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "testcodes", __VA_ARGS__))	//! Infomation
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "testcodes", __VA_ARGS__))	//! Warnning
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "testcodes", __VA_ARGS__))	//! Error

/*
 * �l�p�`�̒��_���X�g
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
 * ���_�`�揇
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
 * �R���X�g���N�^
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


	/* �J�E���^������ */
	animateTick = 0;
	animatePos = 0;
}
/*
 * �R���X�g���N�^
 * ����		�F name �F�L�����N�^��
 * 			�Fid		�FID
 */
clsCharactor::clsCharactor(const char* name, int id)
{
	clsCharactor();
}
/*
 * �f�X�g���N�^
 */
clsCharactor::~clsCharactor(void)
{

}
/*
 *	SetMaterialSize
 *	�L�����N�^�̃}�e���A���̃T�C�Y��ݒ肷��
 *	����	�F	width	�F��
 *		�F	height	�F����
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
 *	�L�����N�^�̓񎟌��̃T�C�Y��ݒ肷��
 *	����	�F	width	�F��
 *		�F	height	�F����
 */
bool clsCharactor::SetCharSize(long wifth, long height)
{

}
/*
 *	AddTexture
 *	�e�N�X�`����ǉ�����
 *	 ����	�F	pImage�F	�e�N�X�`���C���[�W�ւ̃|�C���^
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
 *	�A�j���[�V�����̒ǉ�
 *	����	�F	T.B.D
 */
int clsCharactor::AdddAnimate(void)
{

}
/*
 *	�t���[���ړ�
 *	�����F	�o�ߎ��Ԃ̍���(NanoSec�P��)
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
 *	�`��
 *	�����F	t.b.d
 */
int clsCharactor::Draw(void)
{

	LOGI("Draw");
	//�J�����ʒu
	gluLookAt(0,0,10, 0,0,-100,0,1,0);
#if 0

	/* �e�N�X�`�����蓖�� */
    glBindTexture(GL_TEXTURE_2D, texNames[0]);
	//��]
    glRotatef(0.0f, 1.0f, 0, 0.5f);

    //���_���X�g
    glVertexPointer(3, GL_FLOAT, 0, charVertices);

    //���_���X�g�L����
    glEnableClientState(GL_VERTEX_ARRAY);
    //�e�N�X�`���̎w��
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
	/* �e�N�X�`�����蓖�� */
    glBindTexture(GL_TEXTURE_2D, texNames[0]);
	//��]
    glRotatef(angle, 1.0f, 0.25f, 0.5f);

    //���_���X�g
    glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
    //���_���X�g�L����
    glEnableClientState(GL_VERTEX_ARRAY);
    //�e�N�X�`���̎w��
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, &cubeTexCoords2[8*animatePos]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, cubeIndices);


}
