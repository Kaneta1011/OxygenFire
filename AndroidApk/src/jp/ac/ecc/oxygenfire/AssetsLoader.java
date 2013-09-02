package jp.ac.ecc.oxygenfire;

import java.io.IOException;
import java.io.InputStream;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;

public class AssetsLoader {
	static final String TAG = "AssetsLoader";
	static public Bitmap loadBitmap(AssetManager assets, String fileName)
	{
        try {
	        InputStream is = assets.open(fileName);
	        Bitmap bm = BitmapFactory.decodeStream(is);
	        return bm;
		} catch (IOException e) {
			//e.printStackTrace();
		}
        Log.e(TAG, "�w�肳�ꂽ�t�@�C���͌�����܂���Bpath = " + fileName);
        return null;
	}
	static String[] getFileName(AssetManager assets, String folderPath)
	{
		try{
			String[] assetLists = assets.list(folderPath);
			return assetLists;
		}catch(IOException e){
			e.printStackTrace();
		}
		Log.e(TAG, "�w�肳�ꂽ�t�H���_�[�͌�����܂���Bpath = " + folderPath);
		return null;
	}
}
