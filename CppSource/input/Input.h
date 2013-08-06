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
	static int getX(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].x; }
	/*
	�����Ă�����W��Y������Ԃ�
	*/
	static int getY(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].y; }
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

public://�Q�b�^�[
	/*
	���o����^�b�`���̍ő吔��Ԃ�
	*/
	static int	getMaxPoint(){return M_POINT_MAX;}

protected:
	struct Info
	{
		bool isUpdate;
		int condition;
		float x, y;
		float pressure;
		float time;

		void init(){
			this->isUpdate = false;
			this->condition = FREE;
			this->x = -1;
			this->y = -1;
			this->pressure = 0.f;
			this->time = 0.f;
		}
	};

protected:
	static int		M_POINT_MAX;
	static Info*	mpInfos;

private:
	static const char* TAG;
	mlInput(){}
	mlInput(mlInput&){}
	~mlInput(){}
	void operator=(mlInput&){}

};

#endif