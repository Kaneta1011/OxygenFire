#pragma once

#include "CppSource\GraphicsLib\Class\kMesh\kMesh.h"
#include "CppSource\GraphicsLib\Class\kMesh\kMeshLoadIEM.h"
#include "CppSource\GraphicsLib\Class\kMesh\kMeshGLES20Render.h"
#include "GraphicsLib\Class\kGraphicsPipline\kGraphicsPipline.h"

#include "CppSource\math\kmathf.h"
#include "CppSource\templateLib\smartptr.h"
#include "input\AnalogStick.h"
namespace klib
{
	class ICharaStrategy;

	class ICharacter
	{
	protected:
		ktl::sp<ICharaStrategy> mp_Strategy;
		const rlib::AnalogStick* m_Stick;
		kSkin* mp_Mesh;
		bool exeStrategy();
	public:
		ICharacter(const char* filename):mp_Mesh(new kSkin(filename,new kMeshLoadIEM,new kMeshGLES20Render)){}
		ICharacter(const char* filename,const rlib::AnalogStick* stick):mp_Mesh(new kSkin(filename,new kMeshLoadIEM,new kMeshGLES20Render)),m_Stick(stick){}
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

		////座標
		//void setPosition(f32 x,f32 y,f32 z){mp_Mesh->setPosition(x,y,z);}
		//void setPosition(const math::Vector3& pos){mp_Mesh->setPosition(pos.x,pos.y,pos.z);}
		//void addPosition(f32 x,f32 y,f32 z){mp_Mesh->addPoisitionX(x);mp_Mesh->addPoisitionY(x);mp_Mesh->addPoisitionZ(x);}
		//void addPosition(const math::Vector3& pos){mp_Mesh->addPoisitionX(pos.x);mp_Mesh->addPoisitionY(pos.x);mp_Mesh->addPoisitionZ(pos.x);}
		////角度
		//void setAngle(f32 val){mp_Mesh->setAngle(val);}
		//void setAngle(f32 x,f32 y,f32 z){mp_Mesh->setAngle(x,y,z);}
		//void setAngle(const math::Vector3& pos){mp_Mesh->setAngle(pos.x,pos.y,pos.z);}
		//void addAngle(f32 x,f32 y,f32 z){mp_Mesh->addAngleX(x);mp_Mesh->addAngleY(x);mp_Mesh->addAngleZ(x);}
		//void addAngle(const math::Vector3& pos){mp_Mesh->addAngleX(pos.x);mp_Mesh->addAngleY(pos.x);mp_Mesh->addAngleZ(pos.x);}
		////大きさ
		//void setScale(f32 val){mp_Mesh->setScale(val);}
		//void setScale(f32 x,f32 y,f32 z){mp_Mesh->setScale(x,y,z);}
		//void setScale(const math::Vector3& pos){mp_Mesh->setScale(pos.x,pos.y,pos.z);}

		//
		//math::Vector3 getPosition()const{return mp_Mesh->getPosition();}
		//math::Vector3 getAngle()const{return mp_Mesh->getPosition();}
		//math::Vector3 getScale()const{return mp_Mesh->getScale();}


	};

	class ICharaStrategy
	{
	protected:
	public:
		ICharaStrategy(){}
		virtual ~ICharaStrategy(){}
		virtual bool execute(ICharacter* acter)=0;
	};

	class kPlayerMove:public ICharaStrategy
	{
	private:
	public:
		virtual bool execute(ICharacter* acter)
		{
			const rlib::AnalogStick* stick=acter->getAnalogStick();
			kSkin* obj=acter->getObj();

			if(stick->enable())
			{
				obj->SetMotion(3);
				obj->addPoisitionX(stick->getX()*0.1f);
				obj->addPoisitionZ(stick->getY()*0.1f);
			}
		}
	};
	
}