
#include "kDevice.h"
#include "../kMesh/kMesh.h"
namespace klib
{
	const kInputLayout* kDevice::m_IAInputLayout;
	const kObjectBuffer* kDevice::m_IAVertexBuffer;
	const kObjectBuffer* kDevice::m_IAIndexBuffer;

	const kBlendState* kDevice::m_OMBlendState;
	const kDepthStencilState* kDevice::m_OMDepthStencilState;
	const kRasterizerState* kDevice::m_RasterizerState;

	static bool isShaderError(GLenum shaderType,GLuint shader)
	{
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			//if(infoLen==0)return false;
			char* buf = new char[infoLen];
			glGetShaderInfoLog(shader, infoLen, NULL, buf);
			eprintf("Could not compile shader %d:\n%s\n",shaderType, buf);
			delete[] buf;

			glDeleteShader(shader);
			shader = 0;
		}
		else
		{
			dprintf("Shader compile success\n");
			return false;
		}
		return true;
	}

	bool kDevice::createVertexShaderFromMemory(kShader* out,const char* buffer,s32 length)
	{
		//�V�F�[�_���쐬����
		out->m_ShaderID = glCreateShader(GL_VERTEX_SHADER);
		//�V�F�[�_�\�[�X���o�C���h����
		glShaderSource(out->m_ShaderID, 1, &buffer, &length);
		//�o�C���h���ꂽ�\�[�X���R���p�C������ID�Ƀo�C���h����
		glCompileShader(out->m_ShaderID);
		if(isShaderError(GL_VERTEX_SHADER,out->m_ShaderID))return false;
		return true;
	}

	bool kDevice::createPixelShaderFromMemory(kShader* out,const char* buffer,s32 length)
	{
		//�V�F�[�_���쐬����
		out->m_ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		//�V�F�[�_�\�[�X���o�C���h����
		glShaderSource(out->m_ShaderID, 1, &buffer, &length);
		//�o�C���h���ꂽ�\�[�X���R���p�C������ID�Ƀo�C���h����
		glCompileShader(out->m_ShaderID);
		if(isShaderError(GL_FRAGMENT_SHADER,out->m_ShaderID))return false;
		return true;

	}

	bool kDevice::createInputLayout(kInputLayout* out,const kGraphicsPipline* usePipline,const kInputElementDesc* desc,u32 descsize)
	{
		dprintf("createInputLayout\n");
		if(!out)return false;
		//���_�t�H�[�}�b�g�����`���m�ۂ���
		out->mp_Desc=new kInputLayoutDesc[descsize];
		out->m_TypeNum=descsize;
		out->m_VertexSize=0;
		for(int i=0;i<descsize;i++)
		{
			//�V�F�[�_���P�[�V�������擾����
			out->mp_Desc[i].m_Location=glGetAttribLocation(usePipline->m_Program,desc[i].SemanticName);
			//�t�H�[�}�b�g�����擾����
			out->mp_Desc[i].m_Types.dxgi_format=PixelToFormat(desc[i].Format,&out->mp_Desc[i].m_Types.component,&out->mp_Desc[i].m_Types.size,&out->mp_Desc[i].m_Types.normalize);
			dprintf("	SemanticName=%s\n",desc[i].SemanticName);
			dprintf("	AttribLocation=%u\n",out->mp_Desc[i].m_Location);
			dprintf("	SemanticSize=%u\n",out->mp_Desc[i].m_Types.size);
			dprintf("	SemanticOffset=%u\n",out->m_VertexSize);
			out->mp_Desc[i].m_Types.size/=8;
			out->m_VertexSize+=out->mp_Desc[i].m_Types.size;
		}
		//bit�P�ʂ�byte�P�ʂɕϊ�
		//out->m_VertexSize/=8;
		dprintf("InputLayoutSize=%ubyte\n",out->m_VertexSize);
		dprintf("%u",sizeof(kMeshVertex));
		return true;
	}

	bool kDevice::createVertexBuffer(kObjectBuffer*out,const void* data,u32 datasize,s32 type)
	{
		dprintf("createVertexBuffer\n");
		//���_�o�b�t�@�I�u�W�F�N�g�̍쐬
		glGenBuffers(1,&out->m_BO);
		glBindBuffer(GL_ARRAY_BUFFER,out->m_BO);
		glBufferData(GL_ARRAY_BUFFER,datasize,data,type);
		out->m_BufferSize=datasize;
		out->m_BufferType=GL_ARRAY_BUFFER;
		//�o�C���h����
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}

	bool kDevice::createIndexBuffer(kObjectBuffer* out,const void* data,u32 datasize,s32 type)
	{
		dprintf("createIndexBuffer\n");
		//�C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�̍쐬
		glGenBuffers(1, &out->m_BO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out->m_BO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize,data, type);
		out->m_BufferSize=datasize;
		out->m_BufferType=GL_ELEMENT_ARRAY_BUFFER;
		//�@�o�C���h�������̂����ǂ�
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return true;
	}
	void kDevice::createBlendState( kBlendState* out,const eBlendType* BlendStateType, u32 BlendStateTypeLength )
	{
		DYNAMIC_ASSERT_LIMIT(BlendStateTypeLength,0,8,"CreateBlendState Error");

		ZeroMemory( &out->m_Desc, sizeof( kBlendState::kBlendDesc ) );
		//GLES�ł͎g���Ă��Ȃ�
		out->m_Desc.AlphaToCoverageEnable = false;

		switch( BlendStateType[0] )
		{
			//�u�����h����
		case k_BLEND_NONE:
			out->m_Desc.RenderTarget.BlendEnable=false;
			out->m_Desc.RenderTarget.SrcBlend=GL_ONE;
			out->m_Desc.RenderTarget.DstBlend=GL_ZERO;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
			//���Z����
		case k_BLEND_ADD:
			out->m_Desc.RenderTarget.BlendEnable=true;
			out->m_Desc.RenderTarget.SrcBlend=GL_ONE;
			out->m_Desc.RenderTarget.DstBlend=GL_ONE;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
		case k_BLEND_ALPHA:
			out->m_Desc.RenderTarget.BlendEnable=true;
			out->m_Desc.RenderTarget.SrcBlend=GL_SRC_ALPHA;
			out->m_Desc.RenderTarget.DstBlend=GL_ZERO;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
		}   
	}

	void kDevice::createBlendStateAll( kBlendState* out,const eBlendType BlendStateType)
	{
		ZeroMemory( &out->m_Desc, sizeof( kBlendState::kBlendDesc ) );
		//GLES�ł͎g���Ă��Ȃ�
		out->m_Desc.AlphaToCoverageEnable = false;

		switch( BlendStateType )
		{
			//�u�����h����
		case k_BLEND_NONE:
			dprintf("k_BLEND_NONE create");
			out->m_Desc.RenderTarget.BlendEnable=false;
			out->m_Desc.RenderTarget.SrcBlend=GL_ONE;
			out->m_Desc.RenderTarget.DstBlend=GL_ZERO;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
			//���Z����
		case k_BLEND_ADD:
			dprintf("k_BLEND_ADD create");
			out->m_Desc.RenderTarget.BlendEnable=true;
			out->m_Desc.RenderTarget.SrcBlend=GL_ONE;
			out->m_Desc.RenderTarget.DstBlend=GL_ONE;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
		case k_BLEND_ALPHA:
			out->m_Desc.RenderTarget.BlendEnable=true;
			out->m_Desc.RenderTarget.SrcBlend=GL_SRC_ALPHA;
			out->m_Desc.RenderTarget.DstBlend=GL_ONE_MINUS_SRC_ALPHA;
			out->m_Desc.RenderTarget.BlendOp=GL_FUNC_ADD;
			break;
		}   
	}
	bool kDevice::createDepthStencilState(kDepthStencilState* out,bool enable,eDepthFunc func)
	{
		out->m_Desc.DepthEnable=enable;
		out->m_Desc.DepthFunc=getDepthFunc(func);
		return true;
	}
	bool kDevice::createRasterizerState(kRasterizerState* out,eFillMode fill,eCullMode cull,bool front)
	{
		out->m_Desc.FillMode=getFillMode(fill);
		out->m_Desc.CullMode=getCullMode(cull);
		//GLES�ł͎g���Ă��Ȃ�
		out->m_Desc.FrontCounterClockwise=front;
		return true;
	}


	bool kDevice::updateSubResource(const kObjectBuffer* in,const void* data,u32 datasize)
	{
		glBindBuffer( in->m_BufferType, in->m_BO );
		glBufferSubData(in->m_BufferType, 0, datasize, data);  
		glBindBuffer( in->m_BufferType, 0 );
		return true;
	}

	bool kDevice::IAsetInputLayout(const kInputLayout* in)
	{
		m_IAInputLayout=in;
		u32 offset=0;
		const kInputLayoutDesc* desc=m_IAInputLayout->mp_Desc;
		//�Z�}���e�B�N�X�̒�`�̐��������_�I�t�Z�b�g���w�肷��
		for(int i=0;i<m_IAInputLayout->m_TypeNum;i++)
		{
			glEnableVertexAttribArray(desc[i].m_Location);
			glVertexAttribPointer(desc[i].m_Location, desc[i].m_Types.component, desc[i].m_Types.dxgi_format, desc[i].m_Types.normalize, in->m_VertexSize, (GLvoid*)offset);
			//dprintf("Location %u\n",desc[i].m_Location);
			//dprintf("Component %u\n",desc[i].m_Types.component);
			//dprintf("VertexSize %u\n",in->m_VertexSize);
			//dprintf("Offset %u\n",offset);
			offset+=desc[i].m_Types.size;
		}
		return true;
	}

	bool kDevice::IAsetVertexBuffer(const kObjectBuffer* in)
	{
		m_IAVertexBuffer=in;
		glBindBuffer( GL_ARRAY_BUFFER, m_IAVertexBuffer->m_BO );
		return true;
	}

	bool kDevice::IAsetIndexBuffer(const kObjectBuffer* in)
	{
		m_IAIndexBuffer=in;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IAIndexBuffer->m_BO );
		return true;
	}

	bool kDevice::OMsetBlendState(const kBlendState* in)
	{
		m_OMBlendState=in;
		//���u�����h��L��/�����ɂ���
		if(m_OMBlendState->m_Desc.RenderTarget.BlendEnable){glEnable(GL_BLEND);dprintf("Blend On")}
		else {glDisable(GL_BLEND);dprintf("Blend OFF")}
		//�u�����h�v�Z�����w�肷��
		glBlendEquation(m_OMBlendState->m_Desc.RenderTarget.BlendOp);
		//�u�����h�W�����w�肷��
		glBlendFunc(m_OMBlendState->m_Desc.RenderTarget.SrcBlend,m_OMBlendState->m_Desc.RenderTarget.DstBlend);
		return true;
	}
	bool kDevice::OMsetDepthStencilState(const kDepthStencilState* in)
	{
		m_OMDepthStencilState=in;
		//�[�x�e�X�g��L��/�����ɂ���
		if(m_OMDepthStencilState->m_Desc.DepthEnable)glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);
		//�[�x��r�֐����w�肷��
		glDepthFunc(m_OMDepthStencilState->m_Desc.DepthFunc);
		return true;
	}
	bool kDevice::RSsetState(const kRasterizerState* in)
	{
		m_RasterizerState=in;
		//�\���J�����O��L��/�����ɂ���
		if(m_RasterizerState->m_Desc.CullMode)glEnable(GL_CULL_FACE); 
		else glDisable(GL_CULL_FACE); 
		//�J�����O�ʂ��w�肷��
		glCullFace(m_RasterizerState->m_Desc.CullMode);
	}
	bool kDevice::drawIndexed(u32 indexnum)
	{
		//�|���S���`����������߂ăC���f�b�N�X�o�b�t�@�ŕ`��
		glDrawElements( m_RasterizerState->m_Desc.FillMode, indexnum, GL_UNSIGNED_SHORT, NULL );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	}

}