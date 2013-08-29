#include "rFrameBuffer.h"

#include "GraphicsLib\Class\tRenderState\RenderState.h"
#include "utility/utility.h"

using namespace rlib;

static const char* TAG = "FrameBuffer";

void FrameBuffer::bindScreenBuffer(float r, float g, float b, float a)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//ビューポートを設定
	glViewport(0, 0, RenderLib::RenderState::getScreenWidth(), RenderLib::RenderState::getScreenHeight());
	glClearColor(r , g , b , a);
	//カラーバッファをクリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

FrameBuffer::FrameBuffer():
width(0), height(0),
mFB(0),
mDepth(0)
{
}

FrameBuffer::~FrameBuffer()
{
	clear();
}

void FrameBuffer::clear()
{
	glDeleteFramebuffers(1, &this->mFB);
	glDeleteRenderbuffers(1, &this->mDepth);
}

void FrameBuffer::init( int width, int height, GLenum format, GLenum bitOrder, GLenum depthFormat )
{
	clear();

	this->width = width; this->height = height;
	//フレームバッファオブジェクトのためのカラーテクスチャ
	this->mColorTex.Initilize(width, height, format, bitOrder);

	//フレームバッファオブジェクトのための深度テクスチャ
	glGenRenderbuffers(1, &this->mDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, this->mDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, depthFormat, width, height);
	checkGlError(TAG, "glRenderbufferStorage");

	//フレームバッファオブジェクトの作成
	glGenFramebuffers(1, &this->mFB);
	glBindFramebuffer(GL_FRAMEBUFFER, this->mFB);

	//カラーと深度の関連付け
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->mColorTex.getID(), 0);
	checkGlError(TAG, "glFramebufferTexture2D");
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->mDepth);
	checkGlError(TAG, "glFramebufferRenderbuffer");

	LOGI(TAG, "Finish create frame buffer");

}

void FrameBuffer::bind(float r, float g, float b, float a)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, this->mFB);

	glViewport(0, 0, width, height);
	glClearColor(r , g , b , a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::bindColorTex(unsigned int activeIndex)
{
	if( activeIndex < Texture::ACTIVE_0 || Texture::ACTIVE_31 < activeIndex )
	{
		LOGE(TAG, "Invalid index of active texture");
		abort();
	}
	this->mColorTex.Setting((Texture::eACTIVE_TYPE)activeIndex);
}
