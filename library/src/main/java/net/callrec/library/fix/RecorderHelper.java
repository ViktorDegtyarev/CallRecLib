package net.callrec.library.fix;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.support.v4.content.ContextCompat;

public class RecorderHelper {
    private static RecorderHelper ourInstance = new RecorderHelper();

    public static RecorderHelper getInstance() {
        return ourInstance;
    }

    private RecorderHelper() {
    }

    public boolean startFixCallRecorder(Context context, int audioSessionId) {
        if (isPermissionAudioSettingsModify(context)) {
            CallRecorderFixHelper.getInstance().initialize();

            CallRecorderFixHelper.getInstance().startFix(audioSessionId);

            return true;
        }

        return false;
    }

    public void stopFixCallRecorder() {
        CallRecorderFixHelper.getInstance().stopFix();
    }

    private boolean isPermissionAudioSettingsModify(Context context) {
        int permissionCheck = ContextCompat.checkSelfPermission(context,
                Manifest.permission.MODIFY_AUDIO_SETTINGS);

        boolean result = false;
        switch (permissionCheck) {
            case PackageManager.PERMISSION_GRANTED:
                result = true;
                break;

            case PackageManager.PERMISSION_DENIED:
                result = false;
                break;
        }

        return result;
    }
}
