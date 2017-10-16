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
