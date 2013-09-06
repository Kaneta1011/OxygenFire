#include "Wind.h"

using namespace rlib;

bool GWind::isWindType(GIMMICK_TYPE type)
{
	return type == eGIMMICK_WIND;
}

GWind::GWind(GWindInfo& info):
IGimmick(&info)
{
	this->mPos = info.pos;
	this->mVelocity = info.dir;
	this->mRange = info.scale;
}

GWind::~GWind()
{
}

int GWind::update()
{
//風の強さを変異させる
	return 0;
}

bool GWind::vs(Bullet* op)
{
	if( IGimmick::vs(op) ){
	//現在の風の強さを弱める
	}
	return false;
}

void GWind::flagOnListener(IGimmick* thiz)
{
}

void GWind::flagOffListener(IGimmick* thiz)
{
}

#ifndef ANDROID_REDNER
void GWind::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
}
#endif
