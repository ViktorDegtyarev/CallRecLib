package net.callrec.library.fix;

import android.util.Log;

public class LibLoader {
    public static final String TAG = "LibLoader";
    private static boolean lLib = false;

    public static synchronized void loadLib() {
        synchronized (LibLoader.class) {
            if (!lLib) {
                System.loadLibrary("callrecfix");
                Log.d(TAG, "Loaded library callrecfix");
                lLib = true;
            }
        }
    }
}
