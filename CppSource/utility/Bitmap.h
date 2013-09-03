#ifndef _BITMAP_
#define _BITMAP_

#include <jni.h>
#include <GLES2/gl2.h>

namespace rlib
{
	class Bitmap
	{
	public:
		Bitmap();
		~Bitmap();

		void clear();
		bool load(const char* fileName, GLint format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

		char* lock();
		void  unlock();

		//
		//	OpenGLが走っているスレッドで使ってください
		//
		void genTexture(GLuint* outTexture);

	public:
		int getWidth()const{return this->mWidth;}
		int getHeight()const{return this->mHeight;}
		GLint getFormat()const{return this->mFormat;}
		GLint getType()const{return this->mType;}

	private:
		int mWidth, mHeight;
		GLint mFormat, mType;

		jobject mBitmap;
	};

}

#endif