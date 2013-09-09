package jp.ac.ecc.oxygenfire;

import android.view.MotionEvent;

public class TouchEventManager {
	/**
	 * 最大タッチ検出数
	 */
	static private int maxPoint = 4;
	static private float[] x = new float[maxPoint];
	static private float[] y = new float[maxPoint];
	static private float[] pressures = new float[maxPoint];
	
	static void setMaxPoint(int max){maxPoint = max;}
	static int getMaxPoint(){return maxPoint;}
	
	static int prevAction = -1;
	static public void onTouch(MotionEvent event)
	{
//		if( false && prevAction != event.getAction() )
//		{
//			int action = event.getAction();
//			String msg="";
//			switch(action & MotionEvent.ACTION_MASK) {
//			case MotionEvent.ACTION_DOWN:			msg = "DOWN";		break;
//			case MotionEvent.ACTION_POINTER_DOWN:	msg = "POINTER_DOWN"; break;
//			case MotionEvent.ACTION_UP:				msg = "UP";			break;
//			case MotionEvent.ACTION_POINTER_UP:		msg = "POINTER_UP"; break;
//			case MotionEvent.ACTION_MOVE:			msg = "MOVE";		break;
//			case MotionEvent.ACTION_CANCEL:			msg = "CANCEL";		break;
//			default:								msg = "unknwon";
//			}
//			Log.i("touch info", "action=" + msg + " | id=" + event.getActionIndex() );
//			prevAction = event.getAction();
//		}
	//タッチしている全座標の取得
		int count = 0;
		for( int i=0; i<event.getPointerCount(); i++ )
		{
			int index = event.getPointerId(i);
			if( index >= maxPoint ) continue;
			x[i] = event.getX(i);
			y[i] = event.getY(i);
			pressures[i] = event.getPressure(i);
			count++;
		}
	//タッチの状態を取得
		int condition = 3;
		int action = event.getAction();
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
		case MotionEvent.ACTION_CANCEL:
			condition = 4;
			break;
		default:
			condition = 3;
		}
	//イベントが発生したポインターのIDを取得している
		int id = (action & MotionEvent.ACTION_POINTER_ID_MASK) >> MotionEvent.ACTION_POINTER_ID_SHIFT;
	//cpp側にデータを転送
		GL2JNILib.sendTouchEvent(count, x, y, pressures, id, condition);
	}
}
