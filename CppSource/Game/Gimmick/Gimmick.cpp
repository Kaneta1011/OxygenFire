#include "Gimmick.h"

#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

#include "EffectLib\Effect.h"

#include "utility\utility.h"

//#include "Tool\Particle.h"

using namespace rlib;
using namespace klib;

static const char* TAG = "Gimmick";

Gimmick::Gimmick():
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
}

int Gimmick::update()
{
	if( this->mIsOn )
	{
		EffectLib::EffectManager_Singleton::getInstance()->Create( EffectLib::BLUE_FIRE, this->mPos );
		return MSG_DEAD;
	}

	return MSG_NON;
}

void Gimmick::render(klib::kMesh* mesh, klib::kGraphicsPipline* pipeline)
{
	mesh->setPosition(this->mPos);
	mesh->setScale(0.01f);
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
	pipline(NULL),
	mpMesh(NULL)
{
}

GimmickManager::~GimmickManager()
{
}

void GimmickManager::clear()
{
	clearData();

	if( pipline ){ delete pipline; pipline = NULL; }
	if( this->mpMesh ){ delete this->mpMesh; this->mpMesh = NULL; } 
}

void GimmickManager::clearData()
{
	this->mData.clear();
}

void GimmickManager::init()
{
	LOGI(TAG_M, "Execute GimmickManager init");

	pipline = new klib::kGraphicsPipline();
	pipline->createVertexShader("vertex.txt");
	pipline->createPixelShader("pixel.txt");
	pipline->createBlendState(k_BLEND_NONE);
	pipline->createDepthStencilState(true,eLESS_EQUAL);
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

	this->mpMesh = new klib::kMesh( "kibako128.IMO", new kMeshLoadIMO(),new kMeshGLES20Render() );
	LOGI(TAG, "OK mesh load");

	LOGI(TAG_M, "Complete GimmickManager init");
}

void GimmickManager::add(GimmickInfo& info)
{
	Gimmick add;
	add.init(info);
	this->mData.pushFront(add);
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
	return MSG_NON;
}

void GimmickManager::render()
{
	Iterator it = this->mData.begin();
	while( !it.isEnd() )
	{
		it->render( this->mpMesh, this->pipline);
		it++;
	}
}
