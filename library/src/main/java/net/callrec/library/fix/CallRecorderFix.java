package net.callrec.library.fix;

/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
public class CallRecorderFix {
    public static native int load();
    public static native int startFix(int i2);
    public static native int stopFix();
    static {
        LibLoader.loadLib();
    }
}
