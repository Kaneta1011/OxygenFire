#include "kPlayer.h"
#include "kaneta\ICharacter\Class\kPlayer\Strategy\Move\kPlayerMove.h"
#include "GraphicsLib\Class\kMesh\kMesh.h"
#include "input\Button.h"
namespace klib
{
	kPlayer::kPlayer(const char* filename):ICharacter(filename){setStrategy(new kPlayerMove(this));}
	kPlayer::kPlayer(const char* filename,const rlib::AnalogStick* stick,const rlib::IButton* button):ICharacter(filename,stick,button){setStrategy(new kPlayerMove(this));}
	kPlayer::~kPlayer()
	{
		//delete m_Camera;
	}

	bool kPlayer::update()
	{
		mp_Mesh->setScale(0.01f);
		mp_Mesh->animation(1.0f);
		mp_Mesh->Update();
		ICharacter::exeStrategy();
		//m_Camera->update();
	}
	bool kPlayer::render(kGraphicsPipline* pipline)
	{
		mp_Mesh->Render(pipline);
	}
}