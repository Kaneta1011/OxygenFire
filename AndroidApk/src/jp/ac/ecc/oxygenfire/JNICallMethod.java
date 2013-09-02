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
				Log.i(TAG,"�ǂݍ��݊����B path=" + fileName);
			return bm;
		}
		else
		{
			Log.e(TAG,"AssetManager���ݒ肳��Ă��܂���B");
			return null;
		}
	}
	
	/*
	 * ���[�J���t�H���_�[�̃p�X���擾����
	 */
	static public String getLoaclDirectoryPath()
	{
		return localPath;
	}
	/*
	 * SD�J�[�h�̃t�H���_�[�p�X���擾����
	 */
	static public String getSDCardDirectoryPath()
	{
		File folder = Environment.getExternalStorageDirectory();
		return folder.getAbsolutePath();
	}
}
