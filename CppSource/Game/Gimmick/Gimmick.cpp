#include "Gimmick.h"

#ifdef ANDROID_REDNER
#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

#include "EffectLib\Effect.h"
#include "Ueda\rTestScene.h"

#include "utility\debugMessageMng.h"
#endif

#include "PlacementLib\Placement.h"

#include "utility\utility.h"

#include "Game\Gimmick\GimmickInfo.h"
#include "Game\Gimmick\GimmickInfoManager.h"


//#include "Tool\Particle.h"

using namespace rlib;
using namespace klib;
using namespace PlacementLib;

static const char* TAG = "Gimmick";

Gimmick::Gimmick():
	mType(-1),
	mIsOn(false)
{
	mCount = 0;
}

Gimmick::~Gimmick()
{
}

void Gimmick::init(GimmickInfo& info)
{
	this->mPos = info.pos;
	this->mRange = info.size;
	this->mVelocity.x = this->mVelocity.y = this->mVelocity.z = 0.f;
	this->mAngle = info.angle;
}

int Gimmick::update()
{
	if( this->mIsOn )
	{
#ifdef ANDROID_REDNER
		EffectLib::EffectManager_Singleton::getInstance()->Create( EffectLib::FIRE_CHARGE, this->mPos );
#endif
		return MSG_DEAD;
	}

	return MSG_NON;
}

#ifdef ANDROID_REDNER
void Gimmick::render(klib::kMesh* mesh, klib::kGraphicsPipline* pipeline)
{
	mesh->setPosition(this->mPos);
	mesh->setAngle(this->mAngle);
	mesh->setScale(this->mRange*0.01f);
	mesh->Update();
	mesh->Render(pipeline);
}
#endif

//===============================================================================
//
//		GimmickManagerクラス
//		・シングルトン
//
//===============================================================================
#ifdef ANDROID_REDNER
#include "GraphicsLib\Class\kInputLayout\kInputLayout.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
#endif

using namespace klib;

static const char* TAG_M = "GimmickManager";

GimmickManager::GimmickManager()
{
}

GimmickManager::~GimmickManager()
{
}

void GimmickManager::clear()
{
	clearData();

}

void GimmickManager::clearData()
{
	Iterator it = this->mData.begin();
	while( it != end() )
	{
		if( (*it) ){ delete (*it); (*it) = NULL; }
		it++;
	}
	this->mData.clear();
}

void GimmickManager::init(const char* giFilePath)
{
	LOGI(TAG_M, "Execute GimmickManager init\n");

	GimmickInfoManager infoMng;
	infoMng.load(giFilePath);
	std::vector<GimmickInfoBase*>& datas = infoMng.getDatas();
	LOGI(TAG, "data count = %d\n", datas.size() );
	this->mData.resize(datas.size());
	for( size_t i=0; i<datas.size(); i++ )
	{
		//タイプに応じて作るギミックを決める
		this->mData[i] = datas[i]->makeGimmick();
	}
	infoMng.clear();

#ifdef ANDROID_REDNER
	loadMeshes();
#endif

	LOGI(TAG, "box num = %d\n", this->mData.size() );
	LOGI(TAG,"OK Placement read\n");
	LOGI(TAG_M, "Complete GimmickManager init\n");
}

#ifdef ANDROID_REDNER
void GimmickManager::loadMeshes()
{
	this->mpMeshies.SetPtr(new klib::kMesh*, true, Gimmick::eTYPE_NUM);
	this->mpMeshies[0] = new klib::kMesh("kibako128.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );
}
#endif

void GimmickManager::add(GimmickInfo& info)
{
	Gimmick* add = new Gimmick();
	add->init(info);
	this->mData.push_back(add);
}

#ifdef ANDROID_REDNER
klib::kMesh* GimmickManager::getMesh( int type )
{
	return this->mpMeshies[0];
}
#endif

int GimmickManager::update()
{
	Iterator it = this->mData.begin();
	while( it != end() )
	{
		int msg = (*it)->update();
		if( msg == Gimmick::MSG_DEAD )
		{
			delete (*it);
			this->mData.erase(it);
		}
		else
		{
			it++;
		}
	}

#ifdef ANDROID_REDNER
	DEBUG_MSG("gimmick num = %d", this->mData.size());
#endif
	return MSG_NON;
}

#ifdef ANDROID_REDNER
void GimmickManager::render()
{
	Iterator it = this->mData.begin();
	while( it != end() )
	{
		(*it)->render( getMesh((*it)->getType()), rTestScene::pipeline);
		it++;
	}
}
#endif
