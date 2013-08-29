#ifndef _INPUT_
#define _INPUT_

#include <jni.h>
#include <assert.h>
#include "GraphicsLib\Class\r2DObj\r2DRenderer.h"

/*
	・画面タッチと仮想ボタンを管理するクラス
	・画面タッチの機能として
		----複数タッチの管理	->	getNowTouchCount()
		----座標と前フレームからの移動量	-> getX() getY() getMoveX() getMoveY()
		----押した時間		->　getTime()
		----圧力			->	getPressure()
		----フリック関連	->	isFlick() (フリック方向がほしいときはgetMove?()を利用してください)
		----ピンチ関連		->	isPinch() getPinchLength() getPinchMoveLength()
	などを用意しています
	・仮想ボタンはまだです
*/
class mlInput
{
public:
	enum CONDITION
	{//順番はAndroidSDKに依存しているので変えないように
		DOWN,
		UP,
		MOVE,
		FREE
	};
public:
	/*
	引数：
		maxPoint : タッチを検出する最大数. ハード側の最大検出個数は大体4個超えるぐらいなのでそれを超える値を渡してもメモリの無駄になります.
	*/
	static void init(int maxPoint);
	/*
	開放処理
	*/
	static void clear();

	/*
	Javaからタッチデータを取ってくる関数
	Javaから呼び出される
	*/
	static void update(JNIEnv* env, jint count, jfloatArray pointsX, jfloatArray pointsY, jfloatArray arrayPressure, int id, int con);

	/*
	次のフレームのための処理を行う
	*/
	static void update(float dt);

public://よく使う関数
	/*
	タッチ状態を返す
	DOWN : 押されたとき
	UP   : 離されたとき
	MOVE : 押し続けているとき
	FREE : 指が離れているとき(押していないとき) この状態だとその他(座標やフリック関係など)の全ぶの値は無意味なものなので使わないで
	*/
	static int key(int id=0){ assert(id<M_POINT_MAX); return mpInfos[id].action; }
	/*
	押している座標のX成分を返す(プロジェクション座標系(-100～100の範囲))
	*/
	static float getX(int id=0){assert(id<M_POINT_MAX); return rlib::r2DHelper::toUpdateCoord( rlib::r2DHelper::convertPosX( mpInfos[id].x ) ); }
	/*
	押している座標のY成分を返す(プロジェクション座標系(-100～100の範囲))
	*/
	static float getY(int id=0){assert(id<M_POINT_MAX); return rlib::r2DHelper::toUpdateCoord( rlib::r2DHelper::convertPosY( mpInfos[id].y ) ); }
	/*
	前フレームからの移動量のX成分を返す(プロジェクション座標系(-100～100の範囲))
	*/
	static float getMoveX(int id=0){assert(id<M_POINT_MAX); return rlib::r2DHelper::toUpdateCoord( rlib::r2DHelper::convertMoveX( mpInfos[id].mx ) ); }
	/*
	前フレームからの移動量のY成分を返す(プロジェクション座標系(-100～100の範囲))
	*/
	static float getMoveY(int id=0){assert(id<M_POINT_MAX); return rlib::r2DHelper::toUpdateCoord( rlib::r2DHelper::convertMoveY( mpInfos[id].my ) ); }

	/*
	フリックしたか？
	*/
	static bool isFlick(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].isFlick; }

	/*
	タッチしてからの経過時間を返す
	タッチしていなければ０を返す
	*/
	static float getTime(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].time; }
	/*
	タッチの圧力を返す
	1.fで最大で、0.fで触ってない
	機種依存の機能で、圧力センサーがないものは1.fか0.fしか返さない
	*/
	static float getPressure(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].pressure; }

	/*
	現在のタッチ数を返す
	*/
	static int getNowTouchCount(){return mNowTouchCount;}

	/*
	前フレームのタッチ数を返す
	*/
	static int getPrevTouchCount(){return mPrevTouchCount;}

	/*
	ピンチ状態か？
	*/
	static bool isPinch(){return mIsPinch;}
	/*
	id=0と1のポイントの距離を返す(プロジェクション座標系(0～200ぐらいの範囲))
	*/
	static float getPinchLength(){return mPinchLength;}

	/*
	id=0と1のポイントの距離の変化量を返す(プロジェクション座標系(0～200ぐらいの範囲))
	*/
	static float getPinchMoveLength(){return mPinchLength-mPrevPinchLength;}

public://ゲッター
	/*
	検出するタッチ数の最大数を返す
	*/
	static int	getMaxPoint(){return M_POINT_MAX;}

	/*
	フリック感度を設定する
	*/
	static void  setFlickSensitivity(float sensitivity){ mFlickSensitivity = sensitivity; }
	/*
	現在のフリック感度を返す
	*/
	static float getFlickSensitivity(){ return mFlickSensitivity; }

public://デバッグ用
	static void debugMseeage();

protected:
	struct Info
	{
		bool isUpdate;
		int action;
		float x, y;
		float mx, my;//前フレームからの移動量
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
	static float	mFlickSensitivity;	//フリック感度
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

#endif