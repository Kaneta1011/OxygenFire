#include "FusePoint.h"

#include "utility\utility.h"

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
IGimmickObj(&info),
mParent(NULL)
{
}

GFusePoint::~GFusePoint()
{
}

int GFusePoint::update()
{
	if( this->mParent == NULL ){
		ListenerContenier::iterator it = this->mOnListener.begin();
		for( ; it != this->mOnListener.end(); it++ )
		{
			if( (*it)->getType() == eGIMMICK_FUSE ){
				this->mParent = (GFuse*)(*it);
				break;
			}
		}
	}

	return 0;
}

bool GFusePoint::vs(Bullet* op)
{
	return false;
}

void GFusePoint::flagOnListener(IGimmick* thiz)
{
	LOGE("GFusePoint","not create flagOffLisener... | thiz name=\"%s\"", thiz->getName().c_str());
}

void GFusePoint::flagOffListener(IGimmick* thiz)
{
	LOGE("GFusePoint","not create flagOffLisener... | thiz name=\"%s\"", thiz->getName().c_str());
}

#ifndef ANDROID_REDNER
void GFusePoint::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
	IGimmickObj::render(mesh, scale, pipeline);
}

#endif
