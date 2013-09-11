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
		///@brief �V�[���|�C���^
		IScene*		mp_Scene;
		///@brief �V�[���X�^�b�N
		workStack	m_SceneStack;
		void checkDuplicate(IScene* scene)
		{
			s32 size=m_SceneStack.size();
			//�X�^�b�N���̃V�[���̏I������
			for(int i=0;i<size;i++)
			{
				DYNAMIC_ASSERT(scene!=m_SceneStack[i],"�V�[���ɃX�^�b�N����Ă���|�C���^�ƃV�[�����d�����Ă��܂�");
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
		* @brief �V�[�������ׂăN���A����
		*/
		void sceneClear()
		{
			s32 size=m_SceneStack.size();
			//�X�^�b�N���̃V�[���̏I������
			for(int i=size-1;i>=0;i--)
			{
				m_SceneStack[i]->exit();
			}
			m_SceneStack.clear();
			//���݂̃V�[���̏I������
			if(mp_Scene)mp_Scene->exit();
			mp_Scene=NULL;
		}
		/**
		* @brief �V�[�����v�b�V������
		* @param[in] scene �v�b�V������V�[��
		*/
		void scenePush(IScene* scene)
		{
			if(mp_Scene)m_SceneStack.push_back(mp_Scene);
			checkDuplicate(scene);
			mp_Scene=scene;
			//�V�[���̊J�n����
			mp_Scene->entry();
		}
		/**
		* @brief �V�[�����|�b�v����
		* �|�b�v����ۂ̓|�C���^�A�N�Z�X�ᔽ�ɒ��ӂ��邱��
		*/
		bool scenePop()
		{
			if(m_SceneStack.empty())return false;
			//�V�[���̏I������
			mp_Scene->exit();
			mp_Scene=m_SceneStack.back();
			m_SceneStack.pop_back();
			return true;
		}
		/**
		* @brief �V�[����ύX����
		* �V�[�������蓖�ĂăX�^�b�N�ɋl�܂��Ă���V�[�������ׂďI��������
		* @param[in] �ύX����V�[��
		*/
		void sceneChange(IScene*scene)
		{
			sceneClear();
			mp_Scene=scene;
			//�V�[���̊J�n����
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