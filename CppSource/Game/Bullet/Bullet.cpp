#include "Bullet.h"

#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"

#include "Game\Gimmick\Gimmick.h"

#include "EffectLib\Effect.h"

//#include "Tool\Particle.h"

using namespace rlib;

Bullet::Bullet()
{
	mCount = 0;
}

Bullet::~Bullet()
{
}

void Bullet::init(BulletInfo& info)
{
	this->mPos = info.pos;
	this->mRange = info.size;
	this->mVelocity = info.velocity;
}

int Bullet::update()
{
	this->mPos += this->mVelocity;
	//this->mVelocity.y -= 0.01f;

	//if( this->mPos.y < 0 )	return MSG_DEAD;
	if(fabs( this->mPos.x ) > 200.f ||
	   fabs( this->mPos.z ) > 200.f )
	   return MSG_DEAD;

	if( mCount % 10 == 0 )
	{
		EffectLib::EffectManager_Singleton::getInstance()->Create( EffectLib::FIRE_BALL, this->mPos );
	}
	mCount ++;

	return MSG_NON;
}

void Bullet::render()
{


}

//===============================================================================
//
//		BulletManagerクラス
//		・シングルトン
//
//===============================================================================
#include "GraphicsLib\Class\kInputLayout\kInputLayout.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"

using namespace klib;

static kInputElementDesc desc[]=
{
	{"POSITION",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
	{"COLOR",0,k_VF_R32G32B32A32_FLOAT,0,eVertex,0},
	{"NORMAL",0,k_VF_R32G32B32_FLOAT,0,eVertex,0},
	{"TEXCOORD",0,k_VF_R32G32_FLOAT,0,eVertex,0}
};
static u32 descnum=sizeof(desc)/sizeof(kInputElementDesc);

static const char* TAG_M = "BulletManager";

BulletManager::BulletManager():
	pipline(NULL)
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::clear()
{
	clearData();

	if( pipline ){ delete pipline; pipline = NULL; }
}

void BulletManager::clearData()
{
	this->mData.clear();
}

void BulletManager::init()
{
	LOGI(TAG_M, "Execute BulletManager init");

	pipline = new klib::kGraphicsPipline();
	pipline->createVertexShader("vertex.txt");
	pipline->createPixelShader("pixel.txt");
	pipline->createBlendState(k_BLEND_NONE);
	pipline->createDepthStencilState(true,eLESS_EQUAL);
	pipline->createRasterizerState(eSOLID,eNONE,false);
	pipline->complete(desc,descnum);
	
	float ary[3]={0.25f,0.5f,1.0f};
	pipline->setShaderValue("val",0.8f);
	pipline->setShaderValue("array",ary,3);
	LOGI(TAG_M, "OK pipeline init");

	LOGI(TAG_M, "Complete BulletManager init");
}

void BulletManager::add(BulletInfo& info)
{
	Bullet add;
	add.init(info);
	this->mData.pushFront(add);
}

int BulletManager::update()
{
	Iterator it = this->mData.begin();
	while( !it.isEnd() )
	{
		int msg = it->update();
		if( msg == Bullet::MSG_DEAD )
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

void BulletManager::collision(rlib::GimmickManager& gimmickMng)
{
	Iterator it = this->mData.begin();
	while(!it.isEnd() )
	{
		GimmickManager::Iterator gIt = gimmickMng.begin();
		bool isHit = false;
		while( !gIt.isEnd() )
		{
			if( it->vs( &(*gIt) ) )
			{
				gIt->on();
				isHit = true;
				break;
			}
			gIt++;
		}

		if( isHit )
		{
			this->mData.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void BulletManager::render()
{
	Iterator it = this->mData.begin();
	while( !it.isEnd() )
	{
		it->render();
		it++;
	}
}
