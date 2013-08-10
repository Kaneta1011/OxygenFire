#ifndef _DEBUG_MESSAGE_MNG_
#define _DEBUG_MESSAGE_MNG_

#include <jni.h>

#ifdef _DEBUG

#define DEBUG_MSG_INIT(env, activity) DebugMessageMng::init((env), (activity))
#define DEBUG_MSG_CLEAR(env) DebugMessageMng::clear((env))
#define DEBUG_MSG(msg, ...) \
	sprintf(DebugMessageMng::sBuffer, (msg), __VA_ARGS__ );\
	DebugMessageMng::addMsg(DebugMessageMng::sBuffer)

#define DEBUG_MSG_NON_ARAG(msg) DebugMessageMng::addMsg((msg))
#define DEBUG_FLUSH_MSG() DebugMessageMng::flush()

//=========================================================================
//
//		デバッグ用として画面にテキストを表示するクラス
//		使用するときは下のマクロを使ってください
//			DEBUG_MSG(msg)
//				・msgに表示する文字列を入れてください
//				・このマクロは必ず、表示する文字列以外の引数を１つ以上必要とします
//					引数が必要ないときは「DEBUG_MSG_NON_ARG」マクロを使ってください
//
//		使用すると表示するテキストの文字数に比例してガベレージコレクションが起きやすくなるので
//		 実行速度は多少遅くなります
//
//		テキストを表示したくないときは、Releaseでコンパイルしてください
//		 マクロによりDebugMessageMngクラスのすべての関数が無効になります。
//
//=========================================================================
class DebugMessageMng
{
public:
	static void init(JNIEnv* env, jobject activity);
	static void clear(JNIEnv* env);

	/*
	画面に表示する文字列を送る
	「可変長には対応していない」ので数値とかを表示したいときは自前で用意してください
	*/
	static void addMsg(char* msg);

	/*
	TextViewに送ったテキスト設定する
	*/
	static void flush();

public:
	static const int BUFFER_SIZE = 256;
	static char		sBuffer[BUFFER_SIZE];

private:
	static jobject			debugInst;
	static jclass			debugClass;
	static jmethodID		idAddMsg;
	static jmethodID		idFlush;
};
#else
#define DEBUG_MSG_INIT(env, activity)
#define DEBUG_MSG_CLEAR(env)
#define DEBUG_MSG(msg, ...)
#define DEBUG_MSG_NON_ARAG(msg)
#define DEBUG_FLUSH_MSG()
#endif

#endif