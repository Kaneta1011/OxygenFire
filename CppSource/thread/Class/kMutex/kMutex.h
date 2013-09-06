#pragma once
/**
* @file �X���b�h�~���[�e�b�N�X
*/
#include "../../../defines.h"
#include "../../../templateLib/pImpl.h"

namespace klib
{
	namespace thread
	{
		/**
		* @class �~���[�e�b�N�X�I�u�W�F�N�g�N���X
		* brief WIN32��Android�Ŏ����t�@�C���𕪂��Ă���̂Œ���
		*/
		class kMutex
		{
		private:
			///@brief �����N���X
			class Impl;
			ktl::pImpl<Impl> m_Impl;
		public:
			kMutex();
			~kMutex();
			///@brief �~���[�e�b�N�X�����b�N����
			void lock(int val=0);
			///@brief �~���[�e�b�N�X����������
			void unLock(int val=0);
		};

	}
}