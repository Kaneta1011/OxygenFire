#include "TitleScene.h"
#include "FrameWork\Class\kFrameWork\kFrameWork.h"
#include "GraphicsLib\Class\kDevice\kDevice.h"
#include "GraphicsLib\Class\r2DObj\r2DObj.h"
#include "main.h"
#include "utility\assetsLoader.h"
#include "input\Button.h"

namespace klib
{
		void TitleScene::setStrategy(ITitleSceneStrategy* strategy)
		{
			m_NextStrategy=strategy;
		}
		//エントリー処理
		void TitleScene::entry()
		{
			eprintf("TitleScene entry");

			m_NextStrategy=NULL;
			m_Strategy.SetPtr(new TitleSceneStart,false);

			AssetsLoader::begin();
			kDevice::begin();



			m_BackGraphics=new rlib::r2DObj();
			m_BackGraphics->load("kanetaPlace/oldpaper.png");
			m_BackGraphics->setPos(0,0);
			m_BackGraphics->setSize(200,true);

			m_StartButton=new rlib::CircleButton();
			m_NewGameButton=new rlib::CircleButton();
			m_LoadGameButton=new rlib::CircleButton();
			m_NewGameButton->init("kanetaPlace/stick.png",0,0,50);
			m_LoadGameButton->init("kanetaPlace/stick.png",0,-50,50);

			kDevice::end();
			AssetsLoader::end();
		}
		//更新処理
		void TitleScene::update()
		{
			//eprintf("TitleScene update");
			if(m_NextStrategy)
			{
				m_Strategy.SetPtr(m_NextStrategy,false);
				m_NextStrategy=NULL;
			}

			m_Strategy->update(this);
		}
		void TitleScene::render()
		{
			//eprintf("TitleScene render");
			m_BackGraphics->render();
			m_Strategy->render(this);
		}
		//終了処理
		void TitleScene::exit()
		{
			eprintf("TitleScene exit");
			SAFE_DELETE(m_NextStrategy);
			SAFE_DELETE(m_BackGraphics);
			SAFE_DELETE(m_StartButton);
			SAFE_DELETE(m_NewGameButton);
			SAFE_DELETE(m_LoadGameButton);
		}
}