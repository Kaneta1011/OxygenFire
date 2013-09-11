#include "TitleScene.h"
#include "kaneta\Scene\GameScene\GameScene.h"
#include "kaneta\Scene\YesOrNoScene\YesOrNoScene.h"

namespace klib
{
	TitleSceneStart::TitleSceneStart()
	{
	}

	TitleSceneStart::~TitleSceneStart()
	{
	}

	void TitleSceneStart::update(TitleScene* scene)
	{
		if(mlInput::getNowTouchCount()>0)
		{
			dprintf("getNowTouch next TitleSceneStartTouch");
			TitleScene::_getInstance().setStrategy(new TitleSceneStartTouch);
		}
	}

	void TitleSceneStart::render(TitleScene* scene)
	{
		dprintf("Start Render");
		scene->m_BackGraphics->render();
	}

	TitleSceneStartTouch::TitleSceneStartTouch()
	{
		m_Time=0.0f;
	}

	TitleSceneStartTouch::~TitleSceneStartTouch()
	{
	}

	void TitleSceneStartTouch::update(TitleScene* scene)
	{
		dprintf("%f",m_Time);
		m_Time+=1.0f;
		scene->m_BackGraphics->setSize(200.0f-m_Time,true);
		//if(200.0f<m_Time)framework.sceneChange(testScene::_getInstancePtr());
		if(200.0f<m_Time)TitleScene::_getInstance().setStrategy(new TitleSceneStartMenu);
	}

	void TitleSceneStartTouch::render(TitleScene* scene)
	{
		scene->m_BackGraphics->render();
	}

	TitleSceneStartMenu::TitleSceneStartMenu()
	{
		m_Flag=YESNO_NULL;
		m_NextScene=NULL;
	}

	TitleSceneStartMenu::~TitleSceneStartMenu()
	{
	}

	void TitleSceneStartMenu::update(TitleScene* scene)
	{
		scene->m_BackGraphics->setSize(200.0f,true);
		scene->m_NewGameButton->update();
		if(scene->m_NewGameButton->getMode()==scene->m_NewGameButton->eDOWN)
		{
			m_NextScene=GameScene::_getInstancePtr();
			YesOrNoScene::_getInstance().setFlag(&m_Flag);
			framework.scenePush(YesOrNoScene::_getInstancePtr());
		}
		if(m_Flag==YESNO_YES)
		{
			framework.sceneChange(m_NextScene);
		}
		else if(m_Flag==YESNO_NO)
		{
			m_Flag=YESNO_NULL;
			m_NextScene=NULL;
		}
		else
		{
		}
	}

	void TitleSceneStartMenu::render(TitleScene* scene)
	{
		scene->m_BackGraphics->render();
		scene->m_NewGameButton->render();
	}
}