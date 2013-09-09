#include	"Effectlib\\Emitter.h"
#include	"utility\textLoader.h"

#include	<iostream>
#include	<math.h>

using namespace EffectLib;
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)



float Rand( float Min, float Max )
{
	if( Min < 0 )
		Max += -Min;
	else
		Max -= Min;

	return ( float )rand() / ( float )RAND_MAX * Max + Min;
}

//----------------------------------------
//	EmitterData
//----------------------------------------
EmitterData::EmitterData()
{
	addLife = 0;
}

//-------------------------------------------------------------------------
//	Emitter
//-------------------------------------------------------------------------
void Emitter::Clear()
{
	m_Count = 0;
	m_LoopFlag = false;
	m_spData.SetPtr(new EmitterData);
	m_spData->Scale=1.0f;
	m_Angle=Vector3(0,0,0);

	m_Velocity = Vector3(0,0,0);
	m_VelFlag = false;
}

void Emitter::Destroy()
{

}
void Emitter::Setting_EmitterType( eEMITTER_TYPE& Out_EmitterType, char* str )
{
	//	パーティクル
	if( strcmp( str, "Particle" ) == 0 ){
		Out_EmitterType = EMITTER_PARTICLE;
		return;
	}
	//	メッシュ
	if( strcmp( str, "Mesh" ) == 0 ){
		Out_EmitterType = EMITTER_MESH;
		return;
	}
}

void Emitter::Setting_VelocityTarget_To_Velocity( const Vector3& VelocityTarget )
{
	float length = m_spData->vel.length();
	m_spData->vel.x = VelocityTarget.x * length;
	m_spData->vel.y = VelocityTarget.y * length;
	m_spData->vel.z = VelocityTarget.z * length;
}

void Emitter::Setting_Alpha()
{
#if 0
	float	rate = (float)m_Count / (float)m_Life;
	rate = rate * 3.14f;
	
	m_Color.alpha = (BYTE)( sinf(rate) * 255.0f );
#endif
}

void Emitter::Setting_Pos(const Vector3& Pos)
{
	m_Pos = 
		m_spData->initPos + Vector3(
			Rand( m_spData->rInitPosMin.x, m_spData->rInitPosMax.x ),
			Rand( m_spData->rInitPosMin.y, m_spData->rInitPosMax.y ),
			Rand( m_spData->rInitPosMin.z, m_spData->rInitPosMax.z ) )
		+ Pos;
}

//-------------------------------------------------------------------------
//	ParticleEmitter
//-------------------------------------------------------------------------
void ParticleEmitter::Clear()
{
	m_spEffectData.SetPtr(new EffectData_Particle);
}

void ParticleEmitter::Destroy()
{

}

bool ParticleEmitter::Update()
{
	//	スタート開始調整
	if( Adjustment_StartFrame() == false ){ return true; }

	//	加算ライフ
	if( m_spData->addLife != 0 ){
		m_spData->life += m_spData->addLife;
		m_spData->addLife = 0;
	}

	if( true == m_LoopFlag ){
		if( m_Count >  ( m_MaxLife / 2 ) ){
			m_Count = m_MaxLife / 2;
		}
	}

	//	寿命が０以下ならエミッター消去
	if( Adjustment_LifeEnd() == true ){ return false; }


	//	加算地合成
	static Vector3 add;
	static Vector3 v;
	static float kakeru;
	kakeru = 1.0f / ((float)m_spData->life/2.0f);
	if(m_Count <= (m_spData->life/2))
	{
		v = m_spData->addVecM - m_spData->addVecS;
		add = v * (kakeru*m_Count);
	}else{
		v = m_spData->addVecE - m_spData->addVecM;
		add = m_spData->addVecM + v * (kakeru*(m_Count-(m_spData->life/2)));
	}
	m_Pos += add;

	//	速度加算
	m_Pos += m_spData->vel;

	//	パーティクル発生
	Generation_Particle();

	return true;
}

bool ParticleEmitter::Adjustment_StartFrame()
{
	if( m_spData->startFrame > 0 )
	{ 
		m_spData->startFrame--;
		return false; 
	}
	m_spData->startFrame = 0;

	return true;
}

