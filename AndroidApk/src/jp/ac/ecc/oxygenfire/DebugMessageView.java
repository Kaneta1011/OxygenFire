package jp.ac.ecc.oxygenfire;

import android.content.Context;
import android.widget.TextView;

public class DebugMessageView extends TextView {
	public StringBuffer msg = new StringBuffer(1024);
	public DebugMessageView(Context context) {
		super(context);
		setText("debug message");
	}
}
