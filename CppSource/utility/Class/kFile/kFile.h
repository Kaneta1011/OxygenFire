#pragma once
#include "..\..\..\defines.h"

namespace klib
{
	namespace utility
	{
		/**
		* @class �t�@�C�����o�̓N���X
		*/
		class kFile
		{
		private:
			///@brief �t�@�C���T�C�Y
			u32 m_FileSize;
			///@brief �t�@�C���|�C���^
			FILE* mp_Fp;
		public:
			kFile();
			/**
			* @brief �p�X�ƃ��[�h���w�肵�ăt�@�C����ǂݍ���
			* @param[in] path �t�@�C���p�X
			* @param[in] mode �ǂݍ��݃��[�h
			*/
			kFile(const char* path,const char* mode);
			~kFile();
			/**
			* @brief �p�X�ƃ��[�h���w�肵�ăt�@�C����ǂݍ���
			* @param[in] path �t�@�C���p�X
			* @param[in] mode �ǂݍ��݃��[�h
			* @return �ǂݍ��݂ɐ��������true
			*/
			bool open(const char* path,const char* mode);
			/**
			* @brief �t�@�C�������
			*/
			void close();
			/**
			* @brief �t�@�C���|�C���^���V�[�N����
			* @param[in] offset �����ʒu����̃I�t�Z�b�g�l
			* @param[in] origin �V�[�N���鏉���ʒu�����Ă�����(fseek�Ɠ���)
			* @return ���������0
			*/
			int seek(s32 offset,s32 origin)const;
			/**
			* @brief �t�@�C������o�b�t�@�ɓǂݍ���
			* @param[out] pbuffer �ǂݍ��ރo�b�t�@
			* @param[in] buffersize �o�b�t�@�̃T�C�Y
			* @return ���ۂɓǂݍ��񂾃T�C�Y
			*/
			u32 read(char* pbuffer,u32 buffersize)const;
			/**
			* @brief �o�b�t�@����t�@�C���ɏ�������
			* @param[in] pbuffer �������ރo�b�t�@
			* @param[in] buffersize �o�b�t�@�̃T�C�Y
			* @return ���ۂɏ������񂾃T�C�Y
			*/
			u32 write(const char* pbuffer,u32 buffersize)const;
			/**
			* @brief �����t�Ńt�@�C���ɏ�������
			* @param[in] format �t�H�[�}�b�g
			* @param[in] ... �ϒ�����
			*/
			void printf(const char* format, ...)const;
			u32 getFileSize()const;
		};
	}
}