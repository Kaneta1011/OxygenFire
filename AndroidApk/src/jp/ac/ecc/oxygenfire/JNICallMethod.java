package jp.ac.ecc.oxygenfire;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.util.Log;

public class JNICallMethod {
	static final String TAG ="JNICallMethod";
	static public AssetManager assets=null;
	static public Bitmap loadImage(String fileName)
	{
		if( assets != null )
		{
			Bitmap bm = AssetsLoader.loadBitmap(assets, fileName);
			Log.i(TAG,"読み込み完了。 path=" + fileName);
			return bm;
		}
		else
		{
			Log.e(TAG,"AssetManagerが設定されていません。");
			return null;
		}
	}
}
