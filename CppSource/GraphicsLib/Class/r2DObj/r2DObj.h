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
	リッチな2D描画
	描画位置の座標系はプロジェクション座標系です
	(画面の範囲(-100(左端) 〜 x 〜 100(右端)) (-100(下) 〜 y 〜 100(上)) )
	回転はまだ
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