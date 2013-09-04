#include "ICharacter.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"
#include "input\AnalogStick.h"

namespace klib
{

	ICharacter::ICharacter(const char* filename):mp_Mesh(new kSkin(filename,new kMeshLoadIEM,new kMeshGLES20Render)){}
	ICharacter::ICharacter(const char* filename,const rlib::AnalogStick* stick):mp_Mesh(new kSkin(filename,new kMeshLoadIEM,new kMeshGLES20Render)),m_Stick(stick){}

	void ICharacter::setStrategy(ICharaStrategy* strategy)
	{
		mp_Strategy.SetPtr(strategy,false);
	}

	bool ICharacter::exeStrategy()
	{
		if(!mp_Strategy.GetPtr())return false;
		mp_Strategy->execute(this);
		return true;
	}
}