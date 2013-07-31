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

// OpenGL ES 2.0 code

#include "Common.h"

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//=================================================================
//
//		OpenGL ESのサンプル
//			hello-gl2と同じ内容
//
//=================================================================
static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("", "GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("", "after %s() glError (0x%x)\n", op, error);
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
    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
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
                    LOGE("loadShader", "Could not compile shader %d:\n%s\n",
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

bool setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics", "setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("setupGraphics", "Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
    checkGlError("glGetAttribLocation");
    LOGI("setupGraphics", "glGetAttribLocation(\"vPosition\") = %d\n",
            gvPositionHandle);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    return true;
}

const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f };

void renderFrame() {
    static float grey;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(grey, grey, grey, 1.0f);
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
    JNIEXPORT void JNICALL Java_com_android_OxygenFire_JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_OxygenFire_JNILib_step(JNIEnv * env, jobject obj);

};


JNIEXPORT void JNICALL Java_com_android_OxygenFire_JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_android_OxygenFire_JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}


















//================================================================
//
//		ファイルテスト用コード
//
//================================================================
#include <stdio.h>
#include <assert.h>

#include "AssetsLoader.h"

#include <sys/types.h>
#include <android\asset_manager.h>
#include <android\asset_manager_jni.h>

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_OxygenFire_JNILib_fileTest(JNIEnv * env, jobject obj, jobject assetsManager, jstring path);
    JNIEXPORT jstring JNICALL Java_com_android_OxygenFire_JNILib_zlibTest(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_Test108_NativeView_RenderBitmap(JNIEnv * env, jobject  obj, jobject bitmap);

};

JNIEXPORT void JNICALL Java_com_android_OxygenFire_JNILib_fileTest(JNIEnv * env, jobject obj, jobject assetsManager, jstring path)
{
	AssetsLoader loader;
	(loader).load(env, assetsManager, path);
}

#include <zlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int CompressTest(int bCompress,const char* pszDstFile,const char* pszSrcFile)
{
	int		ret;
	int		nFd;
	Bytef*	pszMapped;
	uLongf	nFileSize;

	//処理対象ファイルを開く
	nFd = open(pszSrcFile,O_RDONLY);
	if(nFd < 0)
		return	0;

	//処理対象ファイルのサイズ取得
	{
		struct stat	sFileStat;

		ret = fstat(nFd,&sFileStat);
		if(ret < 0)
		{
			close(nFd);
			return	0;
		}
		nFileSize = sFileStat.st_size;
	}

	//処理対象ファイルをメモリマップドファイルにする
	pszMapped = (Bytef*)mmap(NULL,nFileSize,PROT_READ,MAP_SHARED,nFd,0);
	if(pszMapped == MAP_FAILED)
	{
		close(nFd);
		return	0;
	}

	//メイン処理
	{
		Bytef*	pcbCompData;
		uLongf	nCompLen;

		if(bCompress)
		{
			//圧縮処理

			nCompLen = nFileSize * 1.1 + 13;
			pcbCompData = (Bytef*)malloc(nCompLen);

			compress(pcbCompData,&nCompLen,pszMapped,nFileSize);
		}
		else
		{
			//解凍処理

			//元サイズ取得＆バッファ確保
			nCompLen = *(long*)pszMapped;
			pcbCompData = (Bytef*)malloc(nCompLen);

			Bytef*	pTmp;

			//サイズが保存されていた分だけポインタを進めて解凍処理
			pTmp = pszMapped;
			pTmp += sizeof(nFileSize);

			uncompress(pcbCompData,&nCompLen,pTmp,nFileSize - sizeof(nFileSize));
		}

		{
			FILE	*fp;
			fp = fopen(pszDstFile,"w");
			if(fp)
			{
				if(bCompress)
				{
					//圧縮時は圧縮前のサイズを保存
					fwrite(&nFileSize,sizeof(nFileSize),1,fp);
				}

				fwrite(pcbCompData,1,nCompLen,fp);
				fclose(fp);
			}
		}

		free(pcbCompData);
	}

	//メモリマップドファイル開放
	munmap(pszMapped,nFileSize);

	close(nFd);

	return	1;
}

JNIEXPORT jstring JNICALL Java_com_android_OxygenFire_JNILib_zlibTest(JNIEnv * env, jobject obj)
{
	LOGI("hoge","passage Java_com_android_OxygenFire_JNILib_zlibTest");
	//テスト用ファイル作成
	FILE	*fp;
	fp = fopen("/sdcard/test.txt","w");
	if(fp)
	{
		fputs("--------------------------------------------------\n",fp);
		fputs("----------this is compress datahoge---------------\n",fp);
		fputs("--------------------------------------------------\n",fp);
		fputs("Test line.Test line.Test line.Test line.Test line.\n",fp);
		fputs("Test line.Test line.Test line.Test line.Test line.\n",fp);
		fputs("Test line.Test line.Test line.Test line.Test line.\n",fp);
		fputs("--------------------------------------------------\n",fp);
		fclose(fp);
	}
	else
	{
		LOGE("hoge","Failure fopen");
	}
	//圧縮
	CompressTest(1,"/sdcard/test.cmp","/sdcard/test.txt");

	//解凍
	CompressTest(0,"/sdcard/decomp.txt","/sdcard/test.cmp");

	return env->NewStringUTF("Test106text");
}

