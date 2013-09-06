#ifndef _WIND_
#define _WIND_

#include "IGimmick.h"

namespace rlib
{
	//
	//	ステージ内の風クラス
	//		基本的にAABBの領域内にいたら風を吹かせる
	//		弾が範囲内に入ったら、風を弱める
	//
	class GWind : public IGimmick
	{
	public:
		static bool isWindType(GIMMICK_TYPE type);
	public:
		GWind(GWindInfo& info);
		~GWind();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline);
#endif

	protected:
	};
}

#endif