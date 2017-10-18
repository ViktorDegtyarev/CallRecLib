/*Copyright 2017 Viktor Degtyarev

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/

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
