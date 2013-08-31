//	use .h
#include	"ShaderManager.h"
#include	"utility/assetsLoader.h"

//	use namespace
using namespace ShaderLib;

//	use static data
Shader* ShaderManager::m_spSprite = NULL;

//----------------------------------------------------------------------
//	ShaderManager
//----------------------------------------------------------------------
void ShaderManager::Init()
{
	//	Sprite
	Create_Shader(&m_spSprite,"shader/sprite.vs","shader/sprite.fs");
}

void ShaderManager::Create_Shader(RenderLib::Shader** pShader,const char* VS, const char* FS)
{
	char* vs = NULL;
	char* fs = NULL;
	int size;

	*pShader = new RenderLib::Shader;
	AssetsLoader::load(&vs,&size,VS);
	AssetsLoader::load(&fs,&size,FS);
	(*pShader)->Init(vs,fs);
}

void ShaderManager::Delete()
{
	delete m_spSprite; m_spSprite = NULL;
}

