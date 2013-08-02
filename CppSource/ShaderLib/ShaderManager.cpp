//	use .h
#include	"ShaderLib\\ShaderManager.h"
//	use namespace
using namespace ShaderLib;

//	use static data
Shader* ShaderManager::m_spSimple = NULL;

//----------------------------------------------------------------------
//	ShaderManager
//----------------------------------------------------------------------
void ShaderManager::Init()
{
	m_spSimple = new Shader;

//	�e�L�X�g�ǂݍ��݂ł�����������Y�ꂽ���炱���
const char vs[] = 
  "attribute vec4 vPosition;								\n"
	"																					\n"
	"uniform mat4 WVP;												\n"
	"																					\n"
  "void main() {														\n"
	"	vec4 opos = WVP * vPosition;						\n"
	"	opos.z=2.0*opos.z-opos.w;								\n"
  " gl_Position = opos;								\n"
  "}																			  \n";
const char fs[] = 
  "precision mediump float;\n"
  "void main() {\n"
  "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
  "}\n";

	m_spSimple->Init(vs,fs );
}

void ShaderManager::Delete()
{
	delete m_spSimple; m_spSimple = NULL;
}

