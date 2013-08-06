#include "../Input.h"
#include "../../utility/utility.h"

#ifndef NULL
#define NULL 0
#endif

const char* mlInput::TAG = "mlInput";
int		mlInput::M_POINT_MAX = 0;
mlInput::Info*	mlInput::mpInfos = NULL;

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
		info.x = x[index];
		info.y = y[index];
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

	for( int i=0; i<M_POINT_MAX; i++ )
	{
		Info& info = mpInfos[i];
		if( !info.isUpdate )
		{
			info.condition = nextCondition[info.condition];
		}
		info.time += dt;
		if( info.condition == FREE ){
			info.time = 0.f;
		}

		info.isUpdate = false;
	}
}
