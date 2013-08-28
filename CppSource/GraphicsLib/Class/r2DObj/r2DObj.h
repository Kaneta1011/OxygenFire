#ifndef _R_2DOBJ_
#define _R_2DOBJ_

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "math/kmathf.h"

#include "GraphicsLib\Class\rTexture\Texture.h"
#include "r2DRenderer.h"

#include "GraphicsLib\Class\tRenderState\RenderState.h"

#include "Ueda\TmpShader\Shader.h"

namespace klib
{
	class kGraphicsPipline;
}

namespace rlib
{
	class FrameBuffer;

	/*
	���b�`��2D�`��
	�`��ʒu�̍��W�n�̓v���W�F�N�V�������W�n�ł�
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
	public:

	protected:
		//virtual void innerRender(Texture* pTex);

	protected:
		Texture*		mpTexture;

	};
}
#endif