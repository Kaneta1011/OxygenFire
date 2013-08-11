//	use .h
#include	"ShaderLib\\ShaderManager.h"
#include	"utility/assetsLoader.h"
//	use namespace
using namespace ShaderLib;

//	use static data
Shader* ShaderManager::m_spSimple = NULL;

//----------------------------------------------------------------------
//	ShaderManager
//----------------------------------------------------------------------
void ShaderManager::Init()
{
	char* vs = NULL;
	char*	fs = NULL;
	int size;

	//	Simple
	m_spSimple = new Shader;
	AssetsLoader::load(&vs,&size,"simple.vs");
	AssetsLoader::load(&fs,&size,"simple.fs");
	m_spSimple->Init(vs,fs);
}

void ShaderManager::Delete()
{
	delete m_spSimple; m_spSimple = NULL;
}

