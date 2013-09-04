#ifndef __teEffect_H__
#define __teEffect_H__
#include	"math\\kmathf.h"
#include	"StandardLib\\SmartPointer.h"
#include	"EffectLib\\EmitterSet.h"

#include	<iostream>

namespace EffectLib{
using namespace klib::math;


/**
	@brief	�g�������G�~�b�^�[�Z�b�g
*/
enum eEFFECT_TYPE
{
	TEST,
	FIRE_BALL,
	FIRE_CHARGE,
	BLUE_FIRE,
	EXPRO,
	FIRE,
	FIRE_ANIME,
	FIRE_SP,
	WIND,
	YOSI_WIND,
};

static const int EMITTERSET_MAX = 64;
//----------------------------------------------------------------------
//	EffectManager
//----------------------------------------------------------------------
class EffectManager
{
public:
	//	�V���O���g�����l�ŏ��ɂ�낵��
	void Initialize();
	//	scene�ȂǂłP�t���[���Ɉ��K�v
	void Update();
	//	scene�ȂǂłP�t���[���Ɉ��K�v
	void Render();

	//
	//	���̊֐��ōD���ȃG�~�b�^�[�Z�b�g���g��
	//	
	wp<EmitterSet> Create(eEFFECT_TYPE Type);
	wp<EmitterSet> Create( eEFFECT_TYPE Type, const Vector3& Position );
	wp<EmitterSet> Create( eEFFECT_TYPE Type, const Vector3& Position,
		float Scale );
	//void Create( eEFFECT_TYPE Type, const Vector3& Position, const Vector3& VelocityTarget );

	EffectManager(){Clear();}void Clear();
	~EffectManager(){Destroy();}void Destroy();
private:
	void Setting_Texture();
	void Create_EmitterSet(eEFFECT_TYPE Type,int Num);
	sp<EmitterSet>	m_spEffectEmitterSet[EMITTERSET_MAX];
	int m_UseNum;
};





//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	Particle_Singleton
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class EffectManager_Singleton : public EffectManager
{
protected:
	static EffectManager_Singleton* singleton;

	EffectManager_Singleton(){}
	virtual ~EffectManager_Singleton(){}
public:
	static EffectManager_Singleton* getInstance()
	{
		if( singleton == NULL ){
			singleton = new EffectManager_Singleton;
		}
		return singleton;
	}
	static void deleteInstance(){
		if( singleton != NULL ){
			delete singleton;

			singleton = NULL;
		}
	}
};

#define sEffectManager ( EffectManager_Singleton::getInstance() )





}//EffectLib


#endif