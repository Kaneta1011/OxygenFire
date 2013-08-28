#include "rPointSprite.h"

#include "../../../ShaderLib/ShaderManager.h"
#include "../../../utility/utility.h"
#include "../../../RenderLib/RenderState.h"

using namespace rlib;

RenderLib::Shader*	PointSprite::m_spSprite = NULL;

static const char* TAG = "rlib::PointSprite";

void PointSprite::init()
{
	ShaderLib::ShaderManager::Create_Shader(&m_spSprite, "Shader/Sprite.vs","Shader/Sprite.fs");

}

void PointSprite::clear()
{
	if( m_spSprite ){ delete m_spSprite; m_spSprite = NULL; }
}

void PointSprite::render(klib::math::Vector3* arrayDrawPos, int count, myTexture* texture, float size, DRAW_OFFSET_TYPE offsetType)
{
	//if( size > 100.f ) size = 100.f;
	m_spSprite->SetValue("size",size);
	m_spSprite->SetValue("offsetX",getDrawOffsetX(offsetType));
	m_spSprite->SetValue("offsetY",getDrawOffsetY(offsetType));

	m_spSprite->Begin();

	glVertexAttribPointer(S_POS,3,GL_FLOAT, GL_FALSE,0, arrayDrawPos);
	glEnableVertexAttribArray(S_POS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	//glEnable(GL_TEXTURE_2D);
	//texture->Setting(RenderLib::ACTIVE_0);
	//m_spSprite->SetValue_No_BeginEnd("uTex",0);

	glDrawArrays(GL_POINTS,0,count);

	m_spSprite->End();
}

float	PointSprite::getDrawOffsetX(DRAW_OFFSET_TYPE type)
{
	static const float table[]={
		0.f,										//OFFSET_CENTER
		1.f/RenderLib::RenderState::getScreenWidth()//OFFSET_TOP_LEFT
	};
	return table[type];
}

float	PointSprite::getDrawOffsetY(DRAW_OFFSET_TYPE type)
{
	static const float table[]={
		0.f,											//OFFSET_CENTER
		1.f/RenderLib::RenderState::getScreenHeight()	//OFFSET_TOP_LEFT
	};
	return table[type];
}

