#include "../r2DObj.h"

#include "Ueda\TmpShader\ShaderManager.h"
#include "GraphicsLib\Class\rFrameBufer\rFrameBuffer.h"
#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"

using namespace klib::math;
using namespace ShaderLib;
using namespace rlib;

r2DObj::r2DObj():
mpTexture(NULL)
{
}

r2DObj::~r2DObj()
{
	clear();
}

void r2DObj::clear()
{
	if( this->mpTexture ){ delete this->mpTexture; this->mpTexture = NULL; }
}

bool r2DObj::load(const char* filePath)
{
	clear();
	initBuf();

	this->mpTexture = new Texture();
	this->mpTexture->Initilize(filePath);
}

void r2DObj::render()
{
	innerRender(this->mpTexture);
}

void r2DObj::render(rlib::FrameBuffer* frameBuffer)
{
	update();
	setTexBuf(true);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	RenderLib::Shader& shader = r2DPipeline::getShader();
	//shader.SetValue("isFrameBuffer", -1.f);
	shader.Begin();
	glVertexAttribPointer(S_POS,3,GL_FLOAT, GL_FALSE,0,&this->mPosBuf);
	glVertexAttribPointer(S_TEX,2,GL_FLOAT, GL_FALSE,0,&this->mTexBuf);
	glEnableVertexAttribArray(S_POS);
	glEnableVertexAttribArray(S_TEX);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	frameBuffer->bindColorTex(Texture::ACTIVE_0);
	shader.SetValue_No_BeginEnd("colorTex", 0);

	GLuint count = sizeof(this->mPosBuf)/sizeof(this->mPosBuf[0]);
	glDrawArrays(GL_TRIANGLE_STRIP,0,count);

	shader.End();
}

