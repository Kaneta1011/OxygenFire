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
	static int getX(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].x; }
	/*
	押している座標のY成分を返す
	*/
	static int getY(int id=0){assert(id<M_POINT_MAX); return mpInfos[id].y; }
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

public://ゲッター
	/*
	検出するタッチ数の最大数を返す
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