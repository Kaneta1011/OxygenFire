/*
 * Copyright (C) 2007 The Android Open Source Project
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

package com.android.OxygenFire;

import android.app.Activity;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.nio.CharBuffer;


public class MainActivity extends Activity {

    JNIView mView;

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
       
        mView = new JNIView(getApplication());
        setContentView(mView);
        testFile();
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
    
    
    void testFile()
    {
    	//assetからファイル名を取り出す
    	AssetManager assetManager = getAssets();
    	
        String[] assetLists = null;
        try {
        	assetLists = assetManager.list("");//ファイル階層最上層の中身をチェック
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        //とりあえず、最初に見つけたtxtファイルを読み込む
        String filePath = "";
        for( int i=0; i<assetLists.length; i++ )
        {
        	int p = assetLists[i].lastIndexOf('.');
        	if( p == -1 ) continue;
        	
        	String ext = assetLists[i].substring(p+1);
        	if( ext.equals("txt"))
        	{
        		filePath = assetLists[i];
        		break;
        	}
        }
        JNILib.fileTest(assetManager, filePath);
    }
    
    byte[] readAssetsFile(AssetManager assets, String filePath)
    {
        try {
			AssetFileDescriptor fd = assets.openFd(filePath);
			fd.getLength();
			FileInputStream stream = fd.createInputStream();
			byte[] buf = new byte[(int) fd.getLength()];
			stream.read(buf);
			return buf;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        return null;
    }
    
}
