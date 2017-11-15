package net.callrec.library.recorder;

import net.callrec.library.fix.LibLoader;

public class AudioRecordNative {
    static {
        LibLoader.loadLib();
    }

    public AudioRecordNative(int audiosource, int samplerate, int audioformat,
                             int channels, int size) {
        // TODO Auto-generated constructor stub
        //int size = getBufferSize(samplerate, audioformat, channels);
        nativeCreate(audiosource, samplerate, audioformat, channels, size);
    }

    public static int getFrameCount(int samplerate, int audioformat,
                                    int channels) {
        nativeInit();
        channels = 1;
        int result = nativeGetMinFrame(samplerate, audioformat, channels);
        return result;
    }

    public int start() {
        return nativeStart();
    }

    public int stop() {
        return nativeStop();
    }

    public byte[] read(byte[] buffer, int buffersize) {
        return nativeRead(buffer, buffersize);
    }

    public boolean destroy() {
        return nativeDestroy();
    }

    public static native boolean nativeInit();

    public static native boolean nativeDestroy();

    public static native int nativeStart();

    public static native int nativeCreate(int audiosource, int samplerate,
                                          int audioformat, int channels, int size);

    public static native int nativeStop();

    public static native byte[] nativeRead(byte[] buffer, int bufferSize);

    public static native int nativeGetBufferSize(int samplerate,
                                                 int audioformat, int channels);

    public static native int nativeGetMinFrame(int samplerate,
                                               int audioformat, int channels);
}
