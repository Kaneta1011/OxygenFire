#ifndef _R_POINT_SPRITE_
#define _R_POINT_SPRITE_

#include "math/kmathf.h"
#include "GraphicsLib\Class\rTexture\Texture.h"
#include "Ueda\TmpShader\Shader.h"

#include <GLES2\gl2.h>
#include <GLES2/gl2ext.h>

namespace rlib{
	//==============================================================================================
	//
	//	PointSprite class
	//		・点情報からスプライトを描画するクラス
	//		・描画数は7500個ぐらいが、安定して描画できる数みたい
	//
	//==============================================================================================
	class PointSprite
	{
	public:
		enum DRAW_OFFSET_TYPE{
			OFFSET_CENTER,
			OFFSET_TOP_LEFT,
		};
	public:
		typedef Texture myTexture;
		static void init();
		static void clear();

		//
		//	arrayDrawPosの値は0.f～1.fの範囲をとるようにしてください
		//	・画面サイズが異なる機種間での互換を保つためにしています
		//	　イメージはUV座標でしてする感じです
		//	・sizeは0.f～100.fの間になるようになってます。
		//
		static void render(klib::math::Vector3* arrayDrawPos, int count, myTexture* texture, float size, DRAW_OFFSET_TYPE offsetType = OFFSET_CENTER);

	private:
		enum eS_TYPE
		{
			S_POS,
			S_MAX,
		};

	private:
		static float getDrawOffsetX(DRAW_OFFSET_TYPE type);
		static float getDrawOffsetY(DRAW_OFFSET_TYPE type);

	private:
		static RenderLib::Shader*	m_spSprite;

	private:
		PointSprite(){}
		PointSprite(PointSprite&){}
		void operator=(PointSprite&){}
		~PointSprite(){}
	};

}
#endif