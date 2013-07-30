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
import android.view.SurfaceView;
import android.view.WindowManager;

import java.io.File;

public class Oxygenfire_Activity extends Activity {

	GL2JNIView graphic = null;

	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		graphic = new GL2JNIView(this);
		graphic.setRenderer(new SceneRender(this));
		setContentView(graphic);
	}

	@Override
	protected void onPause() {
		super.onPause();
		graphic.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		graphic.onResume();
	}
}
