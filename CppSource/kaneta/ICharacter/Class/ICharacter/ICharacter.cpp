#include "ICharacter.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"
#include "input\AnalogStick.h"

namespace klib
{

	ICharacter::ICharacter(const char* filename):m_NextStrategy(NULL),mp_Mesh(new kSkin(filename,new kMeshLoadIEM,new kMeshGLES20Render)){}
	ICharacter::ICharacter(const char* filename,const rlib::AnalogStick* stick,const rlib::IButton* button):m_NextStrategy(NULL),mp_Mesh(new kSkin(filename,new kMeshLoadIEM,new kMeshGLES20Render)),m_Stick(stick),m_Button(button){}

	void ICharacter::setStrategy(ICharaStrategy* strategy)
	{
		m_NextStrategy=strategy;
		//mp_Strategy.SetPtr(strategy,false);
	}

	bool ICharacter::exeStrategy()
	{
		if(m_NextStrategy)
		{
			mp_Strategy.SetPtr(m_NextStrategy,false);
			m_NextStrategy=NULL;
		}

		if(!mp_Strategy.GetPtr())return false;

		mp_Strategy->execute(this);
		return true;
	}
}