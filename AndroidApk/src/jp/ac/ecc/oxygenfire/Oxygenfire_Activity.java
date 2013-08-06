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
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.WindowManager;

import java.io.File;

public class Oxygenfire_Activity extends Activity {

	GL2JNIView graphic = null;

	private int maxPoint = 4;
	private int[] ids = new int[maxPoint];
	private float[] x = new float[maxPoint];
	private float[] y = new float[maxPoint];
	private float[] pressures = new float[maxPoint];
	
	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		graphic = new GL2JNIView(this);
		graphic.setRenderer(new SceneRender(this));
		setContentView(graphic);
		
		JNICallMethod.assets = getAssets();
		GL2JNILib.systemInit(getAssets(), maxPoint);
		
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
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		//イベントが発生したポインターのIDを取得している
		int action = event.getAction();
		int id = (action & MotionEvent.ACTION_POINTER_ID_MASK) >> MotionEvent.ACTION_POINTER_ID_SHIFT;
		int count = 0;
		for( int i=0; i<event.getPointerCount(); i++ )
		{
			int index = event.getPointerId(i);
			if( index >= maxPoint ) continue;
			ids[i] = index;
			x[i] = event.getX(i);
			y[i] = event.getY(i);
			pressures[i] = event.getPressure(i);
			count++;
		}
		
		int condition = 3;
		switch(action & MotionEvent.ACTION_MASK) {
		case MotionEvent.ACTION_DOWN:
		case MotionEvent.ACTION_POINTER_DOWN:
			condition = MotionEvent.ACTION_DOWN;
			break;
		case MotionEvent.ACTION_UP:
		case MotionEvent.ACTION_POINTER_UP:
			condition = MotionEvent.ACTION_UP;
			break;
		case MotionEvent.ACTION_MOVE:
			condition = MotionEvent.ACTION_MOVE;
			break;
		default:
			condition = 3;
		}
		
		GL2JNILib.sendTouchEvent(count, ids, x, y, pressures, id, condition);
		return super.onTouchEvent(event);
	}
}
