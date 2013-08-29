#include "../r2DObj.h"

#include "Ueda\TmpShader\ShaderManager.h"
#include "GraphicsLib\Class\rFrameBufer\rFrameBuffer.h"
#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"

using namespace klib;
using namespace math;
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
	kTechnique& sh=r2DPipeline::getPipeline();

	sh.setTechnique();
	sh.setTexture("colorTex", 0,&frameBuffer->gexColorTexture());

	glVertexAttribPointer(S_POS,3,GL_FLOAT, GL_FALSE,0,&this->mPosBuf);
	glVertexAttribPointer(S_TEX,2,GL_FLOAT, GL_FALSE,0,&this->mTexBuf);
	glEnableVertexAttribArray(S_POS);
	glEnableVertexAttribArray(S_TEX);

	GLuint count = sizeof(this->mPosBuf)/sizeof(this->mPosBuf[0]);
	glDrawArrays(GL_TRIANGLE_STRIP,0,count);

}

void r2DObj::render(kTechnique* tec)
{
	update();
	//setTexBuf(true);

	tec->setTexture("colorTex",0,this);
	tec->setTechnique();

	glVertexAttribPointer(S_POS,3,GL_FLOAT, GL_FALSE,0,&this->mPosBuf);
	glVertexAttribPointer(S_TEX,2,GL_FLOAT, GL_FALSE,0,&this->mTexBuf);
	glEnableVertexAttribArray(S_POS);
	glEnableVertexAttribArray(S_TEX);

	GLuint count = sizeof(this->mPosBuf)/sizeof(this->mPosBuf[0]);
	glDrawArrays(GL_TRIANGLE_STRIP,0,count);
}


void r2DObj::render(kTechnique* tec,rlib::FrameBuffer* frameBuffer)
{
	update();
	setTexBuf(true);

	tec->setTexture("colorTex",0,&frameBuffer->gexColorTexture());
	tec->setTechnique();

	glVertexAttribPointer(S_POS,3,GL_FLOAT, GL_FALSE,0,&this->mPosBuf);
	glVertexAttribPointer(S_TEX,2,GL_FLOAT, GL_FALSE,0,&this->mTexBuf);
	glEnableVertexAttribArray(S_POS);
	glEnableVertexAttribArray(S_TEX);

	GLuint count = sizeof(this->mPosBuf)/sizeof(this->mPosBuf[0]);
	glDrawArrays(GL_TRIANGLE_STRIP,0,count);
}
