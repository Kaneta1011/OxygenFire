//	use .h
#include	"ShaderManager.h"
#include	"utility/assetsLoader.h"

//	use namespace
using namespace ShaderLib;

void ShaderManager::Create_Shader(RenderLib::Shader** pShader,const char* VS, const char* FS)
{
	char* vs = NULL;
	char*	fs = NULL;
	int size;

	*pShader = new RenderLib::Shader;
	AssetsLoader::load(&vs,&size,VS);
	AssetsLoader::load(&fs,&size,FS);
	(*pShader)->Init(vs,fs);
}
