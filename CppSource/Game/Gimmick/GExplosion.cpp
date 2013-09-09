#include "GExplosion.h"

#include "utility\textWriter.h"
#include "utility\utility.h"

#ifndef ANDROID_REDNER
#include "EffectLib\Effect.h"
using namespace EffectLib;
#endif

using namespace rlib;

//================================================================
//
//		GExplosionInfoクラス
//
//================================================================
void GExplosionInfo::forFile(textWriter& w)
{
	GGimmickInfo::forFile(w);

	w.nestIn();w.writeNest();
	w.write("oxygenCost\t").write(this->oxygenCost).br();
	w.write("limitTemperature\t").write(this->limitTemperature);
	w.nestOut();
	w.br();
}

bool GExplosionInfo::loadParam(textLoader& loader)
{
	if( ! GGimmickInfo::loadParam(loader) )
	{
		if( strcmp("oxygenCost",loader.tmpBuf) == 0 ){
			this->oxygenCost = loader.LoadInt();
			return true;
		}else if( strcmp("limitTemperature",loader.tmpBuf) == 0 ){
			this->limitTemperature = loader.LoadInt();
			return true;
		}
		return false;
	}
	return true;
}

IGimmick* GExplosionInfo::makeGimmick()
{
	IGimmick* make = NULL;
	switch( this->type )
	{
	case eGIMMICK_DRUM:			make = new GDrum(*this); break;
	case eGIMMICK_GASOLINE:		make = new GGasoline(*this); break;
	case eGIMMICK_GARBAGE_BAG:	LOGE("GExplosionInfo", "please create garbage bag class\n"); break;
	case eGIMMICK_WOOD_BOX:		make = new GWoodBox(*this); break;
	case eGIMMICK_CARDBOARD:	LOGE("GExplosionInfo", "please create cardboard class\n");	break;
	default:
		LOGE("GExplosionInfo", "爆発物以外のギミックをGExplosionInfoで作ろうとしてます\n");
	}
	return make;
}

//================================================================
//
//		IGExplosionクラス
//
//================================================================
bool IGExplosion::isExplosion(GIMMICK_TYPE type)
{
	switch(type)
	{
	case eGIMMICK_DRUM:
	case eGIMMICK_GASOLINE:
	case eGIMMICK_GARBAGE_BAG:
	case eGIMMICK_WOOD_BOX:
	case eGIMMICK_CARDBOARD:
		return true;
	default:
		return false;
	}
}

IGExplosion::IGExplosion(GExplosionInfo* info):
IGimmickObj(info),
mTemperature(0),
mLimitTemperature(info->limitTemperature),
mExplosionCount(-1),
mOxygenCost(info->oxygenCost)
{
}

bool IGExplosion::vs(Bullet* op)
{
	if( IGimmick::vs(op) )
	{
		this->mTemperature += op->getTemperature();
		return true;
	}
	return false;
}

void IGExplosion::flagOnListener(IGimmick* thiz)
{
//爆発物だったら一緒に爆発する
	if( isExplosion(thiz->getType()) )
	{
		this->mTemperature = this->mLimitTemperature+2;
	}
	else
	{
		LOGE("IGExplosion", "please make flagOnListener type=%d | thiz->type=%d", this->getType(), thiz->getType());
	}
}

void IGExplosion::flagOffListener(IGimmick* thiz)
{
	LOGE("IGExplosion", "please make flagOffListener type=%d | thiz->type=%d", this->getType(), thiz->getType());
}

int IGExplosion::udateExplosion()
{
	if( updateExplosionCount() )	return UPDATE_MSG_EXPLOSION;
	if( updateTemperature() )		return UPDATE_MSG_OVER_TEMPERATURE;
	return UPDATE_MSG_NON;
}

bool IGExplosion::updateTemperature()
{
	if( isOverTemperature() ){
		this->mExplosionCount = rand()%60 - (this->mTemperature-this->mLimitTemperature);
		if( this->mExplosionCount < 0 )
			this->mExplosionCount = 1;

		return true;
	}

	this->mTemperature--;
	if( this->mTemperature < 0 ) this->mTemperature = 0;
	return false;
}

bool IGExplosion::updateExplosionCount()
{
	if( this->mExplosionCount < 0 ) return false;

	this->mExplosionCount--;
	return this->mExplosionCount == 0;
}

//================================================================
//
//		GDrumクラス
//
//================================================================
GDrum::GDrum(GExplosionInfo& info):
IGExplosion(&info)
{
}

GDrum::~GDrum()
{
}

int GDrum::update()
{
	int msg = udateExplosion();
	if( msg == UPDATE_MSG_EXPLOSION )
	{
#ifndef ANDROID_REDNER
		sEffectManager->Create(BLUE_FIRE, this->mPos);
#endif
		flagOn();
		return MSG_DEAD;
	}
	return 0;
}

bool GDrum::vs(Bullet* op)
{
	if( IGExplosion::vs(op) )
	{
		if( isOverTemperature() )
		{//弾では爆発させない
			this->mTemperature = this->mLimitTemperature-1;
		}
		return true;
	}
	return false;
}

#ifndef ANDROID_REDNER
void GDrum::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
	IGimmickObj::render(mesh, scale, pipeline);
}
#endif

//================================================================
//
//		GWoodBoxクラス
//
//================================================================
GGasoline::GGasoline(GExplosionInfo& info):
IGExplosion(&info)
{
}

GGasoline::~GGasoline()
{
}

int GGasoline::update()
{
	int msg = udateExplosion();
	if( msg == UPDATE_MSG_EXPLOSION ){
		return MSG_DEAD;
	}

	return 0;
}

bool GGasoline::vs(Bullet* op)
{
	return IGExplosion::vs(op);
}

#ifndef ANDROID_REDNER
void GGasoline::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
	IGimmickObj::render(mesh, scale, pipeline);
}
#endif

//================================================================
//
//		GWoodBoxクラス
//
//================================================================
GWoodBox::GWoodBox(GExplosionInfo& info):
IGExplosion(&info)
{
}

GWoodBox::~GWoodBox()
{
}

int GWoodBox::update()
{
	int msg = udateExplosion();
	if( msg == UPDATE_MSG_EXPLOSION ){
#ifndef ANDROID_REDNER
		sEffectManager->Create(BLUE_FIRE, this->mPos);
#endif
		flagOn();
		return MSG_DEAD;
	}

	return 0;
}

bool GWoodBox::vs(Bullet* op)
{
	return IGExplosion::vs(op);
}

#ifndef ANDROID_REDNER
void GWoodBox::render(klib::kMesh* mesh, float scale, klib::kGraphicsPipline* pipeline)
{
	IGimmickObj::render(mesh, scale, pipeline);
}
#endif
