#ifndef _G_CANDLE_
#define _G_CANDLE_

#include "IGimmick.h"

namespace rlib
{
	struct GCandleInfo : public GGimmickInfo
	{
		Vector3 color;

		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

	};

	//
	//	ろうそくクラス
	//
	//		弾に当たると燃え始める
	//		燃えた瞬間、flagOnイベントを発生させる
	//
	//		対応するGResetCandleクラスからflagOffイベントが来たら火は消える
	//
	class GCandle : public IGimmickObj
	{
	public:
		GCandle(GCandleInfo* info);
		virtual ~GCandle();

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