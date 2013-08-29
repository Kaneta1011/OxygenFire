#ifndef __teTexture_H__
#define __teTexture_H__
#include <GLES2/gl2.h>

namespace rlib{

	class Texture
	{
	public:
		enum eACTIVE_TYPE
		{
			ACTIVE_0, ACTIVE_1, ACTIVE_2, ACTIVE_3, ACTIVE_4,
			ACTIVE_5, ACTIVE_6, ACTIVE_7, ACTIVE_8, ACTIVE_9, 
			ACTIVE_10,ACTIVE_11,ACTIVE_12,ACTIVE_13,ACTIVE_14,
			ACTIVE_15,ACTIVE_16,ACTIVE_17,ACTIVE_18,ACTIVE_19,
			ACTIVE_20,ACTIVE_21,ACTIVE_22,ACTIVE_23,ACTIVE_24,
			ACTIVE_25,ACTIVE_26,ACTIVE_27,ACTIVE_28,ACTIVE_29,
			ACTIVE_30,ACTIVE_31,
		};

	public:
		/*
		assetsから画像を読み込む
		*/
		void Initilize(const char* Filename);
		/*
		・空のテクスチャを作成する
		・引数
			width, height	: 画像サイズ
			format			: GL_RGBなどのカラーデータの形式.	default=GL_RGBA
			bitOrder		: formatのデータの区切り.			default=GL_UNSIGNED_SHORT_4_4_4_4
		*/
		void Initilize(int width, int height, GLenum format=GL_RGBA, GLenum bitOrder=GL_UNSIGNED_SHORT_4_4_4_4);

		void Setting(eACTIVE_TYPE Type)const;
		Texture(){m_Id=0;}
		~Texture(){glDeleteTextures(1, &m_Id);}

	public:
		GLuint getID()const{return this->m_Id;}

	private:
		GLuint	m_Id;
	};

}//RenderLib

#endif