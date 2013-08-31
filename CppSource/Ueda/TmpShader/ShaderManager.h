#ifndef __teShaderManager_H__
#define __teShaderManager_H__

#include	"GraphicsLib\Class\tRenderState\RenderState.h"
#include	"Ueda\TmpShader\Shader.h"

namespace ShaderLib{

	using namespace RenderLib;

//	なんかテンプレートつかわれへんから
//	スマポとシングルトンつかわずに

//----------------------------------------------------------------------
//	ShaderManager
//----------------------------------------------------------------------
class ShaderManager
{
public:
	static void Init();
	static void Delete();
	static Shader* getSprite(){return m_spSprite;}

	static void Create_Shader(Shader** pShader,const char* VS, const char* FS);

private:
	//	Data
	static Shader* m_spSprite;
};

}//ShaderLib

#endif