#ifndef __teEmitterSet_H__
#define __teEmitterSet_H__
#include	"math\\kmathf.h"
#include	"StandardLib\\SmartPointer.h"
#include	"EffectLib\\Emitter.h"
#include	"utility\textLoader.h"
#include	<list>

namespace EffectLib{
using namespace klib::math;
static const int EFFECT_EMITTER_MAX = 512;


//----------------------------------------------------------------------
//	EmitterSet
//----------------------------------------------------------------------
class EmitterSet
{
public:
	//	Initialize
	bool Initialize(char* File);
	//	Update
	bool Update();
	//	Render
	void Render();
	//	Create
	void Create( const Vector3& Position );
	void Create( const Vector3& Position, const Vector3& VelosityTarget );
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
	sp<textLoader>	m_spTextLoader;
};



}//EffectLib

#endif
