#include "../Input.h"
#include "../../utility/utility.h"

#include <cmath>

#ifndef NULL
#define NULL 0
#endif

const char* mlInput::TAG = "mlInput";
int		mlInput::M_POINT_MAX = 0;
mlInput::Info*	mlInput::mpInfos = NULL;
float	mlInput::mFlickSensitivity = 50.f;
int		mlInput::mNowTouchCount = 0;

float	mlInput::mPointLength = 0.f;
float	mlInput::mPrevPointLength = 0.f;

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
	jintArray arrayID,
	jfloatArray pointsX, jfloatArray pointsY,
	jfloatArray arrayPressures,
	int eventID, int con)
{
	int pointNum = count;
	if( M_POINT_MAX < pointNum ){ pointNum = M_POINT_MAX;}

//ƒƒ‚ƒŠŠm•Û
	int* ids=env->GetIntArrayElements(arrayID, NULL);
	float* x=env->GetFloatArrayElements(pointsX, NULL);
	float* y=env->GetFloatArrayElements(pointsY, NULL);
	float* pressures = env->GetFloatArrayElements(arrayPressures, NULL);

	for( int i=0; i<pointNum; i++ )
	{
		int index = ids[i];
		Info& info = mpInfos[index];
		info.nextX = x[index];
		info.nextY = y[index];
		info.pressure = pressures[index];
	}

	if( eventID < M_POINT_MAX )
	{
		Info& info = mpInfos[eventID];
		info.condition = con;
		info.isUpdate = true;
	}

//ƒƒ‚ƒŠŠJ•ú
	env->ReleaseIntArrayElements(arrayID,ids,0);
	env->ReleaseFloatArrayElements(pointsX,x,0);
	env->ReleaseFloatArrayElements(pointsY,y,0);
	env->ReleaseFloatArrayElements(arrayPressures,pressures,0);

	//LOGI(TAG, "passage update by send");
}

void mlInput::update(float dt)
{
	static int nextCondition[]={
		MOVE, //DOWN,
		FREE, //UP,
		MOVE, //MOVE,
		FREE, //FREE
	};
	mNowTouchCount = 0;
	for( int i=0; i<M_POINT_MAX; i++ )
	{
		Info& info = mpInfos[i];
		if( !info.isUpdate )
		{
			info.condition = nextCondition[info.condition];
		}

		if( info.condition == DOWN )
		{
			info.mx = 0;
			info.my = 0;
		}
		else
		{
			info.mx = info.nextX - info.x;
			info.my = info.nextY - info.y;
		}
		info.isFlick = ( info.mx*info.mx + info.my*info.my > mFlickSensitivity*mFlickSensitivity );

		info.x = info.nextX;
		info.y = info.nextY;

		info.time += dt;
		if( info.condition == FREE ){
			info.time = 0.f;
			info.mx = 0;
			info.my = 0;
		}
		else
		{
			mNowTouchCount++;
		}

		info.isUpdate = false;
	}

	if( key(1) == MOVE && key(0) == MOVE )
	{
		mPrevPointLength = mPointLength;

		float difX = getX(1) - getX(0);
		float difY = getY(1) - getY(0);

		mPointLength = sqrt( difX*difX + difY*difY );
	}
	else
	{
		mPointLength = mPrevPointLength = 0.f;
	}
}
