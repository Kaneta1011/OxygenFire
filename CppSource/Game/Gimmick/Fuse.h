#ifndef _FUSE_
#define _FUSE_

#include "IGimmick.h"

namespace rlib
{
	struct GLineInfo : public GimmickInfoBase
	{
		klib::math::Vector3 start;
		klib::math::Vector3 end;
		float speed;

		void convert(GimmickLine* data, int index);
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

		GLineInfo():speed(1.f){}
	};

	//
	//	導火線クラス
	//		GFusePointクラスからのイベントに処理は依存している
	//		導火線を移動しているとき、現在どこにいるかを知りたい場合はgetPos()を使ってください
	//		弾とは当たり判定しない
	//
	class GFuse : public IGimmick
	{
	public:
		GFuse(GLineInfo& info);
		virtual ~GFuse();

		virtual int update();
		/*
		弾とは当たり判定しない
		*/
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

		/*
		・始点を返す
		・始点の位置をあらかじめ初期化などで覚えておくことはしないでください
		・始点の位置は対応しているGFusePointの処理の結果に依存しています
		・これは終点も同じです
		*/
		const Vector3& getStart()const{return this->mStart; }
		/*
		・終点を返す
		・終点の位置をあらかじめ初期化などで覚えておくことはしないでください
		・終点の位置は対応しているGFusePointの処理の結果に依存しています
		・これは始点も同じです
		*/
		const Vector3& getEnd()const{ return this->mEnd; }
		/*
		・燃え尽きたか？
		*/
		bool isBurnOut()const{return this->mIsBurnOut; }

		void setPos(Vector3& pos){ this->mPos = pos; }

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
		bool	mIsBurnOut;		//燃え尽きたか？
		Vector3 mStart, mEnd;
		float	mSpeed;
	};
}

#endif
