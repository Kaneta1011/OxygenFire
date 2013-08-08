/**
* @file 簡易関数ホルダークラス群
*/
#pragma once
#include "kFuncHolderDecl.h"

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
		template<typename R>
		class kFuncHolder<R>::Impl
		{
			/*
			* @class 引数無し関数ホルダークラス
			*/
			template<typename F>
			class kFuncHolder0:public IkFuncHolder<R>
			{
			private:
				F m_Function;
			public:
				kFuncHolder0(F func):m_Function(func){}
				R operator()()
				{
					return m_Function();
				}
			};
			/*
			* @class 第1引数関数ホルダークラス
			*/
			template<typename F,typename A1>
			class kFuncHolder1:public IkFuncHolder<R>
			{
			private:
				F m_Function;
				A1 m_Arg1;
			public:
				kFuncHolder1(F func,A1 arg1):m_Function(func),m_Arg1(arg1){}
				R operator()()
				{
					return m_Function(m_Arg1);
				}
			};
			/*
			* @class 第2引数関数ホルダークラス
			*/
			template<typename F,typename A1,typename A2>
			class kFuncHolder2:public IkFuncHolder<R>
			{
			private:
				F m_Function;
				A1 m_Arg1;
				A2 m_Arg2;
			public:
				kFuncHolder2(F func,A1 arg1,A2 arg2):m_Function(func),m_Arg1(arg1),m_Arg2(arg2){}
				R operator()()
				{
					return m_Function(m_Arg1,m_Arg2);
				}
			};
			/*
			* @class 第3引数関数ホルダークラス
			*/
			template<typename F,typename A1,typename A2,typename A3>
			class kFuncHolder3:public IkFuncHolder<R>
			{
			private:
				F m_Function;
				A1 m_Arg1;
				A2 m_Arg2;
				A3 m_Arg3;
			public:
				kFuncHolder3(F func,A1 arg1,A2 arg2,A3 arg3):m_Function(func),m_Arg1(arg1),m_Arg2(arg2),m_Arg3(arg3){}
				R operator()()
				{
					return m_Function(m_Arg1,m_Arg2,m_Arg3);
				}
			};
			/*
			* @class 第4引数関数ホルダークラス
			*/
			template<typename F,typename A1,typename A2,typename A3,typename A4>
			class kFuncHolder4:public IkFuncHolder<R>
			{
			private:
				F m_Function;
				A1 m_Arg1;
				A2 m_Arg2;
				A3 m_Arg3;
				A4 m_Arg4;
			public:
				kFuncHolder4(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4):m_Function(func),m_Arg1(arg1),m_Arg2(arg2),m_Arg3(arg3),m_Arg4(arg4){}
				R operator()()
				{
					return m_Function(m_Arg1,m_Arg2,m_Arg3,m_Arg4);
				}
			};
			/*
			* @class 第5引数関数ホルダークラス
			*/
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5>
			class kFuncHolder5:public IkFuncHolder<R>
			{
			private:
				F m_Function;
				A1 m_Arg1;
				A2 m_Arg2;
				A3 m_Arg3;
				A4 m_Arg4;
				A5 m_Arg5;
			public:
				kFuncHolder5(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5):m_Function(func),m_Arg1(arg1),m_Arg2(arg2),m_Arg3(arg3),m_Arg4(arg4),m_Arg5(arg5){}
				R operator()()
				{
					return m_Function(m_Arg1,m_Arg2,m_Arg3,m_Arg4,m_Arg5);
				}
			};
			/*
			* @class 第6引数関数ホルダークラス
			*/
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
			class kFuncHolder6:public IkFuncHolder<R>
			{
			private:
				F m_Function;
				A1 m_Arg1;
				A2 m_Arg2;
				A3 m_Arg3;
				A4 m_Arg4;
				A5 m_Arg5;
				A6 m_Arg6;
			public:
				kFuncHolder6(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6):m_Function(func),m_Arg1(arg1),m_Arg2(arg2),m_Arg3(arg3),m_Arg4(arg4),m_Arg5(arg5),m_Arg6(arg6){}
				R operator()()
				{
					return m_Function(m_Arg1,m_Arg2,m_Arg3,m_Arg4,m_Arg5,m_Arg6);
				}
			};
			/*
			* @class 第7引数関数ホルダークラス
			*/
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
			class kFuncHolder7:public IkFuncHolder<R>
			{
			private:
				F m_Function;
				A1 m_Arg1;
				A2 m_Arg2;
				A3 m_Arg3;
				A4 m_Arg4;
				A5 m_Arg5;
				A6 m_Arg6;
				A7 m_Arg7;
			public:
				kFuncHolder7(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7):m_Function(func),m_Arg1(arg1),m_Arg2(arg2),m_Arg3(arg3),m_Arg4(arg4),m_Arg5(arg5),m_Arg6(arg6),m_Arg7(arg7){}
				R operator()()
				{
					return m_Function(m_Arg1,m_Arg2,m_Arg3,m_Arg4,m_Arg5,m_Arg6,m_Arg7);
				}
			};
			/*
			* @class 第8引数関数ホルダークラス
			*/
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
			class kFuncHolder8:public IkFuncHolder<R>
			{
			private:
				F m_Function;
				A1 m_Arg1;
				A2 m_Arg2;
				A3 m_Arg3;
				A4 m_Arg4;
				A5 m_Arg5;
				A6 m_Arg6;
				A7 m_Arg7;
				A8 m_Arg8;
			public:
				kFuncHolder8(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8):m_Function(func),m_Arg1(arg1),m_Arg2(arg2),m_Arg3(arg3),m_Arg4(arg4),m_Arg5(arg5),m_Arg6(arg6),m_Arg7(arg7),m_Arg8(arg8){}
				R operator()()
				{
					return m_Function(m_Arg1,m_Arg2,m_Arg3,m_Arg4,m_Arg5,m_Arg6,m_Arg7,m_Arg8);
				}
			};
			/*
			* @class 第9引数関数ホルダークラス
			*/
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
			class kFuncHolder9:public IkFuncHolder<R>
			{
			private:
				F m_Function;
				A1 m_Arg1;
				A2 m_Arg2;
				A3 m_Arg3;
				A4 m_Arg4;
				A5 m_Arg5;
				A6 m_Arg6;
				A7 m_Arg7;
				A8 m_Arg8;
				A9 m_Arg9;
			public:
				kFuncHolder9(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8,A9 arg9):m_Function(func),m_Arg1(arg1),m_Arg2(arg2),m_Arg3(arg3),m_Arg4(arg4),m_Arg5(arg5),m_Arg6(arg6),m_Arg7(arg7),m_Arg8(arg8),m_Arg9(arg9){}
				R operator()()
				{
					return m_Function(m_Arg1,m_Arg2,m_Arg3,m_Arg4,m_Arg5,m_Arg6,m_Arg7,m_Arg8,m_Arg9);
				}
			};
		private:
			IkFuncHolder<R>* m_Function;
		public:
			~Impl(){delete m_Function;}
			Impl():m_Function(NULL){}
			template<typename F>
			Impl(F func):m_Function(NULL){bindFunc(func);}
			template<typename F,typename A1>
			Impl(F func,A1 arg1):m_Function(NULL){bindFunc(func,arg1);}
			template<typename F,typename A1,typename A2>
			Impl(F func,A1 arg1,A2 arg2):m_Function(NULL){bindFunc(func,arg1,arg2);}
			template<typename F,typename A1,typename A2,typename A3>
			Impl(F func,A1 arg1,A2 arg2,A3 arg3):m_Function(NULL){bindFunc(func,arg1,arg2,arg3);}
			template<typename F,typename A1,typename A2,typename A3,typename A4>
			Impl(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4):m_Function(NULL){bindFunc(func,arg1,arg2,arg3,arg4);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5>
			Impl(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5):m_Function(NULL){bindFunc(func,arg1,arg2,arg3,arg4,arg5);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
			Impl(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6):m_Function(NULL){bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
			Impl(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7):m_Function(NULL){bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
			Impl(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8):m_Function(NULL){bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
			Impl(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8,A9 arg9):m_Function(NULL){bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);}
			R operator()(){return(*m_Function)();}
		public:
			template<typename F>
			void bindFunc(F func){SAFE_DELETE(m_Function);m_Function=new kFuncHolder0<F>(func);}
			template<typename F,typename A1>
			void bindFunc(F func,A1 arg1){SAFE_DELETE(m_Function);m_Function=new kFuncHolder1<F,A1>(func,arg1);}
			template<typename F,typename A1,typename A2>
			void bindFunc(F func,A1 arg1,A2 arg2){SAFE_DELETE(m_Function);m_Function=new kFuncHolder2<F,A1,A2>(func,arg1,arg2);}
			template<typename F,typename A1,typename A2,typename A3>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3){SAFE_DELETE(m_Function);m_Function=new kFuncHolder3<F,A1,A2,A3>(func,arg1,arg2,arg3);}
			template<typename F,typename A1,typename A2,typename A3,typename A4>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4){SAFE_DELETE(m_Function);m_Function=new kFuncHolder4<F,A1,A2,A3,A4>(func,arg1,arg2,arg3,arg4);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5){SAFE_DELETE(m_Function);m_Function=new kFuncHolder5<F,A1,A2,A3,A4,A5>(func,arg1,arg2,arg3,arg4,arg5);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6){SAFE_DELETE(m_Function);m_Function=new kFuncHolder6<F,A1,A2,A3,A4,A5,A6>(func,arg1,arg2,arg3,arg4,arg5,arg6);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7){SAFE_DELETE(m_Function);m_Function=new kFuncHolder7<F,A1,A2,A3,A4,A5,A6,A7>(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8){SAFE_DELETE(m_Function);m_Function=new kFuncHolder8<F,A1,A2,A3,A4,A5,A6,A7,A8>(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);}
			template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
			void bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8,A9 arg9){SAFE_DELETE(m_Function);m_Function=new kFuncHolder9<F,A1,A2,A3,A4,A5,A6,A7,A8,A9>(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);}
		};
		//template<typename R>
		//template<typename F>
		//void kFuncHolder<R>::bindFunc(F func){m_Impl->bindFunc(func);}
		//template<typename R>
		//template<typename F,typename A1>
		//void kFuncHolder<R>::bindFunc(F func,A1 arg1){m_Impl->bindFunc(func,arg1);}
		//template<typename R>
		//template<typename F,typename A1,typename A2>
		//void kFuncHolder<R>::bindFunc(F func,A1 arg1,A2 arg2){m_Impl->bindFunc(func,arg1,arg2);}
		//template<typename R>
		//template<typename F,typename A1,typename A2,typename A3>
		//void kFuncHolder<R>::bindFunc(F func,A1 arg1,A2 arg2,A3 arg3){m_Impl->bindFunc(func,arg1,arg2,arg3);}
		//template<typename R>
		//template<typename F,typename A1,typename A2,typename A3,typename A4>
		//void kFuncHolder<R>::bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4);}
		//template<typename R>
		//template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5>
		//void kFuncHolder<R>::bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4,arg5);}
		//template<typename R>
		//template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
		//void kFuncHolder<R>::bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6);}
		//template<typename R>
		//template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
		//void kFuncHolder<R>::bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7);}
		//template<typename R>
		//template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
		//void kFuncHolder<R>::bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8);}
		//template<typename R>
		//template<typename F,typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
		//void kFuncHolder<R>::bindFunc(F func,A1 arg1,A2 arg2,A3 arg3,A4 arg4,A5 arg5,A6 arg6,A7 arg7,A8 arg8,A9 arg9){m_Impl->bindFunc(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9);}
		//template<typename R>
		//R kFuncHolder<R>::operator()(){return (*m_Impl)();}

	}
}