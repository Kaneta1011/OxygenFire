#pragma once
#include "..\..\..\defines.h"
#include "..\..\..\templateLib\kVector.h"

namespace klib
{
	namespace utility
	{
		class kBufferAnalyze
		{
		private:
			u32 m_BufferSize;
			char* mp_Bufferp;
			char* mp_Nowp;
			ktl::kVector<u32> m_Stackp;
		private:
			/**
			* @brief �����Ŏw�肵���T�C�Y���o�b�t�@����͂ݏo�Ȃ����`�F�b�N
			* @param[in] pbuffer �|�C���^
			* @param[in] buffersize �o�b�t�@�T�C�Y
			* @return �͂ݏo����true
			*/
			bool isOver(const char* pbuffer,s32 buffersize)const;
		public:
			kBufferAnalyze();
			kBufferAnalyze(char* pbuffer,u32 buffersize);
			~kBufferAnalyze();

			/**
			* @brief ���݈ʒu��������o�b�t�@�������T�C�Y��v���Ă��邩����
			* @param[in] pbuffer �o�b�t�@�|�C���^
			* @param[in] buffersize �o�b�t�@�T�C�Y
			* @return ��v���Ă�����true
			*/
			bool isEql(const char* pbuffer,u32 buffersize)const;
			/**
			* @brief �|�C���^���w�肷��
			* @param[in] pbuffer �o�b�t�@
			* @param[in] buffersize �o�b�t�@�T�C�Y
			*/
			void setBufferPointer(char* pbuffer,u32 buffersize);
			/**
			* @brief ���݂̐擪�|�C���^����̑��Έʒu���擾����
			* @return ���Έʒu
			*/
			u32 getPos()const;
			/**
			* @brief �擪�|�C���^����̑��Έʒu�Ń|�C���^���w�肷��
			* @param[in] pos ���Έʒu
			* @return �w��ɐ��������true
			*/
			bool setPos(u32 pos);
			/**
			* @brief �|�C���^���V�[�N����
			* @param[in] offset �����ʒu����̃I�t�Z�b�g�l
			* @param[in] origin �V�[�N���鏉���ʒu�����Ă�����(fseek�Ɠ���)
			* @return �V�[�N�ɐ��������true
			*/
			bool seek(s32 offset,s32 origin);
			/**
			* @brief �w�肵���o�b�t�@�Ɏw�肵���T�C�Y���ǂݍ��݃|�C���^��i�߂�(�ǂݍ��݂ł��Ȃ���΃|�C���^��i�߂���false��Ԃ�)
			* @param[out] pbuffer �o�b�t�@
			* @param[in] buffersize �o�b�t�@�T�C�Y
			* @return �ǂݍ��݂ɐ��������true
			*/
			bool read(void* pbuffer,u32 buffersize);
			/**
			* @brief �w�肵���o�b�t�@�Ɏw�肵���T�C�Y���������݃|�C���^��i�߂�
			* @param[in] pbuffer �o�b�t�@
			* @param[in] buffersize �o�b�t�@�T�C�Y
			* @return �������݂ɐ��������true
			*/
			bool write(const void* pbuffer,u32 buffersize);
			u32 getBufferSize()const;
			/**
			* @brief ���݂̃|�C���^�ʒu���X�^�b�N�Ƀv�b�V������
			*/
			void push();
			/**
			* @brief �X�^�b�N����|�C���^�ʒu���|�b�v����
			*/
			void pop();
		};
	}
}