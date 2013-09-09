package jp.ac.ecc.oxygenfire;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.util.Log;

public class JNICallMethod {
	static final String TAG ="JNICallMethod";
	static public AssetManager assets=null;
	static public String localPath = "";
	
	static public Bitmap loadImage(String fileName)
	{
		if( assets != null )
		{
			Bitmap bm = AssetsLoader.loadBitmap(assets, fileName);
			if( bm != null )
				Log.i(TAG,"Complete load image file. path=" + fileName);
			return bm;
		}
		else
		{
			Log.e(TAG,"Don't set AssetManager!");
			return null;
		}
	}
	
}
