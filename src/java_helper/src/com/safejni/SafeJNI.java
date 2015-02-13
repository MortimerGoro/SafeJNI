package com.safejni;

import android.app.Activity;
import android.content.Intent;

import java.util.ArrayList;

/*
 * Helper class that provides a single entry point to notify android events and provide configuration to C++ plugins
 * You can configure the current activity, a dispatch thread and notify life cycle events.
*/
public enum SafeJNI
{
    INSTANCE;

    private Activity _acivity;
    private JavaToNativeDispatcher _dispatcher;
    private ArrayList<ActivityLifeCycleListener> _listeners = new ArrayList<ActivityLifeCycleListener>();

    SafeJNI() {
        System.loadLibrary("safejni");
    }

    public void setActivity(Activity activity) {
        _acivity = activity;
    }

    public Activity getActivity() {
        return _acivity;
    }

    public void setJavaToNativeDispatcher(JavaToNativeDispatcher dispatcher) {
        _dispatcher = dispatcher;
    }

    public void dispatchToNative(Runnable runnable) {

        if (_dispatcher != null) {
            _dispatcher.dispatch(runnable);
        }
        else {
            runnable.run();
        }
    }

    public void addLifeCycleListener(ActivityLifeCycleListener listener) {
        if (!_listeners.contains(listener)) {
            _listeners.add(listener);
        }
    }

    public void removeLifeCycleListener(ActivityLifeCycleListener listener) {
        _listeners.remove(listener);
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data ) {
        for (ActivityLifeCycleListener listener: _listeners) {
            listener.onActivityResult(requestCode, resultCode, data);
        }
    }

    public void onPause() {
        for (ActivityLifeCycleListener listener: _listeners) {
            listener.onPause();
        }
    }

    public void onResume() {
        for (ActivityLifeCycleListener listener: _listeners) {
            listener.onResume();
        }
    }

    public void onDestroy() {
        for (ActivityLifeCycleListener listener: _listeners) {
            listener.onDestroy();
        }
    }

    public interface JavaToNativeDispatcher
    {
        void dispatch(Runnable runnable);
    }

    public interface ActivityLifeCycleListener {
        void onActivityResult(int requestCode, int resultCode, Intent data );
        void onPause();
        void onResume();
        void onDestroy();
    }
}
