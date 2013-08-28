#ifndef _R_FRAME_BUFFER_
#define _R_FRAME_BUFFER_

#include <GLES2\gl2.h>

namespace rlib
{
	class FrameBuffer
	{
	public:
		/*
		・スタティック関数
		・バックバッファをレンダーターゲットに設定する
		*/
		static void bindScreenBuffer(float r=0.1f, float g=0.125f, float b=0.3f, float a=1.f);

	public:
		FrameBuffer();
		~FrameBuffer();

		void clear();
		/*
		・フレームバッファオブジェクトを作成する
		・引数
			width, height	: 画像サイズ
			format			: GL_RGBなどのカラーデータの形式.	default=GL_RGBA
			bitOrder		: formatのデータの区切り.			default=GL_UNSIGNED_SHORT_4_4_4_4
			depthFormat		: 深度バッファの形式.				default=GL_DEPTH_COMPONENT16
		*/
		void init( int width, int height, GLenum format=GL_RGBA, GLenum bitOrder=GL_UNSIGNED_SHORT_4_4_4_4, GLenum depthFormat=GL_DEPTH_COMPONENT16 );

		/*
		・フレームバッファをレンダーターゲットに設定する
		・中でバッファのクリアとビューポートの設定をしています
		*/
		void bind(float r=0.1f, float g=0.125f, float b=0.3f, float a=1.f);

	public:
		void bindColorTex(GLenum activeIndex);

	private:
		int width, height;

		GLuint mFB;
		GLuint mColor;
		GLuint mDepth;
	};
}

#endif