#include	"EffectLib\\EmitterSet.h"
using namespace EffectLib;
#include	<iostream>
#include	<math.h>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


//----------------------------------------------------------------------
//	EmitterSet
//----------------------------------------------------------------------
void EmitterSet::Create(
	const Vector3& Position )
{
	for( int n=0; n<EFFECT_EMITTER_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitter[n].GetRefNum() == 0 ){ continue; }
		
		m_spEffectEmitter[n]->Setting_Pos( Position );
	}
}
void EmitterSet::Setting_Scale(float Scale)
{
	for( int n=0; n<EFFECT_EMITTER_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitter[n].GetRefNum() == 0 ){ continue; }
		
		m_spEffectEmitter[n]->getData()->Scale = Scale;
	}
}

void EmitterSet::Setting_AddLife(int AddLife)
{
	for( int n=0; n<EFFECT_EMITTER_MAX; n++ )
	{
		//	@if 使われてないので終了
		if( m_spEffectEmitter[n].GetRefNum() == 0 ){ continue; }
		
		m_spEffectEmitter[n]->getData()->addLife = AddLife;
	}
}

void EmitterSet::Create(
	const Vector3& Position,
	const Vector3& VelosityTarget )
{
#if 0
	m_It = m_EffectEmitterList.begin();
	while( m_It != m_EffectEmitterList.end() )
	{
		//	各エミッターに設定していく
		m_It->Create_Emitter();
		m_It->setPos( Position );
		m_It->Setting_VelocityTarget_To_Velocity( VelosityTarget );
		m_It++;
	}
#endif
}



bool EmitterSet::Initialize(char* File)
{
LOGI("---------------------------------------------------------\n");
LOGI("%s",File);
LOGI("---------------------------------------------------------\n");

	Load_TES( File );

	return true;
}

void EmitterSet::Clear()
{
	m_Frame = -1;
	m_UseNum = 0;
}

void EmitterSet::Destroy()
{
	
}

void EmitterSet::Setting_Texture(char* Filename)
{
	sp<textLoader> t;
	t.SetPtr(new textLoader(Filename));

	char workN[TEXTURE_NAME_SIZE] = {NULL};
	t->Search( "EmitterMax" );
	int max = t->LoadInt();
	

	for(int n=0;n<max;n++)
	{
		//[in]	Texname
		t->Search( "TextureName" );
		t->LoadName( workN );

		sParticle->Setting_Texture( workN );
	}
}

