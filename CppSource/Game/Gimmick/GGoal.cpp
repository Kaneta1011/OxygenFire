#include "GGoal.h"

using namespace rlib;

//===============================================================
//
//		GGoalInfo�N���X
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
