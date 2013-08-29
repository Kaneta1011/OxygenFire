#include "../r2DRenderer.h"

#include "GraphicsLib\Class\rTexture\Texture.h"
#include "GraphicsLib\Class\rFrameBufer\rFrameBuffer.h"

#include "utility\utility.h"

using namespace rlib;

r2DRenderer::r2DRenderer():
mIsUpdate(true),
mPos(0,0,0),
mSize(50.f,50.f),
mCenterType(DRAW_CENTER)
{
}

void r2DRenderer::initBuf()
{
	initPosBuf();
	initTexBuf();
}

void r2DRenderer::initPosBuf()
{
	//位置データ
	mPosBuf[0].x = 0.f; mPosBuf[0].y = 0.f; mPosBuf[0].z = 0.f;
	mPosBuf[1].x = 1.f; mPosBuf[1].y = 0.f; mPosBuf[1].z = 0.f;
	mPosBuf[2].x = 0.f; mPosBuf[2].y = 1.f; mPosBuf[2].z = 0.f;
	mPosBuf[3].x = 1.f; mPosBuf[3].y = 1.f; mPosBuf[3].z = 0.f;
}

void r2DRenderer::initTexBuf()
{
	//UV座標
	mTexBuf[0].x = 0.f; mTexBuf[0].y = 1.f;
	mTexBuf[1].x = 1.f; mTexBuf[1].y = 1.f;
	mTexBuf[2].x = 0.f; mTexBuf[2].y = 0.f;
	mTexBuf[3].x = 1.f; mTexBuf[3].y = 0.f;
}

void r2DRenderer::render(rlib::Texture* pTex)
{
	innerRender(pTex);
}

void r2DRenderer::innerRender(rlib::Texture* pTex)
{
	update();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	RenderLib::Shader& shader = r2DPipeline::getShader();
	//shader.SetValue("isFrameBuffer", 1.f);
	shader.Begin();
	glVertexAttribPointer(S_POS,3,GL_FLOAT, GL_FALSE,0,&this->mPosBuf);
	glVertexAttribPointer(S_TEX,2,GL_FLOAT, GL_FALSE,0,&this->mTexBuf);
	glEnableVertexAttribArray(S_POS);
	glEnableVertexAttribArray(S_TEX);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glEnable(GL_TEXTURE_2D);
	pTex->Setting(Texture::ACTIVE_0);
	shader.SetValue_No_BeginEnd("colorTex", 0);

	GLuint count = sizeof(this->mPosBuf)/sizeof(this->mPosBuf[0]);
	glDrawArrays(GL_TRIANGLE_STRIP,0,count);

	shader.End();
}

void r2DRenderer::update()
{
	if( !this->mIsUpdate ) return ;

	float rate = 0.01f;
	float x = r2DHelper::toRenderCoord(this->mPos.x), y = r2DHelper::toRenderCoord(this->mPos.y), z = this->mPos.z;
	float sx = r2DHelper::toRenderCoord(this->mSize.x), sy = r2DHelper::toRenderCoord(this->mSize.y);

	this->mPosBuf[0].x = x;			this->mPosBuf[0].y = y-sy;		this->mPosBuf[0].z = z;
	this->mPosBuf[1].x = x+sx;		this->mPosBuf[1].y = y-sy;		this->mPosBuf[1].z = z;
	this->mPosBuf[2].x = x;			this->mPosBuf[2].y = y;			this->mPosBuf[2].z = z;
	this->mPosBuf[3].x = x+sx;		this->mPosBuf[3].y = y;			this->mPosBuf[3].z = z;

	float offsetX = 0.f;
	float offsetY = 0.f;
	getDrawOffset(&offsetX, &offsetY);
	for( int i=0; i<4; i++ )
	{
		this->mPosBuf[i].x += offsetX;
		this->mPosBuf[i].y += offsetY;
	}

	this->mIsUpdate = false;
}

void r2DRenderer::setTexBuf(bool isFrameBuffer)
{
	mTexBuf[0].x = 0.f;
	mTexBuf[1].x = 1.f;
	mTexBuf[2].x = 0.f;
	mTexBuf[3].x = 1.f;

	if( isFrameBuffer ){
		mTexBuf[0].y = 0.f;
		mTexBuf[1].y = 0.f;
		mTexBuf[2].y = 1.f;
		mTexBuf[3].y = 1.f;
	}else{
		mTexBuf[0].y = 1.f;
		mTexBuf[1].y = 1.f;
		mTexBuf[2].y = 0.f;
		mTexBuf[3].y = 0.f;
	}
}

void r2DRenderer::getDrawOffset(float* x, float *y)
{
	static float offsetRate[DRAW_CENTER_TYPE_MAX][2]={
		{ -0.0f, 0.0f },		//DRAW_TOP_LEFT
		{ -0.5f, 0.5f },		//DRAW_CENTER
	};

	*x = offsetRate[this->mCenterType][0] * r2DHelper::toRenderCoord( this->mSize.x );
	*y = offsetRate[this->mCenterType][1] * r2DHelper::toRenderCoord( this->mSize.y );
}


//================================================================
//
//		r2DPipelineクラス
//
//================================================================
#include "Ueda\TmpShader\ShaderManager.h"

klib::kGraphicsPipline*	r2DPipeline::spPipeline = NULL;
RenderLib::Shader*		r2DPipeline::spShader = NULL;

void r2DPipeline::init()
{
	clear();
	spPipeline = new klib::kGraphicsPipline();

	spShader = new RenderLib::Shader();
	ShaderLib::ShaderManager::Create_Shader(&spShader, "Shader/r2d.vs","Shader/r2d.fs");
}

void r2DPipeline::clear()
{
	if( spPipeline ){delete spPipeline ; spPipeline = NULL;}
	if( spShader ){ delete spShader; spShader = NULL; }
}
