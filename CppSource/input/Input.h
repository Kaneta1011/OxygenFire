#ifndef _INPUT_
#define _INPUT_

#include <jni.h>
#include <assert.h>

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
	static void clear();

	static void update(JNIEnv* env, jint count, jintArray arrayID, jfloatArray pointsX, jfloatArray pointsY, jfloatArray arrayPressure, int id, int con);
	static void update(float dt);

public://よく使う関数
	/*
	タッチ状態を返す
	DOWN : 押されたとき
	UP   : 離されたとき
	MOVE : 押し続けているとき
	FREE : 指が離れているとき(押していないとき)
	*/
	static int key(int id=0){ assert(id<M_POINT_MAX); return mpInfos[id].condition; }
	/*
	押している座標のX成分を返す
	*/
	static float getX(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].x; }
	/*
	押している座標のY成分を返す
	*/
	static float getY(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].y; }
	/*
	前フレームからの移動量のX成分を返す
	*/
	static float getMoveX(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].mx; }
	/*
	前フレームからの移動量のY成分を返す
	*/
	static float getMoveY(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].my; }
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
	id=0と1のポイントの距離を返す
	*/
	static float getPointLength(){return mPointLength;}

	/*
	id=0と1のポイントの距離の変化量を返す
	*/
	static float getPointMoveLength(){return mPointLength-mPrevPointLength;}

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

protected:
	struct Info
	{
		bool isUpdate;
		int condition;
		float x, y;
		float mx, my;//前フレームからの移動量
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
	static float	mFlickSensitivity;	//フリック感度
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