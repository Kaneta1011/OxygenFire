#pragma once
#include "defines.h"
#include "../kShader/kShader.h"
#include "../kInputLayout/kInputLayout.h"
#include "math\kmathf.h"
#include	"RenderLib\\RenderState.h"

namespace klib
{


	class kGraphicsPipline
	{
		friend class kDevice;
	private:
		GLuint	m_Program;
		kInputLayout* mp_InputLayout;
		kShader* mp_VertexShader;
		kShader* mp_PixelShader;
	public:
		kGraphicsPipline();
		~kGraphicsPipline();
		bool createVertexShader(const char* filename);
		bool createPixelShader(const char* filename);
		bool complete(const kInputElementDesc* desc,u32 descsize);
		bool setPipline();
		void Send_Matrix(const math::Matrix& mWorld)
		{
			//	WMP
			math::Matrix wvp = 
				mWorld * 
				RenderLib::RenderState::getViewMatrix() *
				RenderLib::RenderState::getProjectionMatrix();

		  SetValue("WVP", wvp );
		}

		void SetValue(const char* Name, const math::Matrix& Mat)
		{
			glUseProgram(m_Program);
			GLint work = glGetUniformLocation( m_Program, Name );
		  glUniformMatrix4fv( work, 1, GL_FALSE, &Mat.m[0][0] );
		glUseProgram(0);
		}
	};
}