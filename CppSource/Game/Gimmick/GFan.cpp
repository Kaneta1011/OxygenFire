#include "GFan.h"

#include "utility\textWriter.h"
#include "utility\utility.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

#include "GraphicsLib\Class\r2DObj\r2DObj.h"

using namespace EffectLib;

#endif

using namespace rlib;

//=======================================================================
//
//		G2DInfoクラス
//
//=======================================================================
void GFanInfo::forFile(textWriter&  w)
{
	GGimmickInfo::forFile(w);
}

bool GFanInfo::loadParam(textLoader& loader)
{
	if( ! GGimmickInfo::loadParam(loader) )
	{
		return false;
	}
	return true;
}

IGimmick* GFanInfo::makeGimmick()
{
	GFan* set = new GFan(this);
	return set;
}

//=======================================================================
//
//		GFanクラス
//
//=======================================================================
GFan::GFan(GFanInfo* info):
	IGimmickObj(info)
{
}

GFan::~GFan()
{
}

int GFan::update()
{
	return MSG_NON;
}

bool GFan::vs(Bullet* op)
{
	return IGimmickObj::vs(op);
}

void GFan::flagOnListener(IGimmick* thiz)
{
}

void GFan::flagOffListener(IGimmick* thiz)
{
}

#ifndef ANDROID_REDNER
void GFan::render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline)
{
}
#endif
