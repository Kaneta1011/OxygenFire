#ifndef __singleParticle_H__
#define __singleParticle_H__
#include	"math\\kmathf.h"
#include	"GraphicsLib\Class\rTexture\Texture.h"
#include	"StandardLib\\SmartPointer.h"
#include	"EffectLib\\Item.h"
#include	"GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"
#include	"GraphicsLib\Class\kInputLayout\kInputLayout.h"


#include	<GLES2/gl2.h>
#include	<GLES2/gl2ext.h>
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;

namespace EffectLib{

using namespace rlib;
using namespace klib::math;


class singleParticle 
{
public:
	//	çÏê¨
	inline void create( const Vector3& pos , const Vector3& move , 
		int life, sp<Texture> tex )
	{
		this->pos = pos;
		this->move = move;
		this->life = life;
	}

	//	ÉxÉNÉgÉã
	inline void setting_move(const Vector3& move)
	{
		this->move = move;
	}

	inline void update()
	{
		pos += move;
	}

	static void render()
	{

	}
	singleParticle():pos(Vector3(0,0,0)),vec(Vector3(0,0,0)),life(0){}
private:
	Vector3 pos;
	Vector3 move;
	int life;
};

#endif