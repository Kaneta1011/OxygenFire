#pragma once
//----------------------------------------------------------------------
//	Singleton
//----------------------------------------------------------------------
namespace klib
{
	namespace ktl
	{
#define SINGLETON_INST_REF 0
		template <class T>
		class kSingleton
		{
		private :
			///@brief �V���O���g���N���X�̃C���X�^���X
			static T* mp_Instance;
		protected:
			///@brief �쐬�A�R�s�[�֎~
			kSingleton() {}
			virtual ~kSingleton() {}
		private:
			kSingleton(const kSingleton&);
			kSingleton& operator=(const kSingleton&);
		public :
			/**
			* @brief �V���O���g���N���X�̍쐬
			*/
			static void _create()
			{ 
				//�C���X�^���X�|�C���^���^�Ȃ瑽�d�쐬
				DYNAMIC_ASSERT(!mp_Instance,"�V���O���g���N���X�̑��d�쐬");
				mp_Instance = new T();
			}
			/**
			* @brief �V���O���g���N���X�̔j��
			*/
			static void _destroy(){SAFE_DELETE(mp_Instance);}
			/**
			* @brief �V���O���g���N���X�̃C���X�^���X�̎擾
			* @�v���v���Z�b�T�ŎQ�Ƃƃ|�C���^��؂�ւ�
			* @return �C���X�^���X�̎Q�Ƃ������̓|�C���^
			*/
#ifdef SINGLETON_INST_REF
			static T& _getInstance()
			{
				if(!_exists()){_create();}
				return *mp_Instance;
			}
#else
			static T* _getInstance()
			{
				if(!_exists()){_create();}
				return mp_Instance;
			}
#endif
			/**
			* @brief �V���O���g���N���X�̃C���X�^���X�̎Q�Ǝ擾
			* @return �C���X�^���X�̎Q��
			*/
			static T& _getInstanceRef()
			{
				if(!_exists()){_create();}
				return *mp_Instance;
			}
			/**
			* @brief �V���O���g���N���X�̃C���X�^���X�̃|�C���^�擾
			* @return �C���X�^���X�̃|�C���^
			*/
			static T* _getInstancePtr()
			{
				if(!_exists()){_create();}
				return mp_Instance;
			}
			/**
			* @brief �V���O���g���N���X�̃C���X�^���X�̑��݂��m�F
			* @return �C���X�^���X�����݂��Ă����true
			*/
			static bool _exists(){return mp_Instance!=NULL;}
		};

		template <class T>
		T* kSingleton<T>::mp_Instance = NULL;
	}
}