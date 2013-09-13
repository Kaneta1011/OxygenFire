#ifndef _INPUT_
#define _INPUT_

#include <jni.h>
#include <assert.h>
#include "GraphicsLib\Class\r2DObj\r2DRenderer.h"
#include "templateLib\kVector.h"

/*
�E��ʃ^�b�`�Ɖ��z�{�^�����Ǘ�����N���X
�E��ʃ^�b�`�̋@�\�Ƃ���
----�����^�b�`�̊Ǘ�	->	getNowTouchCount()
----���W�ƑO�t���[������̈ړ���	-> getX() getY() getMoveX() getMoveY()
----����������		->�@getTime()
----����			->	getPressure()
----�t���b�N�֘A	->	isFlick() (�t���b�N�������ق����Ƃ���getMove?()�𗘗p���Ă�������)
----�s���`�֘A		->	isPinch() getPinchLength() getPinchMoveLength()
�Ȃǂ�p�ӂ��Ă��܂�
�E���z�{�^���͂܂��ł�
*/
class mlInput
{
public:
	enum CONDITION
	{//���Ԃ�AndroidSDK�Ɉˑ����Ă���̂ŕς��Ȃ��悤��
		DOWN,
		UP,
		MOVE,
		FREE
	};
public:
	/*
	�����F
	maxPoint : �^�b�`�����o����ő吔. �n�[�h���̍ő匟�o���͑��4�����邮�炢�Ȃ̂ł���𒴂���l��n���Ă��������̖��ʂɂȂ�܂�.
	*/
	static void init(int maxPoint);
	/*
	�J������
	*/
	static void clear();

	/*
	Java����^�b�`�f�[�^������Ă���֐�
	Java����Ăяo�����
	*/
	static void update(JNIEnv* env, jint count, jfloatArray pointsX, jfloatArray pointsY, jfloatArray arrayPressure, int id, int con);

	/*
	���̃t���[���̂��߂̏������s��
	*/
	static void update(float dt);

public://�悭�g���֐�
	/*
	�^�b�`��Ԃ�Ԃ�
	DOWN : �����ꂽ�Ƃ�
	UP   : �����ꂽ�Ƃ�
	MOVE : ���������Ă���Ƃ�
	FREE : �w������Ă���Ƃ�(�����Ă��Ȃ��Ƃ�) ���̏�Ԃ��Ƃ��̑�(���W��t���b�N�֌W�Ȃ�)�̑S�Ԃ̒l�͖��Ӗ��Ȃ��̂Ȃ̂Ŏg��Ȃ���
	*/
	static int key(int id=0){ assert(id<M_POINT_MAX); return mpInfos[id].action; }
	/*
	�����Ă�����W��X������Ԃ�(�v���W�F�N�V�������W�n(-100�`100�͈̔�))
	*/
	static float getX(int id=0){assert(id<M_POINT_MAX); return rlib::r2DHelper::toUpdateCoord( rlib::r2DHelper::convertPosX( mpInfos[id].x ) ); }
	/*
	�����Ă�����W��Y������Ԃ�(�v���W�F�N�V�������W�n(-100�`100�͈̔�))
	*/
	static float getY(int id=0){assert(id<M_POINT_MAX); return rlib::r2DHelper::toUpdateCoord( rlib::r2DHelper::convertPosY( mpInfos[id].y ) ); }
	/*
	�O�t���[������̈ړ��ʂ�X������Ԃ�(�v���W�F�N�V�������W�n(-100�`100�͈̔�))
	*/
	static float getMoveX(int id=0){assert(id<M_POINT_MAX); return rlib::r2DHelper::toUpdateCoord( rlib::r2DHelper::convertMoveX( mpInfos[id].mx ) ); }
	/*
	�O�t���[������̈ړ��ʂ�Y������Ԃ�(�v���W�F�N�V�������W�n(-100�`100�͈̔�))
	*/
	static float getMoveY(int id=0){assert(id<M_POINT_MAX); return rlib::r2DHelper::toUpdateCoord( rlib::r2DHelper::convertMoveY( mpInfos[id].my ) ); }

