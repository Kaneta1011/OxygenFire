#ifndef _G_RESET_CANDLE_
#define _G_RESET_CANDLE_

#include "IGimmick.h"

namespace rlib
{
	struct GResetCandleInfo : public GGimmickInfo
	{
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

	};

	//
	//	リセットろうそくクラス
	//
	//		弾に当たるとflagOnイベントを発生させ、少しの間燃え始める
	//		火が消えた瞬間flagOffイベントを発生させ、対応しているGCandleクラスに通知する
	//		それと同じく、対応するろうそくの火をつけた順を記憶するGCandleCheckerクラスにもflagOffイベントを飛ばす
	//
	class GResetCandle : public IGimmickObj
	{
	public:
		GResetCandle(GResetCandleInfo* info);
		virtual ~GResetCandle();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);
#endif
	protected:
		int mCount;
#ifndef ANDROID_REDNER
		wp<EffectLib::EmitterSet>	wpFire;
#endif
	};

}

#endif