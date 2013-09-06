#include "GameCommonPipeline.h"

using namespace rlib;

klib::kGraphicsPipline* GameCommonPipeline::pipeline = NULL;

void GameCommonPipeline::init()
{
	pipeline = new klib::kGraphicsPipline();
	//pipeline->createVertexShader("kanetaPlace/shader/vertex.txt");
	//pipeline->createPixelShader("kanetaPlace/shader/pixel.txt");
	pipeline->createShader("kanetaPlace/shader/vertex.txt","kanetaPlace/shader/pixel.txt");
	pipeline->createBlendState(klib::k_BLEND_NONE);
	pipeline->createDepthStencilState(true,true,klib::eLESS_EQUAL);
	pipeline->createRasterizerState(klib::eSOLID,klib::eFRONT,false);

	klib::kInputElementDesc desc[]=
	{
		{"POSITION",0,klib::k_VF_R32G32B32_FLOAT,0,klib::eVertex,0},
		{"COLOR",0,klib::k_VF_R32G32B32A32_FLOAT,0,klib::eVertex,0},
		{"NORMAL",0,klib::k_VF_R32G32B32_FLOAT,0,klib::eVertex,0},
		{"TEXCOORD",0,klib::k_VF_R32G32_FLOAT,0,klib::eVertex,0}
	};
	klib::u32 descnum=sizeof(desc)/sizeof(klib::kInputElementDesc);
	pipeline->complete(desc,descnum);
}

void GameCommonPipeline::clear()
{
	if( pipeline ) { delete pipeline; pipeline = NULL; }
}
