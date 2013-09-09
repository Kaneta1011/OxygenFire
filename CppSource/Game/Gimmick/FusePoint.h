#ifndef _FUSE_POINT_
#define _FUSE_POINT_

#include "IGimmick.h"

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
	//		プレイヤーが近づいたら導火線に燃え移るかを聞くメッセージを表示する
	//		聞いた結果、燃え移るなら対応している導火線にflagOnイベントを通知する
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

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
	};
}

#endif