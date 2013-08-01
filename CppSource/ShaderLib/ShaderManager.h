#ifndef __teShaderManager_H__
#define __teShaderManager_H__
#include	"RenderLib\\Shader\\Shader.h"


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
	static Shader* getSimple(){return m_spSimple;}
private:
	static Shader* m_spSimple;
};

}//ShaderLib

#endif