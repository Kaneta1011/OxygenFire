#pragma once
/**
* @file �X���b�h�C�x���g
*/
#include "../../../kdefines.h"
#include "../../../KTL/pImpl.h"

namespace klib
{
	namespace thread
	{
		/**
		* @class �C�x���g�I�u�W�F�N�g�N���X
		* @brief WIN32��Android�Ŏ����t�@�C���𕪂��Ă���̂Œ���
		*/
		class kEvent
		{
		private:
			///@brief �����N���X
			class Impl;
			ktl::pImpl<Impl> m_Impl;
		public:
			kEvent();
			~kEvent();
			///@brief �C�x���g���V�O�i����Ԃɂ���
			void set();
			///@brief �C�x���g�̃V�O�i����Ԃ�҂�
			void wait();
			///@brief �C�x���g���V�O�i����Ԃɂ���
			void reset();

		};
	}
}