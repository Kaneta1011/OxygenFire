package jp.ac.ecc.oxygenfire;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.view.MotionEvent;
import android.widget.FrameLayout;

public class Debug_Oxygenfire_Activity extends Activity
{
	GL2JNIView graphic = null;
	DebugMessageView debugMsg = null;
	
	StringBuffer msg = new StringBuffer(1024);
	Handler mHandler = new Handler();
	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		//Viewの設定
		graphic = new GL2JNIView(this);
		graphic.setRenderer(new SceneRender(this));
		
		debugMsg = new DebugMessageView(this);
		debugMsg.setTextColor(Color.YELLOW);
		FrameLayout ll_frame = new FrameLayout(this);
		ll_frame.addView(graphic);
		ll_frame.addView(debugMsg);
		setContentView(ll_frame);
		
		//C++側の設定など
		JNICallMethod.assets = getAssets();
		GL2JNILib.systemInit(getAssets(), TouchEventManager.getMaxPoint());
		GL2JNILib.debugInit(this);
		GL2JNILib.initFolderPath(this.getFilesDir().getAbsolutePath(), Environment.getExternalStorageDirectory().getAbsolutePath());
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
		this.mHandler.post(new Runnable() {
			@Override
			public void run(){
				debugMsg.setText(debugMsg.msg);
			}
		});
	}
	
	@Override 
	protected void onPause(){
		super.onPause();
		graphic.queueEvent(new Runnable() {
			@Override
			public void run() {
				GL2JNILib.onPause();
			}
		});
	}
	
	@Override
	protected void onResume(){
		super.onResume();
		GL2JNILib.onResume();
		msg.delete(0, msg.length());
	}
	
	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		GL2JNILib.onDestory();
		GL2JNILib.debugDelete();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event){
		TouchEventManager.onTouch(event);
		return super.onTouchEvent(event);
	}
}
