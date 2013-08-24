#include "kGraphicsPipline.h"
#include "../kDevice/kDevice.h"
#include "../../../utility/Class/kBufferAnalyze/kBufferAnalyze.h"
#include "../../../utility\assetsLoader.h"
#include "../../../utility\kutility.h"

namespace klib
{
		kGraphicsPipline::kGraphicsPipline()
		{
			m_Program=glCreateProgram();
			mp_InputLayout=new kInputLayout();
			mp_VertexShader=new kShader();
			mp_PixelShader=new kShader();
		}
		kGraphicsPipline::~kGraphicsPipline()
		{
			SAFE_DELETE(mp_InputLayout);
			SAFE_DELETE(mp_VertexShader);
			SAFE_DELETE(mp_PixelShader);
			glDeleteProgram(m_Program);
		}
		bool kGraphicsPipline::createVertexShader(const char* filename)
		{
			dprintf("createVertexShader\n");

			char* buffer;
			s32 buffersize;
			AssetsLoader::load(&buffer, &buffersize, filename);
			kDevice::createVertexShaderFromMemory(mp_VertexShader,buffer,buffersize);
			delete[] buffer;

			//	Attach
			glAttachShader(m_Program,mp_VertexShader->m_ShaderID);
			return true;
		}
		bool kGraphicsPipline::createPixelShader(const char* filename)
		{
			dprintf("createPixelShader\n");

			char* buffer;
			s32 buffersize;
			AssetsLoader::load(&buffer, &buffersize, filename);
			kDevice::createPixelShaderFromMemory(mp_PixelShader,buffer,buffersize);
			delete[] buffer;

			//	Attach
			glAttachShader(m_Program,mp_PixelShader->m_ShaderID);
			return true;
		}
		bool kGraphicsPipline::complete(const kInputElementDesc* desc,u32 descsize)
		{
			dprintf("complete\n");
			glLinkProgram(m_Program);
			kDevice::createInputLayout(mp_InputLayout,this,desc,descsize);
			return true;
		}
		bool kGraphicsPipline::setPipline()
		{
			dprintf("setPipline\n");
			glUseProgram(m_Program);

			kDevice::IAsetInputLayout(mp_InputLayout);

			return true;
		}
}