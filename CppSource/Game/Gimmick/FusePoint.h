#ifndef _FUSE_POINT_
#define _FUSE_POINT_

#include "IGimmick.h"
#include "Fuse.h"

namespace rlib
{
	struct GFusePointInfo : public GGimmickInfo
	{
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

	};

	//===========================================================
	//
	//	導火線の両端
	//		燃え移りたい場合はflagOn()をつかってからgetFuse()を呼び出してください
	//		enableBurning()で1度通った導火線かを判断できます(trueで可能)
	//
	//		一度燃え移ったら2度とメッセージを出さない？
	//		それともエラーメッセージ的なものをだす？
	//
	//		決して燃えない
	//
	//===========================================================
	class GFusePoint : public IGimmickObj
	{
	public:
		GFusePoint(GFusePointInfo& info);
		virtual ~GFusePoint();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

		void setFuse(GFuse* fuse){ this->mParent = fuse; }

		//
		//・対応しているGFuseを返す
		//・使う前に必ず、flagOn()を呼び出してください
		//
		GFuse*	getFuse(){ return this->mParent; }

		/*
		燃え移ることが可能か？
		*/
		bool enableBurning()const{ return !this->mParent->isBurnOut(); }

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
		GFuse* mParent;
	};
}

#endif