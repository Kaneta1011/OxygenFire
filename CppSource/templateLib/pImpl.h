#pragma once
namespace klib
{
	namespace ktl
	{
		///@brief 不完全型を受け付けるためのインタフェース
		/// ポインタを void* で扱っている部分がミソ_
		struct ImplHolderBase
		{
			virtual ~ImplHolderBase() {}
			virtual ImplHolderBase* Clone() const = 0;
			virtual void CopyTo( ImplHolderBase* pHolder ) const = 0;
			virtual void* GetPtr() = 0;
			virtual const void* GetPtr() const= 0;
		};
		///@brief ポインタを保持するクラス
		template <class T>
		class ImplHolder : public ImplHolderBase
		{
		private:
			T* m_ptr; ///>@brief これが Impl の実体
		public:
			///@brief コンストラクタ
			explicit ImplHolder( T* ptr ): m_ptr( ptr ) {}
			///@brief デストラクタImplを解放
			~ImplHolder()
			{
				delete m_ptr;
			}
			///@brief ここで T のコピーコンストラクタが呼ばれる
			ImplHolderBase* Clone() const
			{
				return new ImplHolder( new T( *m_ptr ));
			}
			///@brief  ImplHolder 以外は考えられないので、static_cast で変換可能
			/// ここで T の代入演算子が呼ばれる
			void CopyTo( ImplHolderBase* pHolder ) const
			{
				*( static_cast<ImplHolder*>( pHolder )->m_ptr ) = *m_ptr; 
			}
			///@brief ポインタ取得
			void* GetPtr()
			{
				return m_ptr;
			}
			///@brief constなポインタの取得
			const void* GetPtr() const
			{
				return m_ptr;
			}
		private: 
			///@brief コピーを禁止するためにコピーコンストラクタを実装しない
			ImplHolder( const ImplHolder& );
			///@brief 代入を禁止するために代入演算子を実装しない
			ImplHolder& operator = ( const ImplHolder& );
		};
		///@brief Pimpl クラスの実体
		template <class T>
		class pImpl
		{
		private:
			ImplHolderBase* m_pHolder; ///<@brief  T に依存しない
		public:
			pImpl():m_pHolder(NULL){}
			///@brief コンストラクタこの時点で T の型が決まる
			explicit pImpl( T* ptr ) : m_pHolder( new ImplHolder<T>( ptr )) {}
			///@brief 代入演算子この時点で T の型が決まる
			pImpl& operator=( T* rhs )
			{
				m_pHolder=new ImplHolder<T>( rhs );
				return *this;
			}
			///@bried デストラクタImplを解放する
			~pImpl()
			{
				delete m_pHolder; // T が不完全型でも問題ない
			}
			///@brief コピーコンストラクタ Clone で対応
			pImpl( const pImpl& init ) : m_pHolder( init.m_pHolder->Clone()) {}
			///@brief 代入演算子 CopyTo で対応
			pImpl& operator=( const pImpl& rhs )
			{
				rhs.m_pHolder->CopyTo( m_pHolder ); // 代入は CopyTo で対応
				return *this;
			}
			///@brief 関節演算子でImplの参照取得
			T& operator*()
			{
				return *( static_cast<T*>( m_pHolder->GetPtr()));
			}
			///@brief 関節演算子でconstなImplの参照を取得
			const T& operator*() const
			{
				return *( static_cast<const T*>( m_pHolder->GetPtr()));
			}
			///@brief アロー演算子でImplのポインタを取得
			T* operator -> ()
			{
				return &( **this );
			}
			///@brief アロー演算子でconstなImplのポインタを取得
			const T* operator->() const
			{
				return &( **this );
			}
		};
	}
}