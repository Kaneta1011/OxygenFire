#pragma once
#include "defines.h"
namespace klib
{
	class kShader
	{
		friend class kDevice;
		friend class kGraphicsPipline;
	private:
		GLuint m_ShaderID;
	public:
		kShader()
		{
			m_ShaderID=0;
		}
		~kShader()
		{
			glDeleteShader(m_ShaderID);
		}
	};
}