void EmitterSet::Load_TES( char* Filename )
{
	m_spTextLoader.SetPtr( new textLoader(Filename) );

	//	エミッターの最大数取得
	m_spTextLoader->Search( "EmitterMax" );
	int max = m_spTextLoader->LoadInt();
	m_UseNum = max;

	//
	//	@for エミッターの最大数分ループ
	//
	for( int n=0; n<max; n++ )
	{
		//	Emitter { 
		m_spTextLoader->Search( "Emitter" );
		m_spTextLoader->In();


		//
		//	使われていない配列を探して、そこにデータを格納
		//
		ParticleEmitter* p;
		for( int n=0; n<EFFECT_EMITTER_MAX; n++ )
		{
			//	使われていたらcontinue
			if( m_spEffectEmitter[n].GetRefNum() != 0 ){ continue; }


			//	ParticleEmitterを格納する
			m_spEffectEmitter[n].SetPtr( new ParticleEmitter );


			p = (ParticleEmitter*)m_spEffectEmitter[n].GetPtr() ;

			break;
		}

		char workN[TEXTURE_NAME_SIZE] = {NULL};
		sp<textLoader> t = m_spTextLoader;
		bool workB = false;

		//--------------------------------------------------
		//	EmitterParameters
		//--------------------------------------------------
		//Parametersの中に入る
		t->Search( "EmitterParameters" );
		t->In();

		//	共通データ
		t->Search( "Common" );
		t->In();

		//[in]	StartFrame
		t->Search("StartFrame");
		p->getData()->startFrame = t->LoadInt();

		//[in]	Life
		t->Search("Life");

		p->getData()->life = t->LoadInt();
		p->getData()->rLifeMin = t->LoadInt();
		p->getData()->rLifeMax = t->LoadInt();

		static Vector3 v;

		//[in]	Velocity
		t->Search("Velocity");
		t->In();
		t->Search("Value");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getData()->vel = v;
		t->Search("RandMin");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getData()->rVelMin = v;
		t->Search("RandMax");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getData()->rVelMax = v;

		p->getData()->vel = 
			Vector3( tRand(p->getData()->rVelMin.x,p->getData()->rVelMax.x),
							 tRand(p->getData()->rVelMin.y,p->getData()->rVelMax.y),
							 tRand(p->getData()->rVelMin.z,p->getData()->rVelMax.z) )
			+ p->getData()->vel;

		t->Out();

		//[in]	InitPos
		t->Search("InitPos");
		t->In();
		t->Search("Value");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getData()->initPos = v;
		t->Search("RandMin");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getData()->rInitPosMin = v;
		t->Search("RandMax");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getData()->rInitPosMax = v;

		t->Out();

		//	AddVec
		t->Search("AddVec");
		t->In();
			t->Search("Start");
			v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
			p->getData()->addVecS = v;
			t->Search("Middle");
			v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
			p->getData()->addVecM = v;
			t->Search("End");
			v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
			p->getData()->addVecE = v;

		t->Out();


		t->Out();

		//--------------------------------------------------
		//	Parameters
		//--------------------------------------------------
		t->Search("Parameters");
		t->In();

		//[in]	Texname
		t->Search( "TextureName" );
		t->LoadName( workN );
		strcpy( p->getEffectData()->file, workN );

#if 0
		sParticle->Setting_Texture( workN );
#endif


		//[in]	TextureMoveFlag
		t->Search( "TextureMoveFlag" );
		if( t->LoadInt() == 0 )
		{
			workB = false;
		}else{
			workB = true;
		}
		p->getEffectData()->moveFlag = workB;

		//[in]	TextureSize
		t->Search( "TextureSize" );
		p->getEffectData()->size = m_spTextLoader->LoadInt();

		//[in]	ParticleIndex
		this->m_spTextLoader->Search( "ParticleIndex" );
		p->getEffectData()->index = m_spTextLoader->LoadInt();

		//[in]	Life
		t->Search("Life");
		p->getEffectData()->life = t->LoadInt();
		p->getEffectData()->rLifeMin = t->LoadInt();
		p->getEffectData()->rLifeMax = t->LoadInt();

		p->setMaxLife(p->getEffectData()->life);

		//[in]	WindPower
		t->Search("WindPower");
		p->getEffectData()->windPower = t->LoadFloat();

		//[in]	CenterPower
		t->Search("CenterPower");
		p->getEffectData()->centerPowerS = t->LoadFloat();
		p->getEffectData()->centerPowerM = t->LoadFloat();
		p->getEffectData()->centerPowerE = t->LoadFloat();

		//[in]	Color
		t->Search("Color");
		t->In();
		t->Search("Start");
		COLOR c;
		c.red=t->LoadInt();c.green=t->LoadInt();c.blue=t->LoadInt();
		p->getEffectData()->colorS = c;
		t->Search("Middle");
		c.red=t->LoadInt();c.green=t->LoadInt();c.blue=t->LoadInt();
		p->getEffectData()->colorM = c;
		t->Search("End");
		c.red=t->LoadInt();c.green=t->LoadInt();c.blue=t->LoadInt();
		p->getEffectData()->colorE = c;

		t->Out();

		//[in]	RandColor
		t->Search("RandColor");
		t->In();
		t->Search("Start");
		c.red=t->LoadInt();c.green=t->LoadInt();c.blue=t->LoadInt();
		p->getEffectData()->rColorS = c;
		t->Search("Middle");
		c.red=t->LoadInt();c.green=t->LoadInt();c.blue=t->LoadInt();
		p->getEffectData()->rColorM = c;
		t->Search("End");
		c.red=t->LoadInt();c.green=t->LoadInt();c.blue=t->LoadInt();
		p->getEffectData()->rColorE = c;

		t->Out();

		//[in]	Scale
		t->Search("Scale");
		t->In();
		t->Search("Value");
		p->getEffectData()->scaleS = t->LoadFloat();
		p->getEffectData()->scaleM = t->LoadFloat();
		p->getEffectData()->scaleE = t->LoadFloat();
		t->Search("RandMin");
		p->getEffectData()->rScaleStartMin = t->LoadFloat();
		p->getEffectData()->rScaleMiddleMin = t->LoadFloat();
		p->getEffectData()->rScaleEndMin = t->LoadFloat();
		t->Search("RandMax");
		p->getEffectData()->rScaleStartMax = t->LoadFloat();
		p->getEffectData()->rScaleMiddleMax = t->LoadFloat();
		p->getEffectData()->rScaleEndMax = t->LoadFloat();

		t->Out();

		//[in]	Velocity
		t->Search("Velocity");
		t->In();
		t->Search("Value");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getEffectData()->vel = v;
		t->Search("RandMin");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getEffectData()->rVelMin = v;
		t->Search("RandMax");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getEffectData()->rVelMax = v;

		t->Out();

		//[in]	InitPos
		t->Search("InitPos");
		t->In();
		t->Search("Value");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getEffectData()->initPos = v;
		t->Search("RandMin");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getEffectData()->rInitPosMin = v;
		t->Search("RandMax");
		v.x=t->LoadFloat();v.y=t->LoadFloat();v.z=t->LoadFloat();
		p->getEffectData()->rInitPosMax = v;

		t->Out();

		t->Out();

		t->Out();
	}

	m_spTextLoader.Clear();
}

