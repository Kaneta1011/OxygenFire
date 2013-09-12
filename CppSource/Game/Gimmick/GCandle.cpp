#include "GCandle.h"

#include "utility\textWriter.h"
#include "utility\utility.h"

#ifndef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
using namespace EffectLib;

#endif

using namespace rlib;

//=======================================================================
//
//		GCandleInfoクラス
//
//=======================================================================
void GCandleInfo::forFile(textWriter&  w)
{
	GGimmickInfo::forFile(w);
}

bool GCandleInfo::loadParam(textLoader& loader)
{
	if( ! GGimmickInfo::loadParam(loader) )
	{
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
#ifndef ANDROID_REDNER
	,mpMesh(NULL)
#endif
{
	size_t size = this->mName.size()-1;
	this->mID = this->mName[size];
	LOGI("GCandle","GCandle::GCandle() : id = %c | name = \"%s\"\n", this->mID, this->mName.c_str());
#ifndef ANDROID_REDNER
	loadMesh(this->mID);
#endif
}

#ifndef ANDROID_REDNER
void GCandle::loadMesh(char ID)
{
	std::string path = "gimmick/candle/candle";
	path += ID;
	path += ".IMO";
	LOGI("GCandle","GCandle::loadMesh() : load mesh | path=\"%s\"", path.c_str());
	if( this->mpMesh ){ delete this->mpMesh; this->mpMesh = NULL; }
	this->mpMesh = new klib::kMesh(path.c_str(), new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
}
#endif

GCandle::~GCandle()
{
#ifndef ANDROID_REDNER
	if( this->mpMesh ){ delete this->mpMesh; this->mpMesh = NULL; }
	if( wpFire.IsExist() ){
		wpFire->End();
	}
#endif
}

int GCandle::update()
{
#ifndef ANDROID_REDNER
	if( isFlag() ){
		if( !wpFire.IsExist() ){
			wpFire = sEffectManager->Create(FIRE, this->mPos, this->getRadius()*0.1f);
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
	if( thiz->getType() == eGIMMICK_RESET_CANDLE ){
	//リセットろうそくからイベントが送られたら火を消す
		flagOff();
		LOGI("GCandle","GCandle::flagOffListener() : catch resetCandle event | name=\"%s\" | thiz name=\"%s\"\n", getName().c_str(), thiz->getName().c_str());
	}
}

#ifndef ANDROID_REDNER
void GCandle::render(klib::kMesh* mesh, const klib::math::Vector3& scale, klib::kGraphicsPipline* pipeline)
{
	IGimmickObj::render(this->mpMesh, scale, pipeline);
}
#endif
