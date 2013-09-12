#ifndef _G_GOAL_
#define _G_GOAL_

#include "IGimmick.h"

namespace rlib
{
	struct GGoalInfo : public GGimmickInfo
	{
		virtual void forFile(textWriter& writer);
		virtual bool loadParam(textLoader& loader);

		virtual IGimmick* makeGimmick();
	};

	//===========================================================
	//
	//ゴール
	//・flagOnListenerまたはflagOffListenerのどちらかが呼ばれたら扉が開きます
	//・なので、イベントの設定には気をつけてください
	//・フラグがtrueなら扉が開き、近づいたらクリアーできる
	//・フラグがfalseなら扉は閉まっている
	//
	//===========================================================
	class GGoal : public IGimmickObj
	{
	public:
		GGoal(GGoalInfo& info);
		virtual ~GGoal();

		virtual int update();
		virtual bool vs(Bullet* op);

		virtual void flagOnListener(IGimmick* thiz);
		virtual void flagOffListener(IGimmick* thiz);

#ifndef ANDROID_REDNER
		virtual void render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline);
#endif
	private:
#ifndef ANDROID_REDNER
		klib::kSkin*	mpSkin;
#endif
	};
}

#endif
