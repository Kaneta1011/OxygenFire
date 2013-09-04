#pragma once

#include "math\kmathf.h"
#include "templateLib\smartptr.h"
#include "kaneta\ICharacter\Class\ICharaStrategy\ICharaStrategy.h"



//前方宣言
namespace rlib
{
	class AnalogStick;
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
		ktl::sp<ICharaStrategy> mp_Strategy;
		const rlib::AnalogStick* m_Stick;
		kSkin* mp_Mesh;
		math::Vector3 m_Move;
		bool exeStrategy();
	public:
		ICharacter(const char* filename);
		ICharacter(const char* filename,const rlib::AnalogStick* stick);
		virtual ~ICharacter(){}
		/**
		* @brief 状態処理クラスを入れる
		* NULLは入れないように
		*/
		void setStrategy(ICharaStrategy* strategy);

		virtual bool update()=0;
		virtual bool render(kGraphicsPipline* pipline)=0;

		const rlib::AnalogStick* getAnalogStick()const{return m_Stick;}
		kSkin* getObj(){return mp_Mesh;}

		math::Vector3 getMove()const{return m_Move;}
		void setMove(const math::Vector3& move){m_Move=move;}

	};
	
}