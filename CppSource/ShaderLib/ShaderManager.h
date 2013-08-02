#ifndef __teShaderManager_H__
#define __teShaderManager_H__
#include	"RenderLib\\Shader\\Shader.h"


namespace ShaderLib{

using namespace RenderLib;

//	�Ȃ񂩃e���v���[�g�����ւ񂩂�
//	�X�}�|�ƃV���O���g�����킸��

//----------------------------------------------------------------------
//	ShaderManager
//----------------------------------------------------------------------
class ShaderManager
{
public:
	static void Init();
	static void Delete();
	static Shader* getSimple(){return m_spSimple;}
private:
	static Shader* m_spSimple;
};

}//ShaderLib

#endif