#include <jni.h>
#include <android/bitmap.h>
#include <stdlib.h>
#include "png\png.h"


/* this function is from Android NDK bitmap-plasma */
static uint16_t  make565(int red, int green, int blue)
{
	return (uint16_t)( ((red   << 8) & 0xf800) |
					   ((green << 2) & 0x03e0) |
					   ((blue  >> 3) & 0x001f) );
}



static int DrawBitmap(AndroidBitmapInfo* pBitmapInfo, void* pPixels, const char* pszImageFile)
{
	FILE	*fp;

	//画像ファイルの読み込み
	fp = fopen(pszImageFile,"r");
	if(fp == NULL)
		return	0;

	int		bSucceeded = 0;

	png_bytepp	ppRowImage = NULL;
	png_structp	pPng = NULL;
	png_infop	pInfo = NULL;

	while(1)
	{
		pPng = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
		if(pPng == NULL)
			break;

		pInfo = png_create_info_struct(pPng);
		if(pInfo == NULL)
			break;

		//ファイルポインタの設定
		png_init_io(pPng,fp);

		png_uint_32	nWidth;
		png_uint_32	nHeight;
		int		nBitDepth;
		int		nColorType;
		int		nIntMethod;
		int		nCompMethod;
		int		nFilterMethod;
		png_uint_32	nImageBytes;
		png_uint_32	nRowBytes;

		//画像情報取得
		png_read_info(pPng,pInfo);
		png_get_IHDR(pPng,pInfo,&nWidth,&nHeight,&nBitDepth,&nColorType,&nIntMethod,&nCompMethod,&nFilterMethod);
		nRowBytes	= png_get_rowbytes(pPng,pInfo);
		nImageBytes	= nHeight * nRowBytes;

		//画像データ用バッファーの確保
		ppRowImage = (png_bytepp)malloc(sizeof(png_bytep) * nHeight + nImageBytes);
		if(ppRowImage == NULL)
			break;

		int		yy;

		//画像データ用バッファーの初期化
		{
			png_bytep	pRowImage;

			pRowImage = (png_bytep)&(ppRowImage[nHeight]);
			for(yy = 0; yy < nHeight; yy++)
			{
				ppRowImage[yy] = pRowImage;
				pRowImage += nRowBytes;
			}
		}


		//画像ファイル読み込み
		png_read_image(pPng,ppRowImage);


		//描画
		for(yy = 0; yy < pBitmapInfo->height && yy < nHeight; yy++)
		{
			int		xx;
			uint16_t*	pLine;
			char*		pImagePixel;

			pLine = (uint16_t*)pPixels;					//ビットマップの行データ
			pImagePixel = (char*)(ppRowImage[yy]);		//画像ファイルの行データ

			for(xx = 0; xx < pBitmapInfo->width && xx < nWidth; xx++)
			{
				//■■注意■■1ピクセル3バイト前提で処理している。しかも画像ファイルが1ピクセル3バイトかどうかはチェックしていない！
				pLine[xx] = make565(pImagePixel[0],pImagePixel[1],pImagePixel[2]);
				pImagePixel += 3;
			}

			pPixels = (char*)pPixels + pBitmapInfo->stride;
		}

		bSucceeded = 1;
		break;				//必須忘れないこと！
	}


	if(ppRowImage)
	{
		free(ppRowImage);
		ppRowImage = NULL;
	}

	if(pPng)
	{
		if(pInfo)
			png_destroy_read_struct(&pPng,&pInfo,NULL);
		else
			png_destroy_read_struct(&pPng,NULL,NULL);

		pPng = NULL;
		pInfo = NULL;
	}

	fclose(fp);

	return	bSucceeded;
}

JNIEXPORT void JNICALL Java_com_Test108_NativeView_RenderBitmap(JNIEnv * env, jobject  obj, jobject bitmap)
{
	AndroidBitmapInfo  info;
	void*			  pixels;
	int				ret;

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0)
		return;

	if (info.format != ANDROID_BITMAP_FORMAT_RGB_565)
		return;

	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0)
		return;

	DrawBitmap(&info,pixels,"/sdcard/pngnow.png");

	AndroidBitmap_unlockPixels(env, bitmap);

}
