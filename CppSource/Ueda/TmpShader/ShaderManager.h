#ifndef __teShaderManager_H__
#define __teShaderManager_H__

#include	"GraphicsLib\Class\tRenderState\RenderState.h"
#include	"Ueda\TmpShader\Shader.h"

namespace ShaderLib{

//	�Ȃ񂩃e���v���[�g�����ւ񂩂�
//	�X�}�|�ƃV���O���g�����킸��

//----------------------------------------------------------------------
//	ShaderManager
//----------------------------------------------------------------------
class ShaderManager
{
public:
	static void Create_Shader(RenderLib::Shader** pShader, const char* VS, const char* FS);

private:
};

}//ShaderLib

#endif