	/*
	�t���b�N�������H
	*/
	static bool isFlick(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].isFlick; }

	/*
	�^�b�`���Ă���̌o�ߎ��Ԃ�Ԃ�
	�^�b�`���Ă��Ȃ���΂O��Ԃ�
	*/
	static float getTime(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].time; }
	/*
	�^�b�`�̈��͂�Ԃ�
	1.f�ōő�ŁA0.f�ŐG���ĂȂ�
	�@��ˑ��̋@�\�ŁA���̓Z���T�[���Ȃ����̂�1.f��0.f�����Ԃ��Ȃ�
	*/
	static float getPressure(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].pressure; }

	/*
	���݂̃^�b�`����Ԃ�
	*/
	static int getNowTouchCount(){return mNowTouchCount;}

	/*
	�O�t���[���̃^�b�`����Ԃ�
	*/
	static int getPrevTouchCount(){return mPrevTouchCount;}

	/*
	�s���`��Ԃ��H
	*/
	static bool isPinch(){return mIsPinch;}
	/*
	id=0��1�̃|�C���g�̋�����Ԃ�(�v���W�F�N�V�������W�n(0�`200���炢�͈̔�))
	*/
	static float getPinchLength(){return mPinchLength;}

	/*
	id=0��1�̃|�C���g�̋����̕ω��ʂ�Ԃ�(�v���W�F�N�V�������W�n(0�`200���炢�͈̔�))
	*/
	static float getPinchMoveLength(){return mPinchLength-mPrevPinchLength;}

public://�Q�b�^�[
	/*
	���o����^�b�`���̍ő吔��Ԃ�
	*/
	static int	getMaxPoint(){return M_POINT_MAX;}

	/*
	�t���b�N���x��ݒ肷��
	*/
	static void  setFlickSensitivity(float sensitivity){ mFlickSensitivity = sensitivity; }
	/*
	���݂̃t���b�N���x��Ԃ�
	*/
	static float getFlickSensitivity(){ return mFlickSensitivity; }

public:
	static void lock();
	static void unlock();

public://�f�o�b�O�p
	static void debugMseeage();

protected:
	struct Info
	{
		bool isUpdate;
		int action;
		float x, y;
		float mx, my;//�O�t���[������̈ړ���
		float prevX, prevY;
		float pressure;
		float time;
		bool isFlick;

		void init(){
			this->isUpdate = false;
			this->action = FREE;
			this->x = this->y = -1;
			this->mx = this->my = 0;
			this->prevX = prevY = -1;
			this->pressure = 0.f;
			this->time = 0.f;
			this->isFlick = false;
		}
	};

protected:
	static void updatePinck();

protected:
	static int		M_POINT_MAX;
	static bool		mIsUpdating;
	static Info*	mpInfos;
	static float	mFlickSensitivity;	//�t���b�N���x
	static int		mNowTouchCount;
	static int		mPrevTouchCount;

	static bool		mIsPinch;
	static float	mPinchLength;
	static float	mPrevPinchLength;
	static float	mPincthLengths[2];
	static float	mPrevPinchLengths[2];

private:
	static const char* TAG;
	mlInput(){}
	mlInput(mlInput&){}
	~mlInput(){}
	void operator=(mlInput&){}

};

struct PrevTouch
{
	klib::f32 time;
	int sx,sy;
	PrevTouch():time(0.0f),sx(0),sy(0){}
};

typedef klib::ktl::kVector<PrevTouch> PrevTouchTable;
static PrevTouchTable m_PrevTouchTable;

static int getTouch(int* sx,int* sy)
{
	//�^�b�`���o8�t���[���ȓ��ɗ����΃^�b�`�Ƃ݂Ȃ�

	int touchIndex=-1;
	for(int i=0;i<mlInput::getMaxPoint();i++)
	{
		if(mlInput::getTime(i)<FEQ_EPS)
		{
			if(0.0f<m_PrevTouchTable(i).time && m_PrevTouchTable(i).time<8.0f*16.666666f)
			{
				touchIndex=i;
				*sx=m_PrevTouchTable(i).sx;
				*sy=m_PrevTouchTable(i).sy;
			}
		}
		m_PrevTouchTable(i).time=mlInput::getTime(i);
		m_PrevTouchTable(i).sx=mlInput::getX(i);
		m_PrevTouchTable(i).sy=mlInput::getY(i);
	}
	return touchIndex;
}

#endif