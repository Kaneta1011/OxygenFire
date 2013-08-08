#pragma once
namespace klib
{
	namespace ktl
	{
		///@brief �s���S�^���󂯕t���邽�߂̃C���^�t�F�[�X
		/// �|�C���^�� void* �ň����Ă��镔�����~�\_
		struct ImplHolderBase
		{
			virtual ~ImplHolderBase() {}
			virtual ImplHolderBase* Clone() const = 0;
			virtual void CopyTo( ImplHolderBase* pHolder ) const = 0;
			virtual void* GetPtr() = 0;
			virtual const void* GetPtr() const= 0;
		};
		///@brief �|�C���^��ێ�����N���X
		template <class T>
		class ImplHolder : public ImplHolderBase
		{
		private:
			T* m_ptr; ///>@brief ���ꂪ Impl �̎���
		public:
			///@brief �R���X�g���N�^
			explicit ImplHolder( T* ptr ): m_ptr( ptr ) {}
			///@brief �f�X�g���N�^Impl�����
			~ImplHolder()
			{
				delete m_ptr;
			}
			///@brief ������ T �̃R�s�[�R���X�g���N�^���Ă΂��
			ImplHolderBase* Clone() const
			{
				return new ImplHolder( new T( *m_ptr ));
			}
			///@brief  ImplHolder �ȊO�͍l�����Ȃ��̂ŁAstatic_cast �ŕϊ��\
			/// ������ T �̑�����Z�q���Ă΂��
			void CopyTo( ImplHolderBase* pHolder ) const
			{
				*( static_cast<ImplHolder*>( pHolder )->m_ptr ) = *m_ptr; 
			}
			///@brief �|�C���^�擾
			void* GetPtr()
			{
				return m_ptr;
			}
			///@brief const�ȃ|�C���^�̎擾
			const void* GetPtr() const
			{
				return m_ptr;
			}
		private: 
			///@brief �R�s�[���֎~���邽�߂ɃR�s�[�R���X�g���N�^���������Ȃ�
			ImplHolder( const ImplHolder& );
			///@brief ������֎~���邽�߂ɑ�����Z�q���������Ȃ�
			ImplHolder& operator = ( const ImplHolder& );
		};
		///@brief Pimpl �N���X�̎���
		template <class T>
		class pImpl
		{
		private:
			ImplHolderBase* m_pHolder; ///<@brief  T �Ɉˑ����Ȃ�
		public:
			pImpl():m_pHolder(NULL){}
			///@brief �R���X�g���N�^���̎��_�� T �̌^�����܂�
			explicit pImpl( T* ptr ) : m_pHolder( new ImplHolder<T>( ptr )) {}
			///@brief ������Z�q���̎��_�� T �̌^�����܂�
			pImpl& operator=( T* rhs )
			{
				m_pHolder=new ImplHolder<T>( rhs );
				return *this;
			}
			///@bried �f�X�g���N�^Impl���������
			~pImpl()
			{
				delete m_pHolder; // T ���s���S�^�ł����Ȃ�
			}
			///@brief �R�s�[�R���X�g���N�^ Clone �őΉ�
			pImpl( const pImpl& init ) : m_pHolder( init.m_pHolder->Clone()) {}
			///@brief ������Z�q CopyTo �őΉ�
			pImpl& operator=( const pImpl& rhs )
			{
				rhs.m_pHolder->CopyTo( m_pHolder ); // ����� CopyTo �őΉ�
				return *this;
			}
			///@brief �֐߉��Z�q��Impl�̎Q�Ǝ擾
			T& operator*()
			{
				return *( static_cast<T*>( m_pHolder->GetPtr()));
			}
			///@brief �֐߉��Z�q��const��Impl�̎Q�Ƃ��擾
			const T& operator*() const
			{
				return *( static_cast<const T*>( m_pHolder->GetPtr()));
			}
			///@brief �A���[���Z�q��Impl�̃|�C���^���擾
			T* operator -> ()
			{
				return &( **this );
			}
			///@brief �A���[���Z�q��const��Impl�̃|�C���^���擾
			const T* operator->() const
			{
				return &( **this );
			}
		};
	}
}