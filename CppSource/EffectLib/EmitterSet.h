#ifndef __teEmitterSet_H__
#define __teEmitterSet_H__
#include	"math\\kmathf.h"
#include	"StandardLib\\SmartPointer.h"
#include	"EffectLib\\Emitter.h"
#include	"utility\textLoader.h"

namespace EffectLib{
using namespace klib::math;
static const int EFFECT_EMITTER_MAX = 256;


//----------------------------------------------------------------------
//	EmitterSet
//----------------------------------------------------------------------
class EmitterSet
{
public:
	//	Setting �e��p�����[�^��ݒ�
	void Setting_Position(const Vector3& Pos);

	//	Emitter���E�������Ȃ���
	void Loop();

	/*
		Emitter���I��

		wp�ł��炤���͎��Ԃ����邩�ǂ����m�F���Ă�������
		if( true == wp.IsExist() ){  }
	*/
	void End();
	
	static void Setting_Texture(char* Filename);

	//===========================================================
	//				������������  �C�ɂ��Ȃ���OK  ������������	[�g�p�֎~]
	//===========================================================
	//	Create
	void Create( const Vector3& Position );
	void Create( const Vector3& Position, const Vector3& VelosityTarget );
	void Setting_Scale(float Scale);
	//	Initialize
	bool Initialize(char* File);
	//	Update
	bool Update();
	//	Render
	void Render();
	//	BasicSet
	EmitterSet(){Clear();}void Clear();
	virtual ~EmitterSet(){Destroy();}void Destroy();
protected:
	//	Load
	void Load_TES( char* Filename );
	//	List
	sp<Emitter>	m_spEffectEmitter[EFFECT_EMITTER_MAX];
	//	Data
	int							m_Frame;
	int							m_UseNum;
	sp<textLoader>	m_spTextLoader;
};



}//EffectLib

#endif
