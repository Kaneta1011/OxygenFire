#include "FusePoint.h"

using namespace rlib;

//=======================================================================
//
//		GFusePointInfoクラス
//
//=======================================================================
void GFusePointInfo::forFile(textWriter&  w)
{
	GGimmickInfo::forFile(w);
}

bool GFusePointInfo::loadParam(textLoader& loader)
{
	if( ! GGimmickInfo::loadParam(loader) )
	{
		return false;
	}
	return true;
}

IGimmick* GFusePointInfo::makeGimmick()
{
	GFusePoint* set = new GFusePoint(*this);
	return set;
}

//=======================================================================
//
//		GFusePointクラス
//
//=======================================================================
GFusePoint::GFusePoint(GFusePointInfo& info):
IGimmickObj(&info)
{
}

GFusePoint::~GFusePoint()
{
}

int GFusePoint::update()
{
	return 0;
}

bool GFusePoint::vs(Bullet* op)
{
	return false;
}

void GFusePoint::flagOnListener(IGimmick* thiz)
{
}

void GFusePoint::flagOffListener(IGimmick* thiz)
{
}

#ifndef ANDROID_REDNER
void GFusePoint::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
	mesh->setPosition(this->mPos);
	mesh->setAngle(this->mAngle);
	mesh->setScale(this->mRange * scale);
	mesh->Update();
	mesh->Render(pipeline);
}

#endif
