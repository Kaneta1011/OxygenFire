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
		{Vector3(0.5f,0.5f,0),Vector2(0,0)},		//�E��
		{Vector3(-0.5f,0.5f,0),Vector2(1,0)},		//����
		{Vector3(0.5f,-0.5f,0),Vector2(0,1)},		//�E��
		{Vector3(-0.5f,-0.5f,0),Vector2(1,1)}	//����
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
		//�p���s��̍쐬
		Matrix rotz;
		rotz.identity();
		rotz.setRZ(angle);

		//�r���{�[�h�s��̍쐬
		Matrix billbordMat=RenderLib::RenderState::getViewMatrix();
		billbordMat.transpose();
		billbordMat.decomposeR(&billbordMat);
		billbordMat._11=-billbordMat._11;billbordMat._12=-billbordMat._12;billbordMat._13=-billbordMat._13;
		billbordMat._31=-billbordMat._31;billbordMat._32=-billbordMat._32;billbordMat._33=-billbordMat._33;
		billbordMat._41=pos.x;billbordMat._42=pos.y;billbordMat._43=pos.z;
		billbordMat.mulS(sx,sy,0.0f);

		//���f���s��쐬(�e�X�g�p)
		math::Matrix wvp = 
			rotz*
			billbordMat * 
			RenderLib::RenderState::getViewMatrix() *
			RenderLib::RenderState::getProjectionMatrix();
		pipline->setTexture("colorTex",0,tex);
		pipline->setShaderValue("WVP",wvp);
		//���_�o�b�t�@���p�C�v���C���ɃZ�b�g����
		kDevice::IAsetVertexBuffer(m_VBO);
		//�C���f�b�N�X�o�b�t�@���p�C�v���C���ɃZ�b�g����
		kDevice::IAsetIndexBuffer(m_IBO);
		//�V�F�[�_�[���p�C�v���C���ɃZ�b�g����
		pipline->setPipline();
		kDevice::drawIndexed(6);
	}

	void kPlane::render(kGraphicsPipline* pipline,rlib::r2DObj* tex,f32 angle,f32 sx,f32 sy,const Vector3& pos,const Vector3& norm,f32 offsetx,f32 offsety,f32 width,f32 height)
	{
		//�p���s��̍쐬
		Matrix rotz;
		rotz.identity();
		rotz.setRZ(angle);
		Matrix mat=RenderLib::RenderState::getViewMatrix();
		MatrixFrontVec(&mat,pos,norm,Vector3(0,1,0));
		mat.mulS(sx,sy,0.0f);
		mat=rotz*mat;
		//���f���s��쐬(�e�X�g�p)
		math::Matrix wvp = 
			rotz*
			mat * 
			RenderLib::RenderState::getViewMatrix() *
			RenderLib::RenderState::getProjectionMatrix();
		pipline->setTexture("colorTex",0,tex);
		pipline->setShaderValue("WVP",wvp);
		//���_�o�b�t�@���p�C�v���C���ɃZ�b�g����
		kDevice::IAsetVertexBuffer(m_VBO);
		//�C���f�b�N�X�o�b�t�@���p�C�v���C���ɃZ�b�g����
		kDevice::IAsetIndexBuffer(m_IBO);
		//�V�F�[�_�[���p�C�v���C���ɃZ�b�g����
		pipline->setPipline();
		kDevice::drawIndexed(6);
	}
}