#include "Bullet.h"

#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIMO.h"

#include "Game\Gimmick\Gimmick.h"

#include "EffectLib\Effect.h"

#include "Ueda\rTestScene.h"

using namespace rlib;

Bullet::Bullet()
{
	mCount = 0;

}

Bullet::~Bullet()
{
	//this->mEmitter->End();
}

void Bullet::init(BulletInfo& info)
{
	this->mPos = info.pos;
	this->mRange = info.size;
	this->mVelocity = info.velocity;
	//this->mEmitter.Clear();
	//this->mEmitter = EffectLib::EffectManager_Singleton::getInstance()->Create( EffectLib::FIRE_BALL );
	//this->mEmitter->Setting_Position( this->mPos );
	//this->mEmitter->Setting_Scale( this->mRange.x );
	//this->mEmitter->Loop();
}

int Bullet::update()
{
	this->mPos += this->mVelocity;
	//this->mVelocity.y -= 0.01f;

	//if( this->mPos.y < 0 )	return MSG_DEAD;
	if(fabs( this->mPos.x ) > 200.f ||
	   fabs( this->mPos.z ) > 200.f )
	{
		//this->mEmitter->End();
		return MSG_DEAD;
	}
	//this->mEmitter->Setting_Position( this->mPos );

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
klib::kMesh					*mpMesh;

static const char* TAG_M = "BulletManager";

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::clear()
{
	clearData();
	delete mpMesh;
}

void BulletManager::clearData()
{
	this->mData.clear();
}

void BulletManager::init()
{
	LOGI(TAG_M, "Execute BulletManager init");

	mpMesh = new klib::kMesh("kanetaPlace/kman.IMO", new klib::kMeshLoadIMO(), new klib::kMeshGLES20Render() );
	mpMesh->setScale(0.01f);
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
		while( gIt != gimmickMng.end() )
		{
			if( it->vs( (*gIt) ) )
			{
				(*gIt)->on();
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
		//it->render();
		mpMesh->setPosition(it->getPos());
		mpMesh->Update();
		mpMesh->Render(rTestScene::pipeline);
		it++;
	}
}
