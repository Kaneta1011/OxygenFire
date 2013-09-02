#ifndef _R_2DOBJ_
#define _R_2DOBJ_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "math/kmathf.h"

#include "GraphicsLib\Class\rTexture\Texture.h"
#include "r2DRenderer.h"

#include "GraphicsLib\Class\tRenderState\RenderState.h"

namespace klib
{
	class kGraphicsPipline;
}

namespace rlib
{
	class FrameBuffer;

	/*
	ƒŠƒbƒ`‚È2D•`‰æ
	•`‰æˆÊ’u‚ÌÀ•WŒn‚ÍƒvƒƒWƒFƒNƒVƒ‡ƒ“À•WŒn‚Å‚·
	(‰æ–Ê‚Ì”ÍˆÍ(-100(¶’[) ` x ` 100(‰E’[)) (-100(‰º) ` y ` 100(ã)) )
	‰ñ“]‚Í‚Ü‚¾
	*/
	class r2DObj : public r2DRenderer
	{
	public:
		r2DObj();
		~r2DObj();

		void clear();

		bool load(const char* filePath);

		void render();
		void render(rlib::FrameBuffer* frameBuffer);

		void render(klib::kTechnique* tec);
		void render(klib::kTechnique* tec,rlib::FrameBuffer* frameBuffer);
	public:
		GLuint getTextureID()const{return mpTexture->getID();}
	protected:
		//virtual void innerRender(Texture* pTex);

	protected:
		Texture*		mpTexture;

	};
}
#endif