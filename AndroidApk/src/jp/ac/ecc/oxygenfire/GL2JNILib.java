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

import android.content.res.AssetManager;
import android.util.Log;

// Wrapper for native library

public class GL2JNILib {

     static {
         System.loadLibrary("oxygenfire");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(int width, int height, JNICallMethod methods);
     public static native void update(float dt);
     
     /**
      * @param asset : native側に送るAssetManager
      * @Assetなどのｃ＋＋側のグラフィック以外の初期化を行う
      */
     public static native void systemInit(AssetManager asset, int input_maxPoint);
     public static native void sendTouchEvent(int count, float[] pointsX, float[] pointsY, float[] arrayPressure, int id, int con);
     
     public static native void onPause();
     public static native void onResume();
     public static native void onDestory();
     
     /*
      * デバッグ用
      */
     public static native void debugInit(Debug_Oxygenfire_Activity activity);
     public static native void debugDelete();

}