void EmitterSet::Setting_Position(const Vector3& Pos)
{
	int use = 0;
	//
	//	@for EffectEmitter最大数分ループ
	//
	for( int n=0; n<EFFECT_EMITTER_MAX; n++ )
	{
		if( use >= m_UseNum ){ break; }
		if( m_spEffectEmitter[n].GetRefNum() == 0 ){ continue; }

		m_spEffectEmitter[n]->setPos(Pos);
		use++;
	}
}
void EmitterSet::Setting_Velocity(const Vector3& Velocity)
{
	int use = 0;
	//
	//	@for EffectEmitter最大数分ループ
	//
	for( int n=0; n<EFFECT_EMITTER_MAX; n++ )
	{
		if( use >= m_UseNum ){ break; }
		if( m_spEffectEmitter[n].GetRefNum() == 0 ){ continue; }

		m_spEffectEmitter[n]->setVelocity(Velocity);
		use++;
	}
}

void EmitterSet::Loop()
{
	int use = 0;
	//
	//	@for EffectEmitter最大数分ループ
	//
	for( int n=0; n<EFFECT_EMITTER_MAX; n++ )
	{
		if( use >= m_UseNum ){ break; }
		if( m_spEffectEmitter[n].GetRefNum() == 0 ){ continue; }

		m_spEffectEmitter[n]->setLoopFlag(true);
		use++;
	}
}

void EmitterSet::End()
{
	int use = 0;
	//
	//	@for EffectEmitter最大数分ループ
	//
	for( int n=0; n<EFFECT_EMITTER_MAX; n++ )
	{
		if( use >= m_UseNum ){ break; }
		if( m_spEffectEmitter[n].GetRefNum() == 0 ){ continue; }

		m_spEffectEmitter[n]->setLoopFlag(false);
		use++;
	}
}

bool EmitterSet::Update()
{
	m_Frame++;

	bool updateFlag = false;
	int use = 0;
	//
	//	@for EffectEmitter最大数分ループ
	//
	for( int n=0; n<EFFECT_EMITTER_MAX; n++ )
	{
		if( use >= m_UseNum ){ break; }
		if( m_spEffectEmitter[n].GetRefNum() == 0 ){ continue; }

		if( m_spEffectEmitter[n]->Update() == true ){
			use++;
			updateFlag = true;
		}else{
			m_spEffectEmitter[n].Clear();
			m_UseNum--;
			continue;
		}
	}
	
	return updateFlag;
}

void EmitterSet::Render()
{
	int use=0;
	for( int n=0; n<EFFECT_EMITTER_MAX; n++ )
	{
		if( use >= m_UseNum ){ break; }
		//	@if 使われてないので終了
		if( m_spEffectEmitter[n].GetRefNum() == 0 ){ continue; }
		m_spEffectEmitter[n]->Render();
		use++;
	}
}
