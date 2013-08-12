#include "../Input.h"
#include "../../utility/utility.h"
#include "../../utility/debugMessageMng.h"

#include <cmath>

#ifndef NULL
#define NULL 0
#endif

const char* mlInput::TAG = "mlInput";
int		mlInput::M_POINT_MAX = 0;
bool	mlInput::mIsUpdate = false;
mlInput::Info*	mlInput::mpInfos = NULL;
float	mlInput::mFlickSensitivity = 50.f;
int		mlInput::mNowTouchCount = 0;

bool	mlInput::mIsPinch = false;
float	mlInput::mPinchLength = 0.f;
float	mlInput::mPrevPinchLength = 0.f;

void mlInput::init(int maxPoint)
{
	clear();
	M_POINT_MAX = maxPoint;
	mpInfos = new Info[maxPoint];
	for( int i=0; i<M_POINT_MAX; i++ )
	{
		Info& info = mpInfos[i];
		info.init();
	}
	LOGI(TAG,"Complete init");
}

void mlInput::clear()
{
	if( mpInfos ){ delete[] mpInfos; mpInfos = NULL; }
}

void mlInput::update(
	JNIEnv* env,
	jint count,
	jfloatArray pointsX, jfloatArray pointsY,
	jfloatArray arrayPressures,
	int eventID, int action)
{
	int pointNum = count;
	if( M_POINT_MAX < pointNum ){ pointNum = M_POINT_MAX;}

//�������m��
	float* x=env->GetFloatArrayElements(pointsX, NULL);
	float* y=env->GetFloatArrayElements(pointsY, NULL);
	float* pressures = env->GetFloatArrayElements(arrayPressures, NULL);

	for( int i=0; i<pointNum; i++ )
	{
		Info& info = mpInfos[i];
		info.x = x[i];
		info.y = y[i];
		info.pressure = pressures[i];
	}

	if( eventID < M_POINT_MAX )
	{
		Info& info = mpInfos[eventID];
		info.action = action;
		if( info.isUpdate )
		{
			info.isUpdate = false;
			if( info.action == DOWN )
			{//�������Ȃ�A�ړ��ʂ�1�t���[���O�̍��W������������
				info.mx = info.my = 0.f;
				info.prevX = info.x;
				info.prevY = info.y;
			}
		}
		//switch(action){
		//case DOWN:	LOGI(TAG,"DOWN");		break;
		//case UP:	LOGI(TAG,"UP");			break;
		//case MOVE:	LOGI(TAG,"MOVE");		break;
		//default:	LOGI(TAG,"UNKNOWN");	break;
		//}
	}
	mNowTouchCount = pointNum;

//�������J��
	env->ReleaseFloatArrayElements(pointsX,x,0);
	env->ReleaseFloatArrayElements(pointsY,y,0);
	env->ReleaseFloatArrayElements(arrayPressures,pressures,0);

	//LOGI(TAG, "passage update by send");
}

/*
	���̃t���[���̂��߂̏������s��
*/
void mlInput::update(float dt)
{
	static const int FIN_UP = -2;
	static int nextAction[]={
		MOVE,//DOWN
		FIN_UP,//UP
		MOVE,//MOVE
		FREE,//FREE
	};

	int touchCount = mNowTouchCount;
	for( int i=0; i<touchCount; i++ )
	{
		Info& info = mpInfos[i];
		info.time += dt;
		if( info.isUpdate ){
			info.action = nextAction[info.action];
		}
		info.isUpdate = true;

		if( info.action == FIN_UP )
		{//�f�[�^�𖳌��ɂ��āA���ɂ���^�b�`�f�[�^��1�O�ɂ߂�
			for( int j=i+1; j<getNowTouchCount(); j++ )
			{
				mpInfos[j-1] = mpInfos[j];
			}
			//�Ō�Ɍ��݂̃^�b�`����1���炷
			touchCount--;
			i--;//�f�[�^���߂��̂œY������1�߂��Ă��܂����킹��
			continue;
		}
		else
		{
			info.mx = info.x - info.prevX;
			info.my = info.y - info.prevY;
			info.prevX = info.x;
			info.prevY = info.y;

		//�t���b�N�`�F�b�N
			info.isFlick = ( info.mx*info.mx + info.my*info.my >= mFlickSensitivity*mFlickSensitivity );
		}
 	}
	mNowTouchCount = touchCount;

	updatePinck();

//�^�b�`����Ă��Ȃ��f�[�^�𖳌��ɂ��Ă���
	for( int i=getNowTouchCount(); i<M_POINT_MAX; i++ )
	{
		Info& info = mpInfos[i];
		info.init();
	}
}

inline float getPointLength()
{
	float workX = mlInput::getX(1) - mlInput::getX(0);
	float workY = mlInput::getY(1) - mlInput::getY(0);
	return sqrt(workX*workX + workY*workY);
}

void mlInput::updatePinck()
{
	if( mlInput::mIsPinch )
	{
		if( getNowTouchCount() != 2 )
		{
			mIsPinch = false;
			mPinchLength = mPrevPinchLength = 0.f;
		}
		else
		{
			mPrevPinchLength = mPinchLength;
			mPinchLength = getPointLength();
		}
	}
	else
	{
		if( getNowTouchCount() == 2 )
		{
			mIsPinch = true;
			mPinchLength = getPointLength();
			mPrevPinchLength = mPinchLength;
		}
	}
}

void mlInput::debugMseeage()
{
	DEBUG_MSG_NON_ARAG("touch infomation");
	DEBUG_MSG("now touchNum = %d", mlInput::getNowTouchCount());

	if( mlInput::isPinch() )
	{
		DEBUG_MSG_NON_ARAG("pinch");
		DEBUG_MSG("pinch len = %.2f", mlInput::getPinchLength());
		DEBUG_MSG("pinch move len = %.2f", mlInput::getPinchMoveLength());
	}
	else
	{
		DEBUG_MSG_NON_ARAG("non pinch");
	}

	for( int i=0; i<mlInput::getNowTouchCount(); i++ )
	{
		DEBUG_MSG("id = %d", i);
		switch(mlInput::key(i))
		{
		case mlInput::DOWN:		DEBUG_MSG_NON_ARAG("DOWN");	break;
		case mlInput::UP:		DEBUG_MSG_NON_ARAG("UP");	break;
		case mlInput::MOVE:		DEBUG_MSG_NON_ARAG("MOVE");	break;
		case mlInput::FREE:		DEBUG_MSG_NON_ARAG("FREE");	break;
		}
		DEBUG_MSG("x = %.2f\ty = %.2f", mlInput::getX(i), mlInput::getY(i));
		DEBUG_MSG("time = %.2f[ms]", mlInput::getTime(i));
		if( mlInput::isFlick() )
		{
			DEBUG_MSG_NON_ARAG("Flick!!");
		}
		else
		{
			DEBUG_MSG_NON_ARAG("Non flick..");
		}
	}
}