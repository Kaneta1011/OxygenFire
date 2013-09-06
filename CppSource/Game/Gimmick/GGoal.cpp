#include "GGoal.h"

using namespace rlib;

//===============================================================
//
//		GGoalInfoƒNƒ‰ƒX
//
//===============================================================
void GGoalInfo::forFile(textWriter& writer)
{
	GGimmickInfo::forFile(writer);
}

bool GGoalInfo::loadParam(textLoader& loader)
{
	return GGimmickInfo::loadParam(loader);
}

IGimmick* GGoalInfo::makeGimmick()
{
	return NULL;
}
