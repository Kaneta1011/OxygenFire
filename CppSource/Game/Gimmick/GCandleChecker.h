#ifndef _G_CNADLE_CHECKER_
#define _G_CNADLE_CHECKER_

#include "IGimmick.h"

namespace rlib
{
	struct GCandleCheckerInfo : public GimmickInfoBase
	{
		std::string correctAnswer;	//正解の順序

		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();

		GCandleCheckerInfo():correctAnswer(""){}
		virtual ~GCandleCheckerInfo(){}

		static std::string NON_DATA;
	};

	//
	//	ろうそくチェッカークラス
	//
	//	正しい順序にろうそくの火をつけたか見るクラス
	//	成功したならflagOnイベントを発生させる
	//
	//	GCandleクラスからflagOnイベントが送られてきたら、正解の順序かチェックする
	//
	class GCandleChecker : public IGimmick
	{
	public:
		GCandleChecker(GCandleCheckerInfo* info);
		virtual ~GCandleChecker();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);
#endif

	protected:
		int mNowCheckIndex;			//間違えたらmCorrectAnswerの文字数以上の値になる
		std::string mCorrectAnswer;
	};
}

#endif