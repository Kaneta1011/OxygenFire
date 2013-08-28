#include "Texture.h"

#include "utility\utility.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <assert.h>
#include "utility\assetsLoader.h"

using namespace rlib;

static const char* TAG = "rlib::Texture";

void Texture::Initilize(const char* Filename)
{
	JNIEnv* env;
	getJNIEnv(&env);

	AssetsLoader::loadTexture(&m_Id,Filename,env);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

void Texture::Initilize(int width, int height, GLenum format, GLenum bitOrder)
{
	glGenTextures(1, &this->m_Id);
	glBindTexture(GL_TEXTURE_2D, this->m_Id);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, bitOrder, 0);
	checkGlError(TAG, "glTexImage2D");
}

void Texture::Setting(eACTIVE_TYPE Type)const
{
	GLenum index = GL_TEXTURE0 + Type;
	glActiveTexture(index);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_Id);
}
