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
//		�f�o�b�O�p�Ƃ��ĉ�ʂɃe�L�X�g��\������N���X
//		�g�p����Ƃ��͉��̃}�N�����g���Ă�������
//			DEBUG_MSG(msg)
//				�Emsg�ɕ\�����镶��������Ă�������
//				�E���̃}�N���͕K���A�\�����镶����ȊO�̈������P�ȏ�K�v�Ƃ��܂�
//					�������K�v�Ȃ��Ƃ��́uDEBUG_MSG_NON_ARG�v�}�N�����g���Ă�������
//
//		�g�p����ƕ\������e�L�X�g�̕������ɔ�Ⴕ�ăK�x���[�W�R���N�V�������N���₷���Ȃ�̂�
//		 ���s���x�͑����x���Ȃ�܂�
//
//		�e�L�X�g��\���������Ȃ��Ƃ��́ARelease�ŃR���p�C�����Ă�������
//		 �}�N���ɂ��DebugMessageMng�N���X�̂��ׂĂ̊֐��������ɂȂ�܂��B
//
//=========================================================================
class DebugMessageMng
{
public:
	static void init(JNIEnv* env, jobject activity);
	static void clear(JNIEnv* env);

	/*
	��ʂɕ\�����镶����𑗂�
	�u�ϒ��ɂ͑Ή����Ă��Ȃ��v�̂Ő��l�Ƃ���\���������Ƃ��͎��O�ŗp�ӂ��Ă�������
	*/
	static void addMsg(char* msg);

	/*
	TextView�ɑ������e�L�X�g�ݒ肷��
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