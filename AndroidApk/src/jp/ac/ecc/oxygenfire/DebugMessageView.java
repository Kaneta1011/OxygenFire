package jp.ac.ecc.oxygenfire;

import android.content.Context;
import android.graphics.Canvas;
import android.text.Editable;
import android.util.Log;
import android.widget.TextView;
import android.widget.TextView.BufferType;

public class DebugMessageView extends TextView {

	public boolean isUpdate = false;
	public StringBuffer msg = new StringBuffer(1024);
	public DebugMessageView(Context context) {
		super(context);
	}
	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		if( isUpdate )
		{
			setText(msg);
			isUpdate = false;
		}
		super.onDraw(canvas);
	}
}