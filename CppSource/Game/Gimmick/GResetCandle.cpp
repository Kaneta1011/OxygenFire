#include "GResetCandle.h"

#include "utility\textWriter.h"
#include "utility\utility.h"

using namespace rlib;
#ifndef ANDROID_REDNER
using namespace EffectLib;
#endif

//=======================================================================
//
//		GResetCandleInfoクラス
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
#ifndef ANDROID_REDNER
	if( wpFire.IsExist() ){
		wpFire->End();
	}
#endif
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
#ifndef ANDROID_REDNER
	if( isFlag() ){
		if( !wpFire.IsExist() ){
			wpFire = sEffectManager->Create(FIRE, this->mPos);
			wpFire->Loop();
		}
	}else{
		if( wpFire.IsExist() ){
			wpFire->End();
		}
	}
#endif

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
