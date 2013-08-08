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

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.widget.TextView.BufferType;

public class Debug_Oxygenfire_Activity extends Activity {
	GL2JNIView graphic = null;
	DebugMessageView debugMsg = null;
	
	StringBuffer msg = new StringBuffer(1024);
	
	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		graphic = new GL2JNIView(this);
		graphic.setRenderer(new SceneRender(this));
		
		debugMsg = new DebugMessageView(this);
		
		FrameLayout ll_frame = new FrameLayout(this);
		ll_frame.addView(graphic);
		ll_frame.addView(debugMsg);
		setContentView(ll_frame);
		
		JNICallMethod.assets = getAssets();
		GL2JNILib.systemInit(getAssets(), TouchEventManager.getMaxPoint());
		GL2JNILib.debugInit(this);
	}

	protected void addMsg(String msg)
	{
		//Log.i("debug",this.msg);
		this.msg.insert(this.msg.length(), msg);
		this.msg.insert(this.msg.length(), "\n");
	}
	
	protected void flush()
	{
		debugMsg.msg.delete(0, debugMsg.msg.length());
		debugMsg.msg.append(this.msg);
		this.msg.delete(0, this.msg.length());
		//Log.i("",debugMsg.msg.toString());
	}
	
	@Override
	protected void onPause() {
		super.onPause();
		graphic.onPause();
		GL2JNILib.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		graphic.onResume();
		GL2JNILib.onResume();
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
