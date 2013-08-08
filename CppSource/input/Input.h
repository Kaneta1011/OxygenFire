#ifndef _INPUT_
#define _INPUT_

#include <jni.h>
#include <assert.h>

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
	static void clear();

	static void update(JNIEnv* env, jint count, jintArray arrayID, jfloatArray pointsX, jfloatArray pointsY, jfloatArray arrayPressure, int id, int con);
	static void update(float dt);

public://�悭�g���֐�
	/*
	�^�b�`��Ԃ�Ԃ�
	DOWN : �����ꂽ�Ƃ�
	UP   : �����ꂽ�Ƃ�
	MOVE : ���������Ă���Ƃ�
	FREE : �w������Ă���Ƃ�(�����Ă��Ȃ��Ƃ�)
	*/
	static int key(int id=0){ assert(id<M_POINT_MAX); return mpInfos[id].condition; }
	/*
	�����Ă�����W��X������Ԃ�
	*/
	static float getX(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].x; }
	/*
	�����Ă�����W��Y������Ԃ�
	*/
	static float getY(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].y; }
	/*
	�O�t���[������̈ړ��ʂ�X������Ԃ�
	*/
	static float getMoveX(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].mx; }
	/*
	�O�t���[������̈ړ��ʂ�Y������Ԃ�
	*/
	static float getMoveY(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].my; }
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
	id=0��1�̃|�C���g�̋�����Ԃ�
	*/
	static float getPointLength(){return mPointLength;}

	/*
	id=0��1�̃|�C���g�̋����̕ω��ʂ�Ԃ�
	*/
	static float getPointMoveLength(){return mPointLength-mPrevPointLength;}

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

protected:
	struct Info
	{
		bool isUpdate;
		int condition;
		float x, y;
		float mx, my;//�O�t���[������̈ړ���
		float nextX, nextY;
		float pressure;
		float time;
		bool isFlick;

		void init(){
			this->isUpdate = false;
			this->condition = FREE;
			this->x = this->y = -1;
			this->mx = this->my = 0;
			this->nextX = nextY = -1;
			this->pressure = 0.f;
			this->time = 0.f;
			this->isFlick = false;
		}
	};

protected:
	static int		M_POINT_MAX;
	static Info*	mpInfos;
	static float	mFlickSensitivity;	//�t���b�N���x
	static int		mNowTouchCount;
	static float	mPointLength;
	static float	mPrevPointLength;

private:
	static const char* TAG;
	mlInput(){}
	mlInput(mlInput&){}
	~mlInput(){}
	void operator=(mlInput&){}

};

#endif