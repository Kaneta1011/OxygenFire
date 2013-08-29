#include "kGraphicsPipline.h"
#include "../kDevice/kDevice.h"
#include "../../../utility/Class/kBufferAnalyze/kBufferAnalyze.h"
#include "../../../utility\assetsLoader.h"
#include "../../../utility\kutility.h"
#include "../r2DObj/r2DObj.h"



namespace klib
{
	
		kTechnique::kTechnique()
		{
			//�V�F�[�_�[�v���O�������쐬
			m_Program=glCreateProgram();
			//���_�V�F�[�_�[���m��
			mp_VertexShader=new kShader;
			//�s�N�Z���V�F�[�_�[���m��
			mp_PixelShader=new kShader;

			//�u�����h�X�e�[�g���m��
			mp_BlendState=new kBlendState;
			//�[�x�X�e�[�g���m��
			mp_DepthStencilState=new kDepthStencilState;
			//���X�^���C�U�X�e�[�g���m��
			mp_RasterizerState=new kRasterizerState;
		}
		kTechnique::~kTechnique()
		{
			SAFE_DELETE(mp_VertexShader);
			SAFE_DELETE(mp_PixelShader);

			SAFE_DELETE(mp_BlendState);
			SAFE_DELETE(mp_DepthStencilState);
			SAFE_DELETE(mp_RasterizerState);
			//�V�F�[�_�[�v���O�������������
			glDeleteProgram(m_Program);
			dprintf("delete kTechnique");
		}
		bool kTechnique::createVertexShader(const char* filename)
		{
			dprintf("createVertexShade\n");

			char* buffer;
			s32 buffersize;
			//�A�Z�b�g����V�F�[�_�[�R�[�h��ǂݍ���
			AssetsLoader::load(&buffer, &buffersize, filename);
			//�R�[�h����V�F�[�_�[���R���p�C������
			if(!kDevice::createVertexShaderFromMemory(mp_VertexShader,buffer,buffersize))
			{
				eprintf("filename=%s",filename);
				eprintf("%s",buffer);
			}
			delete[] buffer;

			//	�v���O�����ɃA�^�b�`
			glAttachShader(m_Program,mp_VertexShader->m_ShaderID);
			return true;
		}
		bool kTechnique::createPixelShader(const char* filename)
		{
			dprintf("createPixelShader\n");

			char* buffer;
			s32 buffersize;
			//�A�Z�b�g����V�F�[�_�[�R�[�h��ǂݍ���
			AssetsLoader::load(&buffer, &buffersize, filename);
			//�R�[�h����V�F�[�_�[���R���p�C������
			kDevice::createPixelShaderFromMemory(mp_PixelShader,buffer,buffersize);
			delete[] buffer;

			//	�v���O�����ɃA�^�b�`
			glAttachShader(m_Program,mp_PixelShader->m_ShaderID);
			return true;
		}
		bool kTechnique::createBlendState(const eBlendType BlendStateType)
		{
			kDevice::createBlendStateAll(mp_BlendState,BlendStateType);
			return true;
		}
		bool kTechnique::createDepthStencilState(bool enable,eDepthFunc func)
		{
			kDevice::createDepthStencilState(mp_DepthStencilState,enable,func);
			return true;
		}
		bool kTechnique::createRasterizerState(eFillMode fill,eCullMode cull,bool front)
		{
			kDevice::createRasterizerState(mp_RasterizerState,fill,cull,front);
		}

		bool kTechnique::complete()
		{
			dprintf("complete\n");
			//�v���O�����������N����
			glLinkProgram(m_Program);
			return true;
		}
		bool kTechnique::setTechnique()
		{

			glUseProgram(m_Program);

			kDevice::OMsetBlendState(mp_BlendState);
			kDevice::OMsetDepthStencilState(mp_DepthStencilState);
			kDevice::RSsetState(mp_RasterizerState);

			return true;
		}
		void kTechnique::setShaderValue(const char* name,f32 val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniform1f(ID,val);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,const f32* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniform1fv(ID,len,val);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,s32 val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniform1i(ID,val);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,const s32* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniform1iv(ID,len,val);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector2& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniform2f(ID,val.x,val.y);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector2* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniform2fv(ID,len,(f32*)val);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector3& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniform3f(ID,val.x,val.y,val.z);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector3* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniform3fv(ID,len,(f32*)val);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector4& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniform4f(ID,val.x,val.y,val.z,val.w);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,const math::Vector4* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniform4fv(ID,len,(f32*)val);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,const math::Matrix& val)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniformMatrix4fv(ID,1,GL_FALSE,(f32*)&val);//�l�ݒ�
		}
		void kTechnique::setShaderValue(const char* name,const math::Matrix* val,u32 len)
		{
			GLint ID;
			glUseProgram(m_Program);
			ID = glGetUniformLocation(m_Program,name);//�V�F�[�_�ł̖��O���֘A�t����
			glUniformMatrix4fv(ID,len,GL_FALSE,(f32*)val);//�l�ݒ�
		}

		void kTechnique::setTexture(const char* name,u32 index,const rlib::r2DObj* tex)
		{
			GLenum texIndex = GL_TEXTURE0 + index;
			glActiveTexture(texIndex);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex->getTextureID());
			int loc = glGetUniformLocation(m_Program,name);
		  if( loc >= 0 )
		  {
			  glUniform1i(loc, index);
		  } else {
			  eprintf("Failure SetValue_No_BeginEnd name=%s", name);
		  }
		}

		void kTechnique::setTexture(const char* name,u32 index,const rlib::Texture* tex)
		{
			GLenum texIndex = GL_TEXTURE0 + index;
			glActiveTexture(texIndex);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, tex->getID());
			int loc = glGetUniformLocation(m_Program,name);
		  if( loc >= 0 )
		  {
			  glUniform1i(loc, index);
		  } else {
			  eprintf("Failure SetValue_No_BeginEnd name=%s", name);
		  }
		}

		kGraphicsPipline::kGraphicsPipline():kTechnique()
		{
			//���_���C�A�E�g���m��
			mp_InputLayout=new kInputLayout;
		}
		kGraphicsPipline::~kGraphicsPipline()
		{
			SAFE_DELETE(mp_InputLayout);
			dprintf("delete kGraphicsPipline");
		}
		bool kGraphicsPipline::complete(const kInputElementDesc* desc,u32 descsize)
		{
			dprintf("complete\n");
			//�v���O�����������N����
			glLinkProgram(m_Program);
			//���_���C�A�E�g���`����쐬����
			kDevice::createInputLayout(mp_InputLayout,this,desc,descsize);
			return true;
		}
		bool kGraphicsPipline::setPipline()
		{

			glUseProgram(m_Program);

			kDevice::IAsetInputLayout(mp_InputLayout);
			kDevice::OMsetBlendState(mp_BlendState);
			kDevice::OMsetDepthStencilState(mp_DepthStencilState);
			kDevice::RSsetState(mp_RasterizerState);

			return true;
		}

}