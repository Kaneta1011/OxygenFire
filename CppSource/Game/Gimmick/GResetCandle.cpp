#include "GResetCandle.h"

#include "utility\textWriter.h"
#include "utility\utility.h"

using namespace rlib;

//=======================================================================
//
//		GFusePointInfoクラス
//
//=======================================================================
void GResetCandleInfo::forFile(textWriter&  w)
{
	GGimmickInfo::forFile(w);
}

bool GResetCandleInfo::loadParam(textLoader& loader)
{
	if( ! GGimmickInfo::loadParam(loader) )
	{
		return false;
	}
	return true;
}

IGimmick* GResetCandleInfo::makeGimmick()
{
	GResetCandle* set = new GResetCandle(this);
	return set;
}

//====================================================================
//
//		リセットろうそくクラス
//
//====================================================================
GResetCandle::GResetCandle(GResetCandleInfo* info):
	IGimmickObj(info),
	mCount(0)
{
}

GResetCandle::~GResetCandle()
{
}

int GResetCandle::update()
{
	if( isFlag() ){
		this->mCount++;
		if( this->mCount > 30 ){
			flagOff();
			this->mCount=0;
		}
	}
	return MSG_NON;
}

bool GResetCandle::vs(Bullet* op)
{
	bool ret = false;
	if( IGimmickObj::vs(op) ){
		flagOn();
		ret = true;
	}
	return ret;
}

void GResetCandle::flagOnListener(IGimmick* thiz)
{

}

void GResetCandle::flagOffListener(IGimmick* thiz)
{
}

#ifndef ANDROID_REDNER
void GResetCandle::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
	IGimmickObj::render(mesh, scale, pipeline);
}
#endif
