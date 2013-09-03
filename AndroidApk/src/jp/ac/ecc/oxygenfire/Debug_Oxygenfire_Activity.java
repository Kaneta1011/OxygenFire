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

package jp.ac.ecc.oxygenfire;

import java.io.File;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.text.Editable;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.TextView.BufferType;
import android.os.*;

public class Debug_Oxygenfire_Activity extends Activity {
	GL2JNIView graphic = null;
	DebugMessageView debugMsg = null;
	
	StringBuffer msg = new StringBuffer(1024);
	Handler mHandler = new Handler();;
	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		//Viewの設定
		graphic = new GL2JNIView(this);
		graphic.setRenderer(new SceneRender(this));
		
		debugMsg = new DebugMessageView(this);
		
		FrameLayout ll_frame = new FrameLayout(this);
		ll_frame.addView(graphic);
		ll_frame.addView(debugMsg);
		setContentView(ll_frame);
		
		//C++側の初期化など
		JNICallMethod.assets = getAssets();
		GL2JNILib.systemInit(getAssets(), TouchEventManager.getMaxPoint());
		GL2JNILib.debugInit(this);
		GL2JNILib.initFolderPath(this.getFilesDir().getAbsolutePath(), Environment.getExternalStorageDirectory().getAbsolutePath());

	}

	private void writeLogSystemInfo()
	{
		Log.d("build","BOARD:" + Build.BOARD);
		Log.d("build","BOOTLOADER:" + Build.BOOTLOADER);    //Android 1.6未対応
		Log.d("build","BRAND:" + Build.BRAND);
		Log.d("build","CPU_ABI:" + Build.CPU_ABI);
		Log.d("build","CPU_ABI2:" + Build.CPU_ABI2);        //Android 1.6未対応
		Log.d("build","DEVICE:" + Build.DEVICE);
		Log.d("build","DISPLAY:" + Build.DISPLAY);
		Log.d("build","FINGERPRINT:" + Build.FINGERPRINT);
		Log.d("build","HARDWARE:" + Build.HARDWARE);        //Android 1.6未対応
		Log.d("build","HOST:" + Build.HOST);
		Log.d("build","ID:" + Build.ID);
		Log.d("build","MANUFACTURER:" + Build.MANUFACTURER);
		Log.d("build","MODEL:" + Build.MODEL);
		Log.d("build","PRODUCT:" + Build.PRODUCT);
		Log.d("build","RADIO:" + Build.RADIO);              //Android 1.6未対応
		Log.d("build","TAGS:" + Build.TAGS);
		Log.d("build","TIME:" + Build.TIME);
		Log.d("build","TYPE:" + Build.TYPE);
		Log.d("build","UNKNOWN:" + Build.UNKNOWN);          //Android 1.6未対応
		Log.d("build","USER:" + Build.USER);
		Log.d("build","VERSION.CODENAME:" + Build.VERSION.CODENAME);
		Log.d("build","VERSION.INCREMENTAL:" + Build.VERSION.INCREMENTAL);
		Log.d("build","VERSION.RELEASE:" + Build.VERSION.RELEASE);
		Log.d("build","VERSION.SDK:" + Build.VERSION.SDK);
		Log.d("build","VERSION.SDK_INT:" + Build.VERSION.SDK_INT);	
	}
	
	protected void addMsg(String msg)
	{
		this.msg.insert(this.msg.length(), msg);
		this.msg.insert(this.msg.length(), "\n");
	}
	
	protected void flush()
	{
		debugMsg.msg.delete(0, debugMsg.msg.length());
		debugMsg.msg.append(this.msg);
		msg.delete(0, msg.length());
		//UIのマルチスレッド対応版
		this.mHandler.post(new Runnable(){
			@Override
			public void run() {
				debugMsg.setText(debugMsg.msg);
			}
		});
	}
	
	@Override
	protected void onPause() {
		super.onPause();
		graphic.queueEvent(new Runnable(){
			@Override
			public void run() {
				// TODO Auto-generated method stub
				GL2JNILib.onPause();
			}
		});
	}

	@Override
	protected void onResume() {
		super.onResume();
		GL2JNILib.onResume();
		msg.delete(0, msg.length());
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		GL2JNILib.onDestory();
		GL2JNILib.debugDelete();
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		TouchEventManager.onTouch(event);
		return super.onTouchEvent(event);
	}
}