bool ParticleEmitter::Adjustment_LifeEnd()
{

	if( true == m_LoopFlag ){
		if( m_MaxLife == 1 ){ m_Count = 0; }
	}else{
		m_Count++;
	}

	if( m_Count > m_spData->life ){ return true; } 

	return false;
}

void ParticleEmitter::Generation_Particle()
{
	//	Position
	static Vector3 p;//pos
	static Vector3 vel;//velocity
	static COLOR col;//color
	static float scaS;
	static float scaM;
	static float scaE;
	static COLOR colS;
	static COLOR colM;
	static COLOR colE;

	static float work;
	static Vector3 V;
	static Vector3 VR;

	p = Vector3(
	Rand(m_spEffectData->rInitPosMin.x,m_spEffectData->rInitPosMax.x),
	Rand(m_spEffectData->rInitPosMin.y,m_spEffectData->rInitPosMax.y),
	Rand(m_spEffectData->rInitPosMin.z,m_spEffectData->rInitPosMax.z) );
	p = m_Pos + m_spEffectData->initPos + p;

	//	R_Velocity

	if( false == m_VelFlag )
	{
		vel = 
			m_spEffectData->vel + 
			Vector3( Rand(m_spEffectData->rVelMin.x,m_spEffectData->rVelMax.x),
							 Rand(m_spEffectData->rVelMin.y,m_spEffectData->rVelMax.y),
							 Rand(m_spEffectData->rVelMin.z,m_spEffectData->rVelMax.z) );
	}else{
		work = m_spEffectData->vel.length();

		Vector3Normalize(&m_Velocity,m_Velocity);

		V = (m_Velocity*work);

		VR = Vector3( Rand(m_spEffectData->rVelMin.x,m_spEffectData->rVelMax.x),
							 Rand(m_spEffectData->rVelMin.y,m_spEffectData->rVelMax.y),
							 Rand(m_spEffectData->rVelMin.z,m_spEffectData->rVelMax.z) );

		work = VR.length();

		Vector3Normalize(&VR,VR);

		VR =m_Velocity*work;

		vel = V + VR;
	}

	//	R_Scale
	scaS = m_spEffectData->scaleS + 
		Rand(m_spEffectData->rScaleStartMin,m_spEffectData->rScaleStartMax);
	scaM = m_spEffectData->scaleM + 
		Rand(m_spEffectData->rScaleMiddleMin,m_spEffectData->rScaleMiddleMax);
	scaE = m_spEffectData->scaleE + 
		Rand(m_spEffectData->rScaleEndMin,m_spEffectData->rScaleEndMax);
	//	カラー
	colS.red = 
		m_spEffectData->colorS.red + 
		Rand( 0, m_spEffectData->rColorS.red );
	colS.green = 
		m_spEffectData->colorS.green + 
		Rand( 0, m_spEffectData->rColorS.green );
	colS.blue = 
		m_spEffectData->colorS.blue + 
		Rand( 0, m_spEffectData->rColorS.blue );
	colM.red = 
		m_spEffectData->colorM.red + 
		Rand( 0, m_spEffectData->rColorM.red );
	colM.green = 
		m_spEffectData->colorM.green + 
		Rand( 0, m_spEffectData->rColorM.green );
	colM.blue = 
		m_spEffectData->colorM.blue + 
		Rand( 0, m_spEffectData->rColorM.blue );
	colE.red = 
		m_spEffectData->colorE.red + 
		Rand( 0, m_spEffectData->rColorE.red );
	colE.green = 
		m_spEffectData->colorE.green + 
		Rand( 0, m_spEffectData->rColorE.green );
	colE.blue = 
		m_spEffectData->colorE.blue + 
		Rand( 0, m_spEffectData->rColorE.blue );

		sParticle->Setting(
			p,
			m_spEffectData->file,
			scaS*m_spData->Scale,
			scaM*m_spData->Scale,
			scaE*m_spData->Scale,
			m_spEffectData->life,
			vel,
			m_spEffectData->moveFlag,
			m_spEffectData->size,
			m_spEffectData->index,
			m_spEffectData->windPower,
			m_spEffectData->centerPowerS,
			m_spEffectData->centerPowerM,
			m_spEffectData->centerPowerE,
			colS,
			colM,
			colE );
}