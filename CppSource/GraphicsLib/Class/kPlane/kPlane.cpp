#include "kPlane.h"
#include "GraphicsLib\Class\kDevice\kDevice.h"
#include "GraphicsLib\Class\tRenderState\RenderState.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"

#include "GraphicsLib\Class\kObjectBuffer\kObjectBuffer.h"
#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"

namespace klib
{
	using namespace math;
	kObjectBuffer* kPlane::m_VBO=NULL;
	kObjectBuffer* kPlane::m_IBO=NULL;

	static planeVertex planeV[4]=
	{
		{Vector3(0.5f,0.5f,0),Vector2(0,0)},		//右上
		{Vector3(-0.5f,0.5f,0),Vector2(1,0)},		//左上
		{Vector3(0.5f,-0.5f,0),Vector2(0,1)},		//右下
		{Vector3(-0.5f,-0.5f,0),Vector2(1,1)}	//左下
	};
	static u16 planeI[6]=
	{
		0,1,2,1,2,3
	};

	void kPlane::init()
	{

		m_VBO=new kObjectBuffer;
		m_IBO=new kObjectBuffer;
		kDevice::createVertexBuffer(m_VBO,planeV,sizeof(planeVertex)*4,GL_STATIC_DRAW);
		kDevice::createIndexBuffer(m_IBO,planeI,sizeof(u16)*6,GL_STATIC_DRAW);
	}
	void kPlane::release()
	{
		delete m_VBO;
		delete m_IBO;
	}

	void kPlane::render(kGraphicsPipline* pipline,rlib::r2DObj* tex,f32 angle,f32 sx,f32 sy,const Vector3& pos,f32 offsetx,f32 offsety,f32 width,f32 height)
	{
		//姿勢行列の作成
		Matrix rotz;
		rotz.identity();
		rotz.setRZ(angle);

		//ビルボード行列の作成
		Matrix billbordMat=RenderLib::RenderState::getViewMatrix();
		billbordMat.transpose();
		billbordMat.decomposeR(&billbordMat);
		billbordMat._11=-billbordMat._11;billbordMat._12=-billbordMat._12;billbordMat._13=-billbordMat._13;
		billbordMat._31=-billbordMat._31;billbordMat._32=-billbordMat._32;billbordMat._33=-billbordMat._33;
		billbordMat._41=pos.x;billbordMat._42=pos.y;billbordMat._43=pos.z;
		billbordMat.mulS(sx,sy,0.0f);

		//モデル行列作成(テスト用)
		math::Matrix wvp = 
			rotz*
			billbordMat * 
			RenderLib::RenderState::getViewMatrix() *
			RenderLib::RenderState::getProjectionMatrix();
		pipline->setTexture("colorTex",0,tex);
		pipline->setShaderValue("WVP",wvp);
		//頂点バッファをパイプラインにセットする
		kDevice::IAsetVertexBuffer(m_VBO);
		//インデックスバッファをパイプラインにセットする
		kDevice::IAsetIndexBuffer(m_IBO);
		//シェーダーをパイプラインにセットする
		pipline->setPipline();
		kDevice::drawIndexed(6);
	}

	void kPlane::render(kGraphicsPipline* pipline,rlib::r2DObj* tex,f32 angle,f32 sx,f32 sy,const Vector3& pos,const Vector3& norm,f32 offsetx,f32 offsety,f32 width,f32 height)
	{
		//姿勢行列の作成
		Matrix rotz;
		rotz.identity();
		rotz.setRZ(angle);
		Matrix mat=RenderLib::RenderState::getViewMatrix();
		MatrixFrontVec(&mat,pos,norm,Vector3(0,1,0));
		mat.mulS(sx,sy,0.0f);
		mat=rotz*mat;
		//モデル行列作成(テスト用)
		math::Matrix wvp = 
			rotz*
			mat * 
			RenderLib::RenderState::getViewMatrix() *
			RenderLib::RenderState::getProjectionMatrix();
		pipline->setTexture("colorTex",0,tex);
		pipline->setShaderValue("WVP",wvp);
		//頂点バッファをパイプラインにセットする
		kDevice::IAsetVertexBuffer(m_VBO);
		//インデックスバッファをパイプラインにセットする
		kDevice::IAsetIndexBuffer(m_IBO);
		//シェーダーをパイプラインにセットする
		pipline->setPipline();
		kDevice::drawIndexed(6);
	}
}