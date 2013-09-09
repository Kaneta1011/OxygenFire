#ifndef _FUSE_
#define _FUSE_

#include "IGimmick.h"

namespace rlib
{
	//
	//	導火線クラス
	//		GFusePointクラスからのイベントに処理は依存している
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

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
		Vector3 mStart, mEnd;

	};
}

#endif
