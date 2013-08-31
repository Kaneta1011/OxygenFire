#ifndef __teEmitter_H__
#define __teEmitter_H__
#include	"math\\kmathf.h"
#include	"EffectLib\\Item.h"
#include	"EffectLib\\Particle.h"
#include	"StandardLib\\SmartPointer.h"

namespace EffectLib{
using namespace klib::math;
//	Constant
enum eEMITTER_TYPE
{
	EMITTER_NoType = -1,

	EMITTER_PARTICLE,
	EMITTER_MESH,
};

//-------------------------------------------------------------------------
//	Emitter
//-------------------------------------------------------------------------
struct EmitterData
{
	int startFrame;
	int life;
	int rLifeMin;
	int rLifeMax;
	Vector3 vel;
	Vector3 rVelMin;
	Vector3 rVelMax;
	Vector3 initPos;
	Vector3 rInitPosMin;
	Vector3 rInitPosMax;
	Vector3 addVecS;
	Vector3 addVecM;
	Vector3 addVecE;
	EmitterData();
};

class Emitter
{
public:
	//	Update
	virtual bool Update(){ return true; }
	//	Render
	virtual void Render(){}
	//	Setting
	static void Setting_EmitterType( eEMITTER_TYPE& Out_EmitterType, char* str );
	void Setting_VelocityTarget_To_Velocity( const Vector3& VelocityTarget );
	virtual void Create_Emitter(){}
	//	set
	void setPos(const Vector3& Pos){m_Pos=Pos;}
	//	setting
	void Setting_Pos(const Vector3& Pos);
	//	get
	sp<EmitterData> getData(){return m_spData;}//時間ないし楽する
	//	BasicSet
	Emitter(){Clear();}void Clear();
	virtual ~Emitter(){Destroy();}void Destroy();
protected:
	//	Setting
	void Setting_Alpha();
	//	Data
	sp<EmitterData> m_spData;
	int					m_Count;
	Vector3			m_Pos;
};

//----------------------------------------
//	ParticleEmitter
//----------------------------------------
struct EffectData
{
	char file[TEXTURE_NAME_SIZE];
	int life;
	int rLifeMin;
	int rLifeMax;
	float windPower;
	float centerPowerS;
	float centerPowerM;
	float centerPowerE;
	COLOR colorS;
	COLOR colorM;
	COLOR colorE;
	COLOR rColorS;
	COLOR rColorM;
	COLOR rColorE;
	float scaleS;
	float scaleM;
	float scaleE;
	float rScaleStartMin;
	float rScaleMiddleMin;
	float rScaleEndMin;
	float rScaleStartMax;
	float rScaleMiddleMax;
	float rScaleEndMax;
	Vector3 vel;
	Vector3 rVelMin;
	Vector3 rVelMax;
	Vector3 initPos;
	Vector3 rInitPosMin;
	Vector3 rInitPosMax;
};
struct EffectData_Particle : public EffectData
{
	bool moveFlag;
	int index;
	int size;
};

//-------------------------------------------------------------------------
//	ParticleEmitter
//-------------------------------------------------------------------------
class ParticleEmitter : public Emitter
{
public:
	//	Upadte
	bool Update();
	//	Render
	void Render(){}
	//	get
	sp<EffectData_Particle> getEffectData(){return m_spEffectData;}
	//	BasicSet
	ParticleEmitter(){ Clear(); } void Clear();
	virtual ~ParticleEmitter(){ Destroy(); } void Destroy();
private:
	//	Generation(発生)
	void Generation_Particle();
	//	Adjustment(調整)
	bool Adjustment_StartFrame();
	bool Adjustment_LifeEnd();
	//	Data
	sp<EffectData_Particle>		m_spEffectData;//えっget-set-作るのめんどくさいし。
};


}//EffectLib

#endif