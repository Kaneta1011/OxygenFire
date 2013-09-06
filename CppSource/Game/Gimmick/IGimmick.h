#ifndef _IGIMMICK_
#define _IGIMMICK_

#include "Game/IObject/IObject.h"
#include <list>
#include <vector>

#include "GimmickInfo.h"
#include "Game\Bullet\Bullet.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMesh.h"
#endif

namespace rlib
{
	class IGimmick;

	class IGimmickListener
	{
	public:
		virtual void flagOnListener(IGimmick* thiz)=0;
		virtual void flagOffListener(IGimmick* thiz)=0;
	};

	class IGimmick : public IObject, public IGimmickListener
	{
	public:
		enum MESSAGE{
			MSG_NON,
			MSG_DEAD,
		};
	public:
		IGimmick(GimmickInfoBase* info);
		virtual ~IGimmick(){}

		virtual int update()=0;

		/*
		AABBの当たり判定を行いたいときはIGimmick::vs関数を使ってください
		その後の処理は派生側で決めてください
		*/
		virtual bool vs(Bullet* op)=0;

		//当たり判定のための半径取得
		virtual float getRadius(){return this->mRange.x;}

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)=0;
#else
		void debugMessage();
#endif
	//フラグをON・OFFする関数
		void flagOn();
		void flagOff();

	//リスナーの登録・削除
		bool addOnListener(IGimmick* gimmick);
		bool addOffListener(IGimmick* gimmick);
		bool removeOnListener(IGimmick* gimmick);
		bool removeOffListener(IGimmick* gimmick);

	//ゲッター
		const std::string&	getName()const{return this->mName;}
		bool				isFlag() const{return this->mFlag;}
		GIMMICK_TYPE		getType()const{return this->mType;}

	protected:
		std::string			mName;
		bool				mFlag;			//フラグ
		GIMMICK_TYPE		mType;

	private:
		typedef std::vector<IGimmick*>				ListenerContenier;
		typedef ListenerContenier::iterator			Iterator;
		typedef ListenerContenier::const_iterator	ConstIterator;

	private:
		ListenerContenier	mOnListener;
		ListenerContenier	mOffListener;
	};

	//
	//	当たり判定を取るギミックの基底クラス
	//
	class IGimmickObj : public IGimmick
	{
	public:
		IGimmickObj(GGimmickInfo* info);
		virtual ~IGimmickObj(){}

		//当たり判定のための半径取得
		virtual float getRadius(){return this->mRadius;}
	protected:
		float mRadius;
	};
}
#endif