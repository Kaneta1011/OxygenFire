#include "Gimmick.h"

#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

#include "EffectLib\Effect.h"
#include "PlacementLib\Placement.h"

#include "utility\utility.h"
#include "utility\debugMessageMng.h"

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
		EffectLib::EffectManager_Singleton::getInstance()->Create( EffectLib::FIRE_CHARGE, this->mPos );
		return MSG_DEAD;
	}

	return MSG_NON;
}

void Gimmick::render(klib::kMesh* mesh, klib::kGraphicsPipline* pipeline)
{
	mesh->setPosition(this->mPos);
	mesh->setAngle(this->mAngle);
	mesh->setScale(this->mRange*0.01f);
	mesh->Update();
	mesh->Render(pipeline);
}

//===============================================================================
//
//		GimmickManagerクラス
//		・シングルトン
//
//===============================================================================
#include "GraphicsLib\Class\kInputLayout\kInputLayout.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

using namespace klib;

static const char* TAG_M = "GimmickManager";

GimmickManager::GimmickManager():
	pipline(NULL)
{
}

GimmickManager::~GimmickManager()
{
}

void GimmickManager::clear()
{
	clearData();

	if( pipline ){ delete pipline; pipline = NULL; }
}

void GimmickManager::clearData()
{
	this->mData.clear();
}

void GimmickManager::init()
{
	LOGI(TAG_M, "Execute GimmickManager init");

	this->mpMeshies.SetPtr(new klib::kMesh*, true, Gimmick::eTYPE_NUM);
	this->mpMeshies[0] = new klib::kMesh("kibako128.IMO", new klib::kMeshLoadIMO, new klib::kMeshGLES20Render() );

	pipline = new klib::kGraphicsPipline();
	pipline->createVertexShader("kanetaPlace/shader/vertex.txt");
	pipline->createPixelShader("kanetaPlace/shader/pixel.txt");
	pipline->createBlendState(k_BLEND_NONE);
	pipline->createDepthStencilState(true,true,eLESS_EQUAL);
	pipline->createRasterizerState(eSOLID,eFRONT,false);

	kInputElementDesc desc[]=
	{
		{"POSITION",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"COLOR",0,k_VF_R32G32B32A32_FLOAT,0,eVertex,0},
		{"NORMAL",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
		{"TEXCOORD",0,k_VF_R32G32_FLOAT,0,eVertex,0}
	};
	u32 descnum=sizeof(desc)/sizeof(kInputElementDesc);
	pipline->complete(desc,descnum);
	LOGI(TAG_M, "OK pipeline init");

	sp<PlacementLib::PlacementData> spData;
	sPlacementManager->GetBox( &spData );
	LOGI(TAG, "Placement data num = %d", spData->Num);

	rlib::GimmickInfo info;
	for( int i=0; i<spData->Num; i++ )
	{
		info.pos = spData->spPos[i];
		info.size = spData->spScale[i];
		info.angle = spData->spAngle[i];
		add( info );
	}
	LOGI(TAG, "box num = %d", this->mData.size() );
	LOGI(TAG,"OK Placement read ");
	LOGI(TAG_M, "Complete GimmickManager init");
}

void GimmickManager::add(GimmickInfo& info)
{
	Gimmick add;
	add.init(info);
	this->mData.pushFront(add);
}

klib::kMesh* GimmickManager::getMesh( int type )
{
	return this->mpMeshies[0];
}

int GimmickManager::update()
{
	Iterator it = this->mData.begin();
	while( !it.isEnd() )
	{
		int msg = it->update();
		if( msg == Gimmick::MSG_DEAD )
		{
			this->mData.erase(it);
		}
		else
		{
			it++;
		}
	}

	DEBUG_MSG("gimmick num = %d", this->mData.size());
	return MSG_NON;
}

void GimmickManager::render()
{
	Iterator it = this->mData.begin();
	while( !it.isEnd() )
	{
		it->render( getMesh(it->getType()), this->pipline);
		it++;
	}
}
