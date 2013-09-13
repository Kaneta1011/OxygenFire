#pragma once


#include "math\kmathf.h"
#include "FrameWork\Class\IScene\IScene.h"
#include "templateLib\kSingleton.h"
#include "templateLib\smartptr.h"

namespace rlib
{
	class r2DObj;
	class CircleButton;
}

namespace klib
{
	class kTechnique;
}

namespace klib
{
	class TitleScene;
	class ITitleSceneStrategy
	{
	private:
	protected:
	public:
		ITitleSceneStrategy(){}
		virtual ~ITitleSceneStrategy(){}
		virtual void update(TitleScene* scene)=0;
		virtual void render(TitleScene* scene)=0;
	};

	class TitleSceneStart:public ITitleSceneStrategy
	{
	private:
	protected:
	public:
		TitleSceneStart();
		virtual ~TitleSceneStart();
		virtual void update(TitleScene* scene);
		virtual void render(TitleScene* scene);
	};

	class TitleSceneStartTouch:public ITitleSceneStrategy
	{
	private:
	protected:
		f32 m_Time;
	public:
		TitleSceneStartTouch();
		virtual ~TitleSceneStartTouch();
		virtual void update(TitleScene* scene);
		virtual void render(TitleScene* scene);
	};
#define YESNO_NULL 0
#define YESNO_YES 1
#define YESNO_NO 2
	class TitleSceneStartMenu:public ITitleSceneStrategy
	{
	private:
	protected:
		int m_Flag;
		IScene* m_NextScene;
	public:
		TitleSceneStartMenu();
		virtual ~TitleSceneStartMenu();
		virtual void update(TitleScene* scene);
		virtual void render(TitleScene* scene);
	};
	class TitleScene:public IScene,public ktl::kSingleton<TitleScene>
	{
		friend class TitleSceneStart;
		friend class TitleSceneStartTouch;
		friend class TitleSceneStartMenu;
		friend class ktl::kSingleton<TitleScene>;
	private:
		ktl::sp<ITitleSceneStrategy> m_Strategy;
		ITitleSceneStrategy* m_NextStrategy;
		kTechnique* m_FirePaperTec;
		rlib::r2DObj* m_MaskTex;
		rlib::r2DObj* m_BackGraphics;
		rlib::r2DObj* m_TitleGraphics;
		rlib::CircleButton* m_StartButton;
		rlib::CircleButton* m_NewGameButton;
		rlib::CircleButton* m_LoadGameButton;
	public:
		void setStrategy(ITitleSceneStrategy* strategy);
		//エントリー処理
		void entry();
		//更新処理
		void update();
		void render();
		//終了処理
		void exit();
	};


}