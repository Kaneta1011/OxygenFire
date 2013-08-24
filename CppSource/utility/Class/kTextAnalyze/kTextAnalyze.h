#pragma once
#include "..\kBufferAnalyze\kBufferAnalyze.h"

namespace klib
{
	namespace utility
	{

		class kTextAnalyze
		{
		private:
			kBufferAnalyze* m_Buffer;
		private:
			int searchTop( void );
		public:
			kTextAnalyze();
			kTextAnalyze(char* pbuffer,u32 buffersize);
			~kTextAnalyze();
			/**
			* @brief �|�C���^���w�肷��
			* @param[in] pbuffer �o�b�t�@
			* @param[in] buffersize �o�b�t�@�T�C�Y
			*/
			void setTextPointer(char* pbuffer,u32 buffersize);
			/**
			* @brief �e�L�X�g���當�����ǂݎ��
			* @param[out] str �ǂݎ����������
			* @return �擾�ɐ��������true
			*/
			bool loadString(char* str);
			/**
			* @brief �e�L�X�g���珬����ǂݎ��
			* @return �ǂݎ��������
			*/
			float LoadFloat();
			/**
			* @brief �e�L�X�g���畄���t������ǂݎ��
			* @return �ǂݎ���������t����
			*/
			int LoadInt();
			/**
			* @brief �e�L�X�g�|�C���^�����̍s�̐擪�Ɉړ�����
			*/
			void	nextLine();
			/**
			* @brief ���݂̃|�C���^�ʒu���X�^�b�N�Ƀv�b�V������
			*/
			void push();
			/**
			* @brief �X�^�b�N����|�C���^�ʒu���|�b�v����
			*/
			void pop();
			//*****************************************************************************
			//		�����n
			//*****************************************************************************
			/**
			* @brief �e�L�X�g���當�������������
			* �����㔭���ł����當����̎��̈ʒu�Ƀ|�C���^���ړ�����
			* �����Ɏ��s����Ό������J�n�����ʒu�Ƀ|�C���^��߂�
			* @param[in] str �������镶����
			* @return �����ł����true
			*/
			bool search(const char* str);
			/**
			* @brief �e�L�X�g�̍s�����當�������������
			* �����㔭���ł����當����̎��̈ʒu�Ƀ|�C���^���ړ�����
			* �����Ɏ��s����Ό������J�n�����ʒu�Ƀ|�C���^��߂�
			* @param[in] str �������镶����
			* @return �����ł����true
			*/
			bool	searchLine( const char* str );
		};
	}
}