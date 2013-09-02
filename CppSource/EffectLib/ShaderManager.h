#ifndef __teShaderManager_H__
#define __teShaderManager_H__
#include	"Shader.h"


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
	static Shader* getSprite(){return m_spSprite;}
private:
	static void Create_Shader(Shader** pShader,char* VS,char* FS);
	//	Data
	static Shader* m_spSimple;
	static Shader* m_spSprite;
};

}//ShaderLib

#endif