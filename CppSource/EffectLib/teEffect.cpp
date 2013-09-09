#include	"EffectLib\\Effect.h"
#include	"EffectLib\\Particle.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

using namespace EffectLib;

//	シングルトン
EffectManager_Singleton* EffectManager_Singleton::singleton = NULL;

void EffectManager::Clear()
{
	m_UseNum = 0;
}

void EffectManager::Destroy()
{
	sParticle->deleteInstance();
}

void EffectManager::Initialize()
{
	Setting_Texture();

	//---------------------------------------------
	//	パーティクル初期化
	//---------------------------------------------
	sParticle->Initialize();
}

void EffectManager::Setting_Texture()
{
	EmitterSet::Setting_Texture("effect/test.tes");
	EmitterSet::Setting_Texture("effect/[E]_fireball.tes");
	EmitterSet::Setting_Texture("effect/[E]_firecharge.tes");
	EmitterSet::Setting_Texture("effect/bluefire.tes");
	EmitterSet::Setting_Texture("effect/expro.tes");
	EmitterSet::Setting_Texture("effect/fire.tes");
	EmitterSet::Setting_Texture("effect/fireanime.tes");
	EmitterSet::Setting_Texture("effect/firesp.tes");
	EmitterSet::Setting_Texture("effect/wind.tes");
	EmitterSet::Setting_Texture("effect/yosiwind.tes");

	LOGI("---------------------------------------------------------\n");
LOGI("%d\n%s",__LINE__,__FILE__);
LOGI("---------------------------------------------------------\n");
}

void EffectManager::Update()
{
	int use=0;
	for( int n=0; n<EMITTERSET_MAX; n++ )
	{
		if( use >= m_UseNum ){ break; }
		if( m_spEffectEmitterSet[n].GetRefNum() == 0 ){ continue; }

		if( m_spEffectEmitterSet[n]->Update() == true ){
			use++;
		}else{
			m_spEffectEmitterSet[n].Clear();
			m_UseNum--;
			continue;
		}
	}

	sParticle->Update();
}

void EffectManager::Render()
{
	int use = 0;
	for( int n=0; n<EMITTERSET_MAX; n++ )
	{
		if( use >= m_UseNum ){ break; }
		if( m_spEffectEmitterSet[n].GetRefNum() == 0 ){ continue; }

		use++;
		m_spEffectEmitterSet[n]->Render();
	}

	//	パーティクル描画
	sParticle->Render();
}

wp<EmitterSet> EffectManager::Create(eEFFECT_TYPE Type)
{
	for( int n=0; n<EMITTERSET_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitterSet[n].GetRefNum() != 0 ){ continue; }

		Create_EmitterSet(Type,n);

		return m_spEffectEmitterSet[n];
	}
	return NULL;
}

wp<EmitterSet> EffectManager::Create(eEFFECT_TYPE Type,const Vector3& Position)
{
	for( int n=0; n<EMITTERSET_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitterSet[n].GetRefNum() != 0 ){ continue; }

		Create_EmitterSet(Type,n);

		m_spEffectEmitterSet[n]->Create(Position);

		return m_spEffectEmitterSet[n];
	}
	return NULL;
}
wp<EmitterSet> EffectManager::Create(eEFFECT_TYPE Type,const Vector3& Position,
																		 float Scale )
{
	for( int n=0; n<EMITTERSET_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitterSet[n].GetRefNum() != 0 ){ continue; }

		Create_EmitterSet(Type,n);

		m_spEffectEmitterSet[n]->Create(Position);

		m_spEffectEmitterSet[n]->Setting_Scale(Scale);

		return m_spEffectEmitterSet[n];
	}
	return NULL;
}

wp<EmitterSet> EffectManager::Create(eEFFECT_TYPE Type,const Vector3& Position,
																		 float Scale, int AddLife )
{
	for( int n=0; n<EMITTERSET_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitterSet[n].GetRefNum() != 0 ){ continue; }

		Create_EmitterSet(Type,n);

		m_spEffectEmitterSet[n]->Create(Position);

		m_spEffectEmitterSet[n]->Setting_Scale(Scale);

		m_spEffectEmitterSet[n]->Setting_AddLife(AddLife);

		return m_spEffectEmitterSet[n];
	}
	return NULL;
}

void EffectManager::Create_EmitterSet(eEFFECT_TYPE Type,int n)
{
	m_spEffectEmitterSet[n].SetPtr(new EmitterSet);

	m_UseNum++;

	switch( Type )
	{
	case TEST:
		m_spEffectEmitterSet[n]->Initialize("effect/test.tes");
		break;
	case FIRE_BALL:
		m_spEffectEmitterSet[n]->Initialize("effect/[E]_fireball.tes");
		break;
	case FIRE_CHARGE:
		m_spEffectEmitterSet[n]->Initialize("effect/[E]_firecharge.tes");
		break;
	case BLUE_FIRE:
		m_spEffectEmitterSet[n]->Initialize("effect/bluefire.tes");
		break;
	case EXPRO:
		m_spEffectEmitterSet[n]->Initialize("effect/expro.tes");
		break;
	case FIRE:
		m_spEffectEmitterSet[n]->Initialize("effect/fire.tes");
		break;
	case FIRE_ANIME:
		m_spEffectEmitterSet[n]->Initialize("effect/fireanime.tes");
		break;
	case FIRE_SP:
		m_spEffectEmitterSet[n]->Initialize("effect/firesp.tes");
		break;
	case WIND:
		m_spEffectEmitterSet[n]->Initialize("effect/wind.tes");
		break;
	case YOSI_WIND:
		m_spEffectEmitterSet[n]->Initialize("effect/yosiwind.tes");
		break;
	}
}