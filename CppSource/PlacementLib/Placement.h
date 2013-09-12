#ifndef __tePlacement_H__
#define __tePlacement_H__
#include	"math\\kmathf.h"
#include	"utility/textLoader.h"
#include	<iostream>
#include	"StandardLib/SmartPointer.h"
#include	<string>

namespace PlacementLib{
using namespace klib::math;
const static int STR_LENGTH = 128;

//==========================================================
//	ここの構造体を受け皿にしてデータをもらって各種設定
//	*基本的に上田君のみ使用することになる
enum ePLACEMENT_TYPE
{
	PLACEMENT_BOX,			//	箱(初期テスト用)
	PLACEMENT_GarbageBag,	//	ゴミ袋
	PLACEMENT_DRUM,			//	ドラム
	PLACEMENT_GASOLINE,		//	ガソリン
	PLACEMENT_WoodenBox,	//	木箱
	PLACEMENT_CARDBOARD,	//	ダンボール
	PLACEMENT_FAN,			//	扇風機
	PLACEMENT_CANDLE,		//	ろうそく
	PLACEMENT_2D,			//	2D画像
	PLACEMENT_GOAL,			//	ゴール
	PLACEMENT_RESET_CANDLE,	//　リセットろうそく
	PLACEMENT_ITTOKAN,		//	いっとかん
	PLACEMENT_TYPE_NUM,
};

struct PlacementData
{
	int Num;
	sp<std::string> spStr;
	sp<ePLACEMENT_TYPE> spType;
	sp<Vector3> spPos;
	sp<Vector3> spScale;
	sp<Vector3> spAngle;
};
struct GimmickLine
{
	int Num;
	sp<std::string> spStr;
	sp<Vector3> spStart;
	sp<Vector3> spEnd;
};
struct WindData
{
	int Num;
	sp<std::string> spStr;
	sp<Vector3> spPos;
	sp<Vector3> spScale;
	sp<Vector3> spAngle;
	sp<Vector3> spWindVec;	//1秒間に進む距離用のベクトル
};
//==========================================================
struct Data;
/* -------------------------------------------------------
	class PlacementManager

	１．Load(char* MQO_Filename);
	関数をつかってオブジェクトの全情報を取得

	２．基本配置物(Characterや箱など)
	int Num = オブジェクトの最大数
	Vector3 spPos = 座標の配列
	Vector3 spScale = サイズの配列
	Vector3 spAngle = アングルの配列
	この情報をGetBox(sp<PlacementData>* spData);
	関数で取得できるのであとは受け取り手が
	Num でループまわして各種オブジェクトに
	座標、スケール、アングルを取得してください

	３．Delete();
	ステージの終わりなどオブジェクトの情報がいらなくなるとき
	この関数をよんでください。あたらしいステージで新しい情報を
	呼び込むようにしてください

 ------------------------------------------------------- */
class PlacementManager
{
public:
	//	MQOの配置データを全て取り込む
	void Load(char* MQO_Filename);
	//	データの解放
	void Delete();
	//	データを転送
	void GetPlayer(sp<PlacementData>* spData);	//これ使わんと下つかって
	std::string GetPlayerStr();

	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	・上田君用
	基本的使い方は説明したけどわからないときは考えずにすぐに聞いて!

	*/
	void GetBox(sp<PlacementData>* spData);
	void GetWind(sp<WindData>* spData);
	void GetGimmickWind(sp<WindData>* spData);
	void GetGimmickLine(sp<GimmickLine>* spData);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	・吉原君用
	プレイヤーは単体やから別で取得作った
	よしはら君のプレイヤーはこれで初期設定OK
	*/
	Vector3 GetPlayerPos();			
	Vector3 GetPlayerScale();
	Vector3 GetPlayerAngle();
	/*	使い方( これだけ )
	pos = sPlacementManager->GetPlayerPos();
	scale = sPlacementManager->GetPlayerScale();
	angle = sPlacementManager->GetPlayerAngle();
	*/
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++






	//=============================================
	//=============================================
	//			↓　この下以降は気にしなくてOK
	//=============================================
	//=============================================
	PlacementManager(){Clear();}void Clear();
	~PlacementManager(){Destroy();}void Destroy();
private:
	void Create_AllData();
	void Create_Data(sp<PlacementData>* spUserData,sp<Data> spData); 
	void Setting_AllData(const char* File);
	void Setting_Data(sp<char> Name,sp<char> Load);
	void Setting_Data(sp<Data> spData);
	void Setting_Data_ScaleHalf(sp<Data> spData);
	void Setting_Data_GimmickLine(sp<Data> spData);
	void Setting_WindData(sp<Data> spData);
	void Setting_ObjectName(sp<char> Name,sp<char> Load);
	void Setting_CreateObjectName(sp<char> Name,sp<char> Load);
	void Add_ObjectNum(sp<char> Name,sp<char> Load);
	void Search_Num(const char* File);
	sp<Data> m_spPlayerData;
	sp<Data> m_spBoxData;
	sp<Data> m_spWindData;
	sp<Data> m_spGimmickWindData;
	sp<Data> m_spGimmickLineData;
	sp<textLoader> tl;
};


struct Data
{
	int Num;
	int NowNum;
	char Name[STR_LENGTH];
	sp<std::string> spStr;
	sp<ePLACEMENT_TYPE> spType; 
	sp<Vector3> spPos;
	sp<Vector3> spScale;
	sp<Vector3> spAngle;
	sp<Vector3>	spWindVec;
	sp<Vector3> spStart;
	sp<Vector3> spEnd;
	Data(char* Name):Num(0),NowNum(0){
		strcpy(this->Name,Name);
	}
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	Particle_Singleton
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class PlacementManager_Singleton : public PlacementManager
{
protected:
	static PlacementManager_Singleton* singleton;

	PlacementManager_Singleton(){}
	virtual ~PlacementManager_Singleton(){}
public:
	static PlacementManager_Singleton* getInstance()
	{
		if( singleton == NULL ){
			singleton = new PlacementManager_Singleton;
		}
		return singleton;
	}
	static void deleteInstance(){
		if( singleton != NULL ){
			delete singleton;

			singleton = NULL;
		}
	}
};

#define sPlacementManager ( PlacementManager_Singleton::getInstance() )

}//PlacementLib

#endif