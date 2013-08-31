#include	"EffectLib\\Effect.h"
#include	"GraphicsLib\Class\tRenderState\RenderState.h"
#include	"EffectLib\\Particle.h"
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

using namespace EffectLib;

//	シングルトン
EffectManager_Singleton* EffectManager_Singleton::singleton = NULL;

void EffectManager::Clear()
{

}

void EffectManager::Destroy()
{
	sParticle->deleteInstance();
}

void EffectManager::Initialize()
{
	//---------------------------------------------
	//	パーティクル初期化
	//---------------------------------------------
	sParticle->Initialize();
}

void EffectManager::Update()
{
	//
	//	@for EffectEmitter最大数分ループ
	//
	for( int n=0; n<EMITTERSET_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitterSet[n].GetRefNum() == 0 ){ continue; }

		if( m_spEffectEmitterSet[n]->Update() == true ){
		}else{
			m_spEffectEmitterSet[n].Clear();
			continue;
		}
	}

	sParticle->Update();
}

void EffectManager::Render()
{
	for( int n=0; n<EMITTERSET_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitterSet[n].GetRefNum() == 0 ){ continue; }

		m_spEffectEmitterSet[n]->Render();
	}

	//	パーティクル描画
	sParticle->Render();
}

sp<EmitterSet> EffectManager::Create(eEFFECT_TYPE Type)
{
	for( int n=0; n<EMITTERSET_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitterSet[n].GetRefNum() != 0 ){ continue; }

		Create_EmitterSet(Type,n);
	}
}

void EffectManager::Create(eEFFECT_TYPE Type,const Vector3& Position)
{

	LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");

	for( int n=0; n<EMITTERSET_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitterSet[n].GetRefNum() != 0 ){ continue; }

		Create_EmitterSet(Type,n);

		m_spEffectEmitterSet[n]->Create(Position);
		break;
	}
}

void EffectManager::Create_EmitterSet(eEFFECT_TYPE Type,int n)
{
	m_spEffectEmitterSet[n].SetPtr(new EmitterSet);
	switch( Type )
	{
	case TEST:
		m_spEffectEmitterSet[n]->Initialize("effect/test.tes");
		LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
		break;
	case FIRE_BALL:
		m_spEffectEmitterSet[n]->Initialize("effect/[E]_fireball.tes");
		LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
		break;
	case FIRE_CHARGE:
		m_spEffectEmitterSet[n]->Initialize("effect/[E]_firecharge.tes");
		LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
		break;
	case BLUE_FIRE:
		m_spEffectEmitterSet[n]->Initialize("effect/bluefire.tes");
		LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
		break;
	case EXPRO:
		m_spEffectEmitterSet[n]->Initialize("effect/expro.tes");
		LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
		break;
	case FIRE:
		m_spEffectEmitterSet[n]->Initialize("effect/fire.tes");
		LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
		break;
	case FIRE_ANIME:
		m_spEffectEmitterSet[n]->Initialize("effect/fireanime.tes");
		LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
		break;
	case FIRE_SP:
		m_spEffectEmitterSet[n]->Initialize("effect/firesp.tes");
		LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
		break;
	case WIND:
		m_spEffectEmitterSet[n]->Initialize("effect/wind.tes");
		LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
		break;
	case YOSI_WIND:
		m_spEffectEmitterSet[n]->Initialize("effect/yosiwind.tes");
		LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
		break;
	}
}