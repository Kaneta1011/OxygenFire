package jp.ac.ecc.oxygenfire;

import android.view.MotionEvent;

public class TouchEventManager {
	static private int maxPoint = 4;
	static private int[] ids = new int[maxPoint];
	static private float[] x = new float[maxPoint];
	static private float[] y = new float[maxPoint];
	static private float[] pressures = new float[maxPoint];
	
	static void setMaxPoint(int max){maxPoint = max;}
	static int getMaxPoint(){return maxPoint;}
	
	static public void onTouch(MotionEvent event)
	{
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
	}
}
