/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
//	音利用のサンプルを一番下に書いておきます。(7/31 植田)
//
#include "sound\mlsound.h"
#include "utility\assetsLoader.h"


// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include "../CppSource/math/kmathf.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

//=======================================================================================================
//
//	JNIEnvが必要になったとき引数にないとき、下のコード参考にしてください。
//	主に、スレッドが走っているときにAssetsからファイルを読み込むときに必要になると思います。
//
//	#include "utility.h"
//
//	JNIEnv* env;
//	getJNIEnv(&env);//<- getJNIEnv()はutility.hで宣言している
//
//	参考HP　：　http://blog.nekobako.net/post/36/　解決方法という見出しの下あたりにコードあり。
//
//=======================================================================================================
JavaVM* g_VM;	//getJNIEnv関数を使うときとマルチスレッドを作るとき必要になる。

//
//	ライブラリがロードされたときに自動的に呼ばれる関数
//
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI("Passage JNI_OnLoad.");
	g_VM = vm;

	return USE_JNI_VERSION;
}

//
//	ライブラリがアンロードされたときに自動的に呼ばれる関数
//
JNIEXPORT void JNI_UnLoad(JavaVM* vm, void* reserved)
{
	LOGI("Passage JNI_UnLoad.");

}

//=======================================================================================================


static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static const char gVertexShader[] = 
    "attribute vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "}\n";

static const char gFragmentShader[] = 
    "precision mediump float;\n"
    "void main() {\n"
	"  gl_FragColor = vec4(0,1,0,1);\n"
    "}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint gProgram;
GLuint gvPositionHandle;

bool setupGraphics(int w, int h, JNIEnv* env) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
            gvPositionHandle);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    return true;
}

const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f };

const GLfloat gTexCoords[] = {0.5f, 0.0f, 0.f, 1.f, 1.f, 1.f };

void renderFrame() {
    static float grey;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(1.0f, grey, grey, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");

	glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError("glDrawArrays");
}

extern "C" {
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject methods);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj);

	//グラフィック以外の処理
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_systemInit(JNIEnv * env, jobject obj, jobject asset);

	//
	//	Activityのライフサイクルにあわせてよびだされる関数
	//	Activityのライフサイクルの参考HP	http://www.javadrive.jp/android/activity/index2.html
	//
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onPause(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onResume(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onDestory(JNIEnv * env, jobject obj);

};

//
//		グラフィックの初期化
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height, jobject methods)
{
    setupGraphics(width, height, env);
}

JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_update(JNIEnv * env, jobject obj)
{
	klib::math::Matrix mat;
	mat.identity();
	mat.setRXYZ(K_PI,0,0);
	mat.inverse();
    renderFrame();
}

/*
	グラフィック以外の初期化
*/
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_systemInit(JNIEnv * env, jobject obj, jobject asset)
{
	LOGI("Passage systemInit.");
	
	AssetsLoader::sInit(env, asset);

	LOGI("Complete systemInit.");
}

//
//		Oxygenfire_Activity非表示になったとき呼び出される
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onPause(JNIEnv * env, jobject obj)
{
	LOGI("Passage onPause.");
	

	LOGI("Complete onPause.");
}

//
//		APIがPause状態から再び再開されるときに呼び出される
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onResume(JNIEnv * env, jobject obj)
{
	LOGI("Passage onResume.");
	

	LOGI("Complete onResume.");
}

//
//		APIが破棄されるときに呼び出される
//
JNIEXPORT void JNICALL Java_jp_ac_ecc_oxygenfire_GL2JNILib_onDestory(JNIEnv * env, jobject obj)
{
	LOGI("Passage onDestory.");
	

	LOGI("Complete onDestory.");
}

//---------------------------------------------------------------------------
//		音利用のサンプル
//{
//	mlSound::Base g_base;
//	g_base.init();	//初期化
//	g_base.clear();	//破棄
//
//	{//ファイルの読み込みと削除
//		g_base.add(
//			0,	//追加する番号/*0～(mlSound::Base::PLAYER_MAX-1)の間*/,
//			env,//JNIEnvクラス。JNIの引数に必ずついてるはず。
//			assetsManager,	//JavaからAssetsManagerクラスを持ってくる。
//			"sound/bgm1.mp3",	//assetsのパス。例ではassetsフォルダーのsoundフォルダーの中にあるbgm1.mp3を読み込む
//		);
//		g_base.del(0/*使用する番号*/);//削除
//	}
//	{//再生関連のコード
//		g_base.play(  /*使用する番号*/, true/*ループフラグ*/);
//		g_base.pause( /*使用する番号*/);//一時停止
//		g_base.stop(  /*使用する番号*/);//再生終了
//		g_base.volume(/*使用する番号*/, volume/*0～1の範囲*/);
//	}
//}
//---------------------------------------------------------------------------
