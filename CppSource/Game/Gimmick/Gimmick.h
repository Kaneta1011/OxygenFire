#ifndef _GIMMICK_
#define _GIMMICK_

#include "Game/IObject/IObject.h"
#include "utility\utility.h"
#include "utility\SinglyLinkedList.h"
#include "StandardLib\SmartPointer.h"
#include <list>
#include <vector>

#include "GimmickInfo.h"
#include "IGimmick.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMesh.h"
#endif

namespace rlib
{
	//===============================================================================
	//
	//		GimmickManagerクラス
	//		・シングルトン
	//
	//===============================================================================
	class GimmickManager
	{
	public:
	//ギミックの種類を判別するための関数
		//爆発物か?(ドラム缶、木箱、ダンボール等)
		static bool isExplosion(GIMMICK_TYPE type);
		//風か?
		static bool isWindType(GIMMICK_TYPE type);
		//導火線か？
		static bool isFuse(GIMMICK_TYPE type);
		//導火線の両端か？プレイヤーが近づいたらメッセージを出します
		static bool isFusePoint(GIMMICK_TYPE type);
		//メッセージを出すタイプか？
		static bool isShowMessegeType(GIMMICK_TYPE type);
		//ぶつかるタイプか？
		static bool isHitGimmick(GIMMICK_TYPE type);
		//ゴールギミックか？
		static bool isGoalGimmick(GIMMICK_TYPE type);

	public:
		static GimmickManager& getInst(){
			static GimmickManager inst;
			return inst;
		}

	public:
		typedef std::vector<IGimmick*> ListType;
		typedef ListType::iterator Iterator;
		typedef ListType::const_iterator ConstIterator;

	public:
		enum MESSAGE
		{
			MSG_NON,
		};

	public:
		void init(const char* giFilePath);
		void clear();
		void clearData();

		int update();

		//
		//・posからrangeの範囲にいるギミックをリストにして返します
		//・メッセージを表示するギミックだけがほしい場合は「getNearShowMessageGimmick()」を利用してください
		//・リストにあるギミックの種類が知りたい場合は「IGimmick::getType()」を使用してください
		//・「IGimmick::getType()」が返す値は「GimmickInfo.h」にありますのでそちらを参照してください
		//
		std::list<IGimmick*> getNearGimmick(const klib::math::Vector3& pos, float range);
		//
		//・posからrangeの範囲にいるギミック内、メッセージを表示するギミックを返します
		//・種類関係なく一定距離にあるギミックがほしい場合は「getNearGimmick()」を利用してください
		//・リストにあるギミックの種類が知りたい場合は「IGimmick::getType()」を使用してください
		//・「IGimmick::getType()」が返す値は「GimmickInfo.h」にありますのでそちらを参照してください
		//
		std::list<IGimmick*> getNearShowMessageGimmick(const klib::math::Vector3& pos, float range);

		//
		//・ギミックとの当たり判定を行う
		//・球VS球を採用している
		//・戻り値のy成分にはposのy成分が必ず入る	
		//
		klib::math::Vector3 collision(const klib::math::Vector3& pos, float range);

		//
		//	・posが受ける風の力を返す
		//
		klib::math::Vector3 calWindPower(const klib::math::Vector3& pos, float range);

		//
		//	ゴールしたか？
		//	・ゴール判定はギミックで行う
		//
		bool isGoal(const klib::math::Vector3& pos);

#ifndef ANDROID_REDNER
		void render();
#endif
	public:
		Iterator begin(){return this->mData.begin();}
		ConstIterator begin()const{return this->mData.begin();}

		Iterator end(){ return this->mData.end(); }
		ConstIterator end()const{return this->mData.end(); }

		unsigned int size(){return this->mData.size();}

	private:
		void remove(unsigned int index);

	private:
		enum MESH_TYPE
		{
			eMESH_DRUM,
			eMESH_GASOLINE,
			eMESH_WOOD_BOX,
			eMESH_GABERAGE_BOX,
			eMESH_CARD_BOARD,
			eMESH_RESET_CANDLE,
			eMESH_TYPE_NUM
		};

	private:
#ifndef ANDROID_REDNER
		void loadMeshes();
		klib::kMesh* getMesh( int type, float* outUnitScale );
#endif
	private:
		ListType mData;

#ifndef ANDROID_REDNER
		sp<klib::kMesh*> mpMeshies;
#endif
	private:
		GimmickManager();
		~GimmickManager();

	};

	#define GIMMICK_MNG rlib::GimmickManager::getInst()
}
#endif