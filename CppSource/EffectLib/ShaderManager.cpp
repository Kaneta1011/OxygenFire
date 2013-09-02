//	use .h
#include	"ShaderManager.h"
#include	"utility/assetsLoader.h"
//	use namespace
using namespace ShaderLib;
using namespace RenderLib;

//	use static data
Shader* ShaderManager::m_spSimple = NULL;
Shader* ShaderManager::m_spSprite = NULL;

//----------------------------------------------------------------------
//	ShaderManager
//----------------------------------------------------------------------
void ShaderManager::Init()
{
	//	Simple
	//Create_Shader(&m_spSimple,"shader/simple.vs","shader/simple.fs");
	//	Sprite
	Create_Shader(&m_spSprite,"shader/sprite.vs","shader/sprite.fs");
}

void ShaderManager::Create_Shader(Shader** pShader,char* VS,char* FS)
{
	char* vs = NULL;
	char*	fs = NULL;
	int size;

	*pShader = new Shader;
	AssetsLoader::load(&vs,&size,VS);
	AssetsLoader::load(&fs,&size,FS);
	(*pShader)->Init(vs,fs);
}

void ShaderManager::Delete()
{
	delete m_spSimple; m_spSimple = NULL;
	delete m_spSprite; m_spSprite = NULL;
}

