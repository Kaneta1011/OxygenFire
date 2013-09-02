package jp.ac.ecc.oxygenfire;

import java.io.File;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.os.Environment;
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
				Log.i(TAG,"“Ç‚İ‚İŠ®—¹B path=" + fileName);
			return bm;
		}
		else
		{
			Log.e(TAG,"AssetManager‚ªİ’è‚³‚ê‚Ä‚¢‚Ü‚¹‚ñB");
			return null;
		}
	}
	
}
