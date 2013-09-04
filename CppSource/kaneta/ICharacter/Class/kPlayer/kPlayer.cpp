#include "kPlayer.h"
#include "kaneta\ICharacter\Class\kPlayer\Strategy\Move\kPlayerMove.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
namespace klib
{
	kPlayer::kPlayer(const char* filename):ICharacter(filename){setStrategy(new kPlayerMove);}
	kPlayer::kPlayer(const char* filename,const rlib::AnalogStick* stick):ICharacter(filename,stick){setStrategy(new kPlayerMove);}
	kPlayer::~kPlayer()
	{
		//delete m_Camera;
	}

	bool kPlayer::update()
	{
		ICharacter::exeStrategy();
		mp_Mesh->Update();
		mp_Mesh->animation(1.0f);
		//m_Camera->update();
	}
	bool kPlayer::render(kGraphicsPipline* pipline)
	{
		mp_Mesh->Render(pipline);
	}
}