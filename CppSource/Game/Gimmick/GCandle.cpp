#include "GCandle.h"

#include "utility\textWriter.h"
#include "utility\utility.h"

using namespace rlib;

//=======================================================================
//
//		GFusePointInfoクラス
//
//=======================================================================
void GCandleInfo::forFile(textWriter&  w)
{
	GGimmickInfo::forFile(w);

	w.nestIn();w.writeNest();
	w.write("color\t");
	writeVec3(this->color, w);
	w.nestOut();
	w.br();
}

bool GCandleInfo::loadParam(textLoader& loader)
{
	if( ! GGimmickInfo::loadParam(loader) )
	{
		if( strcmp("color",loader.tmpBuf) == 0 ){
			this->color.x = loader.LoadFloat();
			this->color.y = loader.LoadFloat();
			this->color.z = loader.LoadFloat();
			return true;
		}
		return false;
	}
	return true;
}

IGimmick* GCandleInfo::makeGimmick()
{
	GCandle* set = new GCandle(this);
	return set;
}

//====================================================================
//
//		ろうそくクラス
//
//====================================================================
GCandle::GCandle(GCandleInfo* info):
	IGimmickObj(info)
{
}

GCandle::~GCandle()
{
}

int GCandle::update()
{
	return MSG_NON;
}

bool GCandle::vs(Bullet* op)
{
	bool ret = false;
	if( IGimmickObj::vs(op) ){
		flagOn();
		ret = true;
	}
	return ret;
}

void GCandle::flagOnListener(IGimmick* thiz)
{

}

void GCandle::flagOffListener(IGimmick* thiz)
{
}


#ifndef ANDROID_REDNER
void GCandle::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
	IGimmickObj::render(mesh, scale, pipeline);
}
#endif
