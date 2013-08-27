#pragma once

namespace klib
{
	class IScene
	{
	private:
	public:
		IScene(){}
		/**
		* @brief ����������
		* �V�[�����ύX�܂��̓v�b�V�����ꂽ���ɌĂяo�����
		*/
		virtual void entry()=0;
		/**
		* @brief �X�V����
		*/
		virtual void update()=0;
		/**
		* @brief �`�揈��
		*/
		virtual void render()=0;
		/**
		* @brief �I������
		* �V�[�����N���A�܂��̓|�b�v���ꂽ���ɌĂяo�����
		*/
		virtual void exit()=0;
	};
}