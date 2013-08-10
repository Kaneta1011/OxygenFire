package jp.ac.ecc.oxygenfire;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;

public class SceneRender implements Renderer{

	SceneRender(Context context){
		super();
	}
	private long time = 0;
	@Override
	public void onDrawFrame(GL10 arg0) {
		// TODO Auto-generated method stub
		time = System.nanoTime() - time;
		float dt = (float)time / 1000.f;
		dt /= 1000.f;
		GL2JNILib.update(dt);
	}

	@Override
	public void onSurfaceChanged(GL10 arg0, int w, int h) {
		// TODO Auto-generated method stub
		JNICallMethod method = new JNICallMethod();
		GL2JNILib.init(w, h, method);
	}

	@Override
	public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
		// TODO Auto-generated method stub
		
	}

}
