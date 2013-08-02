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

//	テキスト読み込みでおうやったか忘れたからこれで
const char vs[] = 
  "attribute vec4 VPosition;								\n"
	"attribute vec4 VColor;										\n"
	"varying vec4 io_Color;										\n"
	"																					\n"
	"uniform mat4 WVP;												\n"
	"																					\n"
  "void main() {														\n"
	"	io_Color = VColor;											\n"
	"	vec4 opos = WVP * VPosition;						\n"
	"	opos.z=2.0*opos.z-opos.w;								\n"
  " gl_Position = opos;											\n"
  "}																			  \n";
const char fs[] = 
  "precision mediump float;									\n"
	"varying vec4 io_Color;										\n"
  "void main() {                            \n"
  "  gl_FragColor = io_Color;								\n"
  "}                                        \n";

	m_spSimple->Init(vs,fs );
}

void ShaderManager::Delete()
{
	delete m_spSimple; m_spSimple = NULL;
}

