#pragma once
#include "../../../templateLib/kVector.h"
#include "../../../templateLib/kSingleton.h"
#include "../IScene/IScene.h"
namespace klib
{
	class kFrameWork
	{
		typedef ktl::kVector<IScene*> workStack;
	private:
		///@brief シーンポインタ
		IScene*		mp_Scene;
		///@brief シーンスタック
		workStack	m_SceneStack;
		void checkDuplicate(IScene* scene)
		{
			s32 size=m_SceneStack.size();
			//スタック内のシーンの終了処理
			for(int i=0;i<size;i++)
			{
				DYNAMIC_ASSERT(scene!=m_SceneStack[i],"シーンにスタックされているポインタとシーンが重複しています");
			}
		}
	public:
		kFrameWork()
		{
			mp_Scene=NULL;
			m_SceneStack.clear();
		}
		~kFrameWork()
		{
			sceneClear();
		}
		/**
		* @brief シーンをすべてクリアする
		*/
		void sceneClear()
		{
			s32 size=m_SceneStack.size();
			//スタック内のシーンの終了処理
			for(int i=size-1;i>=0;i--)
			{
				m_SceneStack[i]->exit();
			}
			m_SceneStack.clear();
			//現在のシーンの終了処理
			if(mp_Scene)mp_Scene->exit();
			mp_Scene=NULL;
		}
		/**
		* @brief シーンをプッシュする
		* @param[in] scene プッシュするシーン
		*/
		void scenePush(IScene* scene)
		{
			if(mp_Scene)m_SceneStack.push_back(mp_Scene);
			checkDuplicate(scene);
			mp_Scene=scene;
			//シーンの開始処理
			mp_Scene->entry();
		}
		/**
		* @brief シーンをポップする
		* ポップする際はポインタアクセス違反に注意すること
		*/
		bool scenePop()
		{
			if(m_SceneStack.empty())return false;
			//シーンの終了処理
			mp_Scene->exit();
			mp_Scene=m_SceneStack.back();
			m_SceneStack.pop_back();
			return true;
		}
		/**
		* @brief シーンを変更する
		* シーンを割り当ててスタックに詰まっているシーンをすべて終了させる
		* @param[in] 変更するシーン
		*/
		void sceneChange(IScene*scene)
		{
			sceneClear();
			mp_Scene=scene;
			//シーンの開始処理
			if(mp_Scene)mp_Scene->entry();
		}

		bool sceneUpdate()
		{
			if(!mp_Scene)return false;
			mp_Scene->update();
			return true;
		}
		bool sceneRender()
		{
			if(!mp_Scene)return false;
			mp_Scene->render();
			return true;
		}
		IScene* getPrevScene()const{return m_SceneStack.back();}
	};
}