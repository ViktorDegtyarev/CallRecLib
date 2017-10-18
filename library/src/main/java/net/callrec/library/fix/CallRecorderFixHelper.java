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

/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
public class CallRecorderFixHelper {
    static final class Singleton {
        static final CallRecorderFixHelper INSTANCE = new CallRecorderFixHelper();

        private Singleton() {
        }
    }

    public static CallRecorderFixHelper getInstance() {
        return Singleton.INSTANCE;
    }

    public void initialize() {
        CallRecorderFix.load();
    }

    public void startFix(int i) {
        CallRecorderFix.startFix(i);
        try {
            Thread.sleep(300);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void stopFix() {
        try {
            Thread.sleep(120);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        CallRecorderFix.stopFix();
    }

}
