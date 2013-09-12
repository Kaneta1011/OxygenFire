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
	case eGIMMICK_DRUM:			make = new GDrum(*this);		break;
	case eGIMMICK_GASOLINE:		make = new GGasoline(*this);	break;
	case eGIMMICK_GARBAGE_BAG:	make = new GGaberageBox(*this); break;
	case eGIMMICK_WOOD_BOX:		make = new GWoodBox(*this);		break;
	case eGIMMICK_CARDBOARD:	make = new GCardBoard(*this);	break;
	case eGIMMICK_ITTOKAN:		make = new GIttokan(*this);		break;
	default:
		LOGE("GExplosionInfo", "GExplosionInfo::makeGimmick() : 爆発物以外のギミックをGExplosionInfoで作ろうとしてます\n");
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
	case eGIMMICK_ITTOKAN:
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
mOxygenCost(info->oxygenCost),
mIsExplosion(false),
mObjBurnInterval(rand()/(float)RAND_MAX * 0.1f + 0.7f),
mEffectBurnInterval(rand()/(float)RAND_MAX * 0.1f + 0.3f)
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
	if( this->isFlag() ) return false;

	if( isOverTemperature() ){
		this->mExplosionCount = rand()%120 + 60;
		this->mExplosionMaxCount = this->mExplosionCount;

		flagOn();

#ifndef ANDROID_REDNER
		if( this->mIsExplosion ){
			sEffectManager->Create(EXPRO, this->mPos, this->getRadius());
		}

		if( !this->wpBurning.IsExist() ){
			this->wpBurning = sEffectManager->Create(FIRE);
			this->wpBurning->Setting_Position(this->mPos);
			this->wpBurning->Setting_Scale(this->mRadius);
			this->wpBurning->Loop();
		}
#endif

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
	bool is = this->mExplosionCount == 0;
#ifndef ANDROID_REDNER
	if( wpBurning.IsExist() ){
		if( is ){
			wpBurning->End();
			LOGI("GExplosion","burning finish!!");
		//最後の煙
			//sEffectManager->Create(BLUE_FIRE, this->mPos);
		}else{
			float rate = this->mExplosionCount / (float)this->mExplosionMaxCount;
			if( rate < this->mEffectBurnInterval ){
				rate /= this->mEffectBurnInterval;
			}else if( rate > 0.95f ) {
				rate = 2.f;
			}else{
				rate = 1.f;
			}
			wpBurning->Setting_Scale( this->getRadius() * rate * 1.5f );
		}
	}
#endif
	return is;
}

#ifndef ANDROID_REDNER
void IGExplosion::render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline)
{
	float rate = 1.f;
	if( this->isFlag() ){
		rate = this->mExplosionCount / (float)this->mExplosionMaxCount;
		if( rate < this->mObjBurnInterval){
			rate =  rate / (this->mObjBurnInterval);
		}else{
			rate = 1.f;
		}
		if( rate > 1.f ) rate = 1.f;
		else if( rate < 0.f ) rate = 0.f;
	}
	IGimmickObj::render(mesh, scale*rate, pipeline);
}
#endif
//================================================================
//
//		GDrumクラス
//
//================================================================
GDrum::GDrum(GExplosionInfo& info):
IGExplosion(&info)
{
	this->mIsExplosion = true;
}

GDrum::~GDrum()
{
}

int GDrum::update()
{
	int msg = udateExplosion();
	if( msg == UPDATE_MSG_EXPLOSION )
	{
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

//================================================================
//
//		GGasolineクラス
//
//================================================================
GGasoline::GGasoline(GExplosionInfo& info):
IGExplosion(&info)
{
	this->mIsExplosion = true;
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

//================================================================
//
//		GIttokanクラス
//
//================================================================
GIttokan::GIttokan(GExplosionInfo& info):
IGExplosion(&info)
{
	this->mIsExplosion = true;
}

GIttokan::~GIttokan()
{
}

int GIttokan::update()
{
	int msg = udateExplosion();
	if( msg == UPDATE_MSG_EXPLOSION ){
		return MSG_DEAD;
	}
	
	return 0;
}

bool GIttokan::vs(Bullet* op)
{
	return IGExplosion::vs(op);
}


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
		return MSG_DEAD;
	}

	return 0;
}

bool GWoodBox::vs(Bullet* op)
{
	return IGExplosion::vs(op);
}

//================================================================
//
//		GCardBoardクラス
//
//================================================================
GCardBoard::GCardBoard(GExplosionInfo& info):
IGExplosion(&info)
{
}

GCardBoard::~GCardBoard()
{
}

int GCardBoard::update()
{
	int msg = udateExplosion();
	if( msg == UPDATE_MSG_EXPLOSION ){
		return MSG_DEAD;
	}

	return 0;
}

bool GCardBoard::vs(Bullet* op)
{
	return IGExplosion::vs(op);
}

//================================================================
//
//		GGaberageBoxクラス
//
//================================================================
GGaberageBox::GGaberageBox(GExplosionInfo& info):
IGExplosion(&info)
{
}

GGaberageBox::~GGaberageBox()
{
}

int GGaberageBox::update()
{
	int msg = udateExplosion();
	if( msg == UPDATE_MSG_EXPLOSION ){
		return MSG_DEAD;
	}

	return 0;
}

bool GGaberageBox::vs(Bullet* op)
{
	return IGExplosion::vs(op);
}
