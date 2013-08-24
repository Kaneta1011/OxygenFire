#pragma once
#include "defines.h"
namespace klib
{
	class kObjectBuffer
	{
		friend class kDevice;
	private:
		GLuint m_BO;
		///@brief バッファのサイズ
		u32 m_BufferSize;
		///@brief バッファの種類(頂点バッファ、インデックスバッファ)
		s32 m_BufferType;
	public:
		kObjectBuffer():m_BO(0),m_BufferSize(0),m_BufferType(0){}
		~kObjectBuffer()
		{
			GLuint buffer[1]={m_BO};
			glDeleteBuffers(1,buffer);
			
		}
	};
}