package jp.ac.ecc.oxygenfire;

import android.content.Context;
import android.graphics.Canvas;
import android.text.Editable;
import android.util.Log;
import android.widget.TextView;
import android.widget.TextView.BufferType;

public class DebugMessageView extends TextView {
	public StringBuffer msg = new StringBuffer(1024);
	public DebugMessageView(Context context) {
		super(context);
		setText("�f�o�b�O���b�Z�[�W");
	}
}
