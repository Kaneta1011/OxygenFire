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
	//	Setting 各種パラメータを設定
	void Setting_Position(const Vector3& Pos);

	//	Emitterを殺したくない時
	void Loop();

	/*
		Emitterを終了

		wpでもらう時は実態があるかどうか確認してください
		if( true == wp.IsExist() ){  }
	*/
	void End();
	
	static void Setting_Texture(char* Filename);

	//===========================================================
	//				↓↓↓↓↓↓  気にしなくてOK  ↓↓↓↓↓↓	[使用禁止]
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
