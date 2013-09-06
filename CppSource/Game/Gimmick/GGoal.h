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
}

#endif
