#pragma once
#include "defines.h"
namespace klib
{
	class kObjectBuffer
	{
		friend class kDevice;
	private:
		GLuint m_BO;
		///@brief �o�b�t�@�̃T�C�Y
		u32 m_BufferSize;
		///@brief �o�b�t�@�̎��(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@)
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