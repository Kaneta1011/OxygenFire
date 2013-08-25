#pragma once
#include "kDevice.h"

namespace klib
{

	bool kDevice::createVertexShaderFromMemory(kShader* out,const char* buffer,s32 length)
	{
		out->m_ShaderID = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(out->m_ShaderID, 1, &buffer, &length);
		glCompileShader(out->m_ShaderID);
		return true;
	}

	bool kDevice::createPixelShaderFromMemory(kShader* out,const char* buffer,s32 length)
	{
		out->m_ShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(out->m_ShaderID, 1, &buffer, &length);
		glCompileShader(out->m_ShaderID);
		return true;
	}

	bool kDevice::createInputLayout(kInputLayout* out,const kGraphicsPipline* usePipline,const kInputElementDesc* desc,u32 descsize)
	{
		dprintf("createInputLayout\n");
		if(!out)return false;
		//頂点フォーマット情報を定義数確保する
		out->mp_Desc=new kInputLayoutDesc[descsize];
		out->m_TypeNum=descsize;
		out->m_VertexSize=0;
		for(int i=0;i<descsize;i++)
		{
			//シェーダロケーションを取得する
			out->mp_Desc[i].m_Location=glGetAttribLocation(usePipline->m_Program,desc[i].SemanticName);
			//フォーマット情報を取得する
			out->mp_Desc[i].m_Types.dxgi_format=PixelToFormat(desc[i].Format,&out->mp_Desc[i].m_Types.component,&out->mp_Desc[i].m_Types.size,&out->mp_Desc[i].m_Types.normalize);
			dprintf("SemanticName=%s\n",desc[i].SemanticName);
			dprintf("AttribLocation=%u\n",out->mp_Desc[i].m_Location);
			dprintf("SemanticSize=%u\n",out->mp_Desc[i].m_Types.size);
			dprintf("SemanticOffset=%u\n",out->m_VertexSize);
			out->m_VertexSize+=out->mp_Desc[i].m_Types.size;
		}
		//bit単位をbyte単位に変換
		out->m_VertexSize/=8;
		dprintf("InputLayoutSize=%ubyte\n",out->m_VertexSize);
		return true;
	}

	bool kDevice::createVertexBuffer(kObjectBuffer*out,const void* data,u32 datasize,s32 type)
	{
		dprintf("createVertexBuffer\n");
		//頂点バッファオブジェクトの作成
		glGenBuffers(1,&out->m_BO);
		glBindBuffer(GL_ARRAY_BUFFER,out->m_BO);
		glBufferData(GL_ARRAY_BUFFER,datasize,data,type);
		out->m_BufferSize=datasize;
		out->m_BufferType=GL_ARRAY_BUFFER;
		//バインド解除
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return true;
	}

	bool kDevice::createIndexBuffer(kObjectBuffer* out,const void* data,u32 datasize,s32 type)
	{
		dprintf("createIndexBuffer\n");
		//インデックスバッファオブジェクトの作成
		glGenBuffers(1, &out->m_BO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out->m_BO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, datasize,data, type);
		out->m_BufferSize=datasize;
		out->m_BufferType=GL_ELEMENT_ARRAY_BUFFER;
		//　バインドしたものをもどす
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return true;
	}

	bool kDevice::updateSubResource(const kObjectBuffer* in,const void* data,u32 datasize)
	{
		glBindBuffer( in->m_BufferType, in->m_BO );
		glBufferSubData(in->m_BufferType, 0, datasize, data);  
		return true;
	}

	bool kDevice::IAsetInputLayout(const kInputLayout* in)
	{
		u32 offset=0;
		const kInputLayoutDesc* desc=in->mp_Desc;
		for(int i=0;i<in->m_TypeNum;i++)
		{
			//現段階ではロケーションは0のみ
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
		glBindBuffer( GL_ARRAY_BUFFER, in->m_BO );
		return true;
	}

	bool kDevice::IAsetIndexBuffer(const kObjectBuffer* in)
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, in->m_BO );
		return true;
	}

}