package com.safejni.test;

import com.jniutils.test.R;

import android.app.Activity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.os.Build;

public class TestActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_test);
		
		System.loadLibrary("safejni");
		System.loadLibrary("safejnitest");
		runTests();
	}
	
	//called by native
	public static String concat(String a, String b)
	{
		return a + b;
	}
	
	//called by native
	public static String[] toUpper(String[] array)
	{
		String[] result = new String[array.length];
		for (int i = 0; i < array.length; ++i) {
			result[i] = array[i].toUpperCase();
		}
		return result;
	}
	
	//called by native
	public static int sum(byte[] array)
	{
		int result = 0;
		for (byte b: array) {
			result+=b;
		}
		return result;
	}
	
	//called by native
	public static byte[] add(byte[] array, int value)
	{
		byte[] result = array.clone();
		for (int i = 0; i < result.length; ++i) {
			result[i]+= value;
		}
		return result;
	}
	
	
	private native void runTests();

}
