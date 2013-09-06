#pragma once

#include "math\kmathf.h"
#include "templateLib\smartptr.h"
#include "kaneta\ICharacter\Class\ICharaStrategy\ICharaStrategy.h"
#include "input\Button.h"


//前方宣言
namespace rlib
{
	class AnalogStick;
	class IButton;
}

namespace klib
{
	class kSkin;
	class kMeshGLES20Render;
	class kMeshLoadIEM;

	class kGraphicsPipline;
}


namespace klib
{
	//キャラクターの基底クラス
	class ICharacter
	{
	protected:
		ICharaStrategy* m_NextStrategy;
		ktl::sp<ICharaStrategy> mp_Strategy;
		const rlib::AnalogStick* m_Stick;
		const rlib::IButton* m_Button;
		kSkin* mp_Mesh;
		math::Vector3 m_Move;
		bool exeStrategy();
	public:
		ICharacter(const char* filename);
		ICharacter(const char* filename,const rlib::AnalogStick* stick,const rlib::IButton* button);
		virtual ~ICharacter(){}
		/**
		* @brief 状態処理クラスを入れる
		* NULLは入れないように
		*/
		void setStrategy(ICharaStrategy* strategy);

		virtual bool update()=0;
		virtual bool render(kGraphicsPipline* pipline)=0;

		const rlib::AnalogStick* getAnalogStick()const{return m_Stick;}
		const rlib::IButton* getButton()const{return m_Button;}
		kSkin* getObj(){return mp_Mesh;}

		math::Vector3 getMove()const{return m_Move;}
		void setMove(const math::Vector3& move){m_Move=move;}

	};
	
}