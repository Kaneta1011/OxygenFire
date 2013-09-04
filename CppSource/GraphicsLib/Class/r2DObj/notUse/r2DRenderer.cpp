#include "../r2DRenderer.h"

#include "GraphicsLib\Class\rTexture\Texture.h"
#include "GraphicsLib\Class\rFrameBufer\rFrameBuffer.h"

#include "utility\utility.h"

using namespace rlib;
using namespace klib;

r2DRenderer::r2DRenderer():
mIsUpdate(true),
mPos(0,0,0),
mSize(r2DHelper::adjustSize(50.f, false)),
mCenterType(DRAW_CENTER),
mColor(1,1,1,1)
{
}

void r2DRenderer::initBuf()
{
	glGenBuffers( S_MAX, this->mBuf);
	initPosBuf();
	initTexBuf();
}

void r2DRenderer::initPosBuf()
{
	glBindBuffer( GL_ARRAY_BUFFER, this->mBuf[S_POS] );
	//位置データ
	mPosBuf[0].x = 0.f; mPosBuf[0].y = 0.f; mPosBuf[0].z = 0.f;
	mPosBuf[1].x = 1.f; mPosBuf[1].y = 0.f; mPosBuf[1].z = 0.f;
	mPosBuf[2].x = 0.f; mPosBuf[2].y = 1.f; mPosBuf[2].z = 0.f;
	mPosBuf[3].x = 1.f; mPosBuf[3].y = 1.f; mPosBuf[3].z = 0.f;
	glBufferData( GL_ARRAY_BUFFER, sizeof(this->mPosBuf), this->mPosBuf, GL_DYNAMIC_DRAW );
}

void r2DRenderer::initTexBuf()
{
	glBindBuffer( GL_ARRAY_BUFFER, this->mBuf[S_TEX] );
	//UV座標
	mTexBuf[0].x = 0.f; mTexBuf[0].y = 1.f;
	mTexBuf[1].x = 1.f; mTexBuf[1].y = 1.f;
	mTexBuf[2].x = 0.f; mTexBuf[2].y = 0.f;
	mTexBuf[3].x = 1.f; mTexBuf[3].y = 0.f;
	glBufferData( GL_ARRAY_BUFFER, sizeof(this->mTexBuf), this->mTexBuf, GL_DYNAMIC_DRAW );
}

void r2DRenderer::render(rlib::Texture* pTex)
{
	innerRender(pTex);
}

void r2DRenderer::innerRender(rlib::Texture* pTex)
{
	update();

	kTechnique& sh=r2DPipeline::getPipeline();
	sh.setTechnique();
	sh.setShaderValue("baseColor", this->mColor);
	sh.setTexture("colorTex", 0,pTex);
	glVertexAttribPointer(S_POS,3,GL_FLOAT, GL_FALSE,0,&this->mPosBuf);
	glVertexAttribPointer(S_TEX,2,GL_FLOAT, GL_FALSE,0,&this->mTexBuf);
	glEnableVertexAttribArray(S_POS);
	glEnableVertexAttribArray(S_TEX);

	GLuint count = sizeof(this->mPosBuf)/sizeof(this->mPosBuf[0]);
	glDrawArrays(GL_TRIANGLE_STRIP,0,count);

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
kTechnique*	r2DPipeline::spPipeline = NULL;

void r2DPipeline::init()
{
	clear();
	spPipeline = new kTechnique();
	spPipeline->createVertexShader("shader/r2d.vs");
	spPipeline->createPixelShader("shader/r2d.fs");
	spPipeline->bindAttribLocation(0,"Pos");
	spPipeline->bindAttribLocation(1,"Tex");
	spPipeline->createBlendState(k_BLEND_ALPHA);
	spPipeline->createDepthStencilState(true,true,eLESS_EQUAL);
	spPipeline->createRasterizerState(eWIRE,eNONE,false);
	spPipeline->complete();
}

void r2DPipeline::clear()
{
	if( spPipeline ){delete spPipeline ; spPipeline = NULL;}
}
