#ifndef _G_CNADLE_CHECKER_
#define _G_CNADLE_CHECKER_

#include "IGimmick.h"

namespace rlib
{
	struct GCandleCheckerInfo : public GGimmickInfo
	{
		Vector3 color;

		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

	};

	//
	//	ろうそくチェッカークラス
	//
	//	正しい順序にろうそくの火をつけたか見るクラス
	//	成功したならflagOnイベントを発生させる
	//
	class GCandleChecker : public IGimmickObj
	{
	public:
		GCandleChecker(GCandleCheckerInfo* info);
		virtual ~GCandleChecker();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

	protected:
	};
}

#endif