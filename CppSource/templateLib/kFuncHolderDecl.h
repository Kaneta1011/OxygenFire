/**
* @file 簡易関数ホルダークラス群
*/
#pragma once
#include "../defines.h"
#include "pImpl.h"

/**
* @namespace かねたライブラリ
*/
namespace klib
{
	/**
	* @namespace かねたテンプレートライブラリ
	*/
	namespace ktl
	{	
		/*
		* @class 関数ホルダー抽象クラス
		*/
		template<typename R>
		class IkFuncHolder
		{
		private:
		public:
			virtual ~IkFuncHolder() {}
			virtual R operator()()=0;
		};

		/*
		* @class 関数ホルダークラス
		* @brief 引数なし~第9引数関数まで対応
		*/
		template<typename R>
		class kFuncHolder:public	IkFuncHolder<R>
		{
		private:
			class Impl;
			pImpl<Impl> m_Impl;
		public:
			~kFuncHolder(){}
			kFuncHolder():m_Impl(new Impl){}
			template<typename F>
			kFuncHolder(F func):m_Impl(new Impl(func)){}
			template<typename F,typename A1>
			kFuncHolder(F func,A1 arg1):m_Impl(new Impl(func,arg1)){}
			template<typename F,typename A1,typename A2>
			kFuncHolder(F func,A1 arg1,A2 arg2):m_Impl(new Impl(func,arg1,arg2)){}
			template<typename F,typename A1,typename A2,typename A3>
			kFuncHolder(F func,A1 arg1,A2 arg2,A3 arg3):m_Impl(new Impl(func,arg1,arg2,arg3)){}
			template<typename F,typename A1,typename A2,typename A3,typename A4>
			kFuncHolder(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4):m_Impl(new Impl(func,arg1,arg2,arg3,arg4)){}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5>
			kFuncHolder(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5):m_Impl(new Impl(func,arg1,arg2,arg3,arg4,arg5)){}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
			kFuncHolder(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6):m_Impl(new Impl(func,arg1,arg2,arg3,arg4,arg5,arg6)){}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
			kFuncHolder(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7):m_Impl(new Impl(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7)){}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
			kFuncHolder(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8):m_Impl(new Impl(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)){}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
			kFuncHolder(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8,A9 arg9):m_Impl(new Impl(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)){}
			R operator()(){return (*m_Impl)();}
		public:
			template<typename F>
			void bindFunc(F func){m_Impl->bindFunc(func);}
			template<typename F,typename A1>
			void bindFunc(F func,A1 arg1){m_Impl->bindFunc(func,arg1);}
			template<typename F,typename A1,typename A2>
			void bindFunc(F func,A1 arg1,A2 arg2){m_Impl->bindFunc(func,arg1,arg2);}
			template<typename F,typename A1,typename A2,typename A3>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3){m_Impl->bindFunc(func,arg1,arg2,arg3);}
			template<typename F,typename A1,typename A2,typename A3,typename A4>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4,arg5);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8,A9 arg9){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);}
		};
	}
}