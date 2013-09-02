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
				Log.i(TAG,"読み込み完了。 path=" + fileName);
			return bm;
		}
		else
		{
			Log.e(TAG,"AssetManagerが設定されていません。");
			return null;
		}
	}
	
	/*
	 * ローカルフォルダーのパスを取得する
	 */
	static public String getLoaclDirectoryPath()
	{
		return localPath;
	}
	/*
	 * SDカードのフォルダーパスを取得する
	 */
	static public String getSDCardDirectoryPath()
	{
		File folder = Environment.getExternalStorageDirectory();
		return folder.getAbsolutePath();
	}
}
