#include "../Input.h"
#include "../../utility/utility.h"
#include "../../utility/debugMessageMng.h"

#include <cmath>

#ifndef NULL
#define NULL 0
#endif

const char* mlInput::TAG = "mlInput";
int		mlInput::M_POINT_MAX = 0;
bool	mlInput::mIsUpdating = false;
mlInput::Info*	mlInput::mpInfos = NULL;
float	mlInput::mFlickSensitivity = 50.f;
int		mlInput::mNowTouchCount = 0;
int		mlInput::mPrevTouchCount = 0;

bool	mlInput::mIsPinch = false;
float	mlInput::mPinchLength = 0.f;
float	mlInput::mPrevPinchLength = 0.f;
float	mlInput::mPincthLengths[2] = {0,0};
float	mlInput::mPrevPinchLengths[2] = {0,0};

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

//メモリ確保
	float* x=env->GetFloatArrayElements(pointsX, NULL);
	float* y=env->GetFloatArrayElements(pointsY, NULL);
	float* pressures = env->GetFloatArrayElements(arrayPressures, NULL);

	//while(mIsUpdating){}

	mIsUpdating = true;

	for( int i=0; i<pointNum; i++ )
	{
		Info& info = mpInfos[i];
		info.prevX = info.x;
		info.prevY = info.y;
		info.x = x[i];
		info.y = y[i];
		info.pressure = pressures[i];
	}

	if( eventID < M_POINT_MAX )
	{
		Info& info = mpInfos[eventID];
		static int order[]={
			1,//DOWN
			0,//UP
			2,//PUSH
			3,//FREE
			-1,//CANCEL
		};//状態の優先度のテーブル
		if( order[action] < order[info.action] )
			info.action = action;

		if( info.action == 4 ){
			info.action == UP;
		}

		if( info.isUpdate )
		{
			info.isUpdate = false;
			if( info.action == DOWN )
			{//押したなら、移動量と1フレーム前の座標を初期化する
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

	mIsUpdating = false;

//メモリ開放
	env->ReleaseFloatArrayElements(pointsX,x,0);
	env->ReleaseFloatArrayElements(pointsY,y,0);
	env->ReleaseFloatArrayElements(arrayPressures,pressures,0);

	//LOGI(TAG, "passage update by send");
}

/*
	次のフレームのための処理を行う
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

	mPrevTouchCount = mNowTouchCount;
	int touchCount = mNowTouchCount;
	for( int i=0; i<touchCount; i++ )
	{
		Info& info = mpInfos[i];
		info.time += dt;
		int oldAction = info.action;
		//if( info.isUpdate ){
			info.action = nextAction[info.action];
		//}
		info.isUpdate = true;

		if( info.action == FIN_UP )
		{//データを無効にして、後ろにあるタッチデータを1つ前につめる
			for( int j=i+1; j<getNowTouchCount(); j++ )
			{
				mpInfos[j-1] = mpInfos[j];
			}
			//最後に現在のタッチ数を1つ減らす
			touchCount--;
			i--;//データをつめたので添え字を1つ戻してつじつまを合わせる
			continue;
		}
		else
		{
			if( oldAction != DOWN )
			{
				info.mx = info.x - info.prevX;
				info.my = info.y - info.prevY;
			}
			info.prevX = info.x;
			info.prevY = info.y;

			//if( info.mx > 50 ) info.mx = 50;
			//if( info.my > 50 ) info.my = 50;
		//フリックチェック
			info.isFlick = ( info.mx*info.mx + info.my*info.my >= mFlickSensitivity*mFlickSensitivity );
		}
 	}
	mNowTouchCount = touchCount;

	updatePinck();

//タッチされていないデータを無効にしておく
	for( int i=getNowTouchCount(); i<M_POINT_MAX; i++ )
	{
		Info& info = mpInfos[i];
		info.init();
	}
	unlock();

}

inline float getPointLength()
{
	float workX = rlib::r2DHelper::convertMoveX( mlInput::getX(1) - mlInput::getX(0) );
	float workY = rlib::r2DHelper::convertMoveY( mlInput::getY(1) - mlInput::getY(0) );
	return rlib::r2DHelper::toUpdateCoord( sqrt(workX*workX + workY*workY) );
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

void mlInput::lock()
{
	mIsUpdating = true;
}

void mlInput::unlock()
{
	mIsUpdating = false;
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

extern "C" {
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_sendTouchEvent(JNIEnv * env, jobject obj, jint count, jfloatArray pointsX, jfloatArray pointsY, jfloatArray arrayPressure, jint id, jint con);
}

//
//		タッチイベントの受信
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_sendTouchEvent(JNIEnv * env, jobject obj, jint count, jfloatArray pointsX, jfloatArray pointsY, jfloatArray arrayPressure, jint id, jint con)
{
	mlInput::update(env, count, pointsX, pointsY, arrayPressure, id, con );
};

