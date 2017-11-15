package net.callrec.library.recorder;

public class AudioFormatNative {
    public static final int AUDIO_SOURCE_DEFAULT = 0;
    public static final int AUDIO_SOURCE_MIC = 1;
    public static final int AUDIO_SOURCE_VOICE_UPLINK = 2;
    public static final int AUDIO_SOURCE_VOICE_DOWNLINK = 3;
    public static final int AUDIO_SOURCE_VOICE_CALL = 4;
    public static final int AUDIO_SOURCE_CAMCORDER = 5;
    public static final int AUDIO_SOURCE_VOICE_RECOGNITION = 6;
    public static final int AUDIO_SOURCE_VOICE_COMMUNICATION = 7;
    // public static final int AUDIO_SOURCE_CNT;
    // public static final int AUDIO_SOURCE_MAX = AUDIO_SOURCE_CNT - 1;

    public static final int AUDIO_FORMAT_INVALID = 0xFFFFFFFF;
    public static final int AUDIO_FORMAT_DEFAULT = 0;
    public static final int AUDIO_FORMAT_PCM = 0x00000000; /* DO NOT CHANGE */
    public static final int AUDIO_FORMAT_MP3 = 0x01000000;
    public static final int AUDIO_FORMAT_AMR_NB = 0x02000000;
    public static final int AUDIO_FORMAT_AMR_WB = 0x03000000;
    public static final int AUDIO_FORMAT_AAC = 0x04000000;
    public static final int AUDIO_FORMAT_HE_AAC_V1 = 0x05000000;
    public static final int AUDIO_FORMAT_HE_AAC_V2 = 0x06000000;
    public static final int AUDIO_FORMAT_VORBIS = 0x07000000;
    public static final int AUDIO_FORMAT_MAIN_MASK = 0xFF000000;
    public static final int AUDIO_FORMAT_SUB_MASK = 0x00FFFFFF;

    public static final int AUDIO_FORMAT_PCM_16_BIT = (AUDIO_FORMAT_PCM | 0x1);
    public static final int AUDIO_FORMAT_PCM_8_BIT = (AUDIO_FORMAT_PCM | 0x2);
    public static final int AUDIO_FORMAT_PCM_32_BIT = (AUDIO_FORMAT_PCM | 0x3);
    public static final int AUDIO_FORMAT_PCM_8_24_BIT = (AUDIO_FORMAT_PCM | 0x4);

    public enum transfer_type {
        TRANSFER_DEFAULT, // not specified explicitly; determine from other
        // parameters
        TRANSFER_CALLBACK, // callback EVENT_MORE_DATA
        TRANSFER_OBTAIN, // FIXME deprecated: call obtainBuffer() and
        // releaseBuffer()
        TRANSFER_SYNC,
    }

    ;

    public static final int AUDIO_INPUT_FLAG_NONE = 0x0; // no attributes
    public static final int AUDIO_INPUT_FLAG_FAST = 0x1;

    public static final int SYNC_EVENT_SAME = -1;
    public static final int SYNC_EVENT_NONE = 0;

    public static final int AUDIO_CHANNEL_OUT_FRONT_LEFT = 0x1;
    public static final int AUDIO_CHANNEL_OUT_FRONT_RIGHT = 0x2;
    public static final int AUDIO_CHANNEL_OUT_FRONT_CENTER = 0x4;
    public static final int AUDIO_CHANNEL_OUT_LOW_FREQUENCY = 0x8;
    public static final int AUDIO_CHANNEL_OUT_BACK_LEFT = 0x10;
    public static final int AUDIO_CHANNEL_OUT_BACK_RIGHT = 0x20;
    public static final int AUDIO_CHANNEL_OUT_FRONT_LEFT_OF_CENTER = 0x40;
    public static final int AUDIO_CHANNEL_OUT_FRONT_RIGHT_OF_CENTER = 0x80;
    public static final int AUDIO_CHANNEL_OUT_BACK_CENTER = 0x100;
    public static final int AUDIO_CHANNEL_OUT_SIDE_LEFT = 0x200;
    public static final int AUDIO_CHANNEL_OUT_SIDE_RIGHT = 0x400;
    public static final int AUDIO_CHANNEL_OUT_TOP_CENTER = 0x800;
    public static final int AUDIO_CHANNEL_OUT_TOP_FRONT_LEFT = 0x1000;
    public static final int AUDIO_CHANNEL_OUT_TOP_FRONT_CENTER = 0x2000;
    public static final int AUDIO_CHANNEL_OUT_TOP_FRONT_RIGHT = 0x4000;
    public static final int AUDIO_CHANNEL_OUT_TOP_BACK_LEFT = 0x8000;
    public static final int AUDIO_CHANNEL_OUT_TOP_BACK_CENTER = 0x10000;
    public static final int AUDIO_CHANNEL_OUT_TOP_BACK_RIGHT = 0x20000;

    public static final int AUDIO_CHANNEL_OUT_MONO = AUDIO_CHANNEL_OUT_FRONT_LEFT;
    public static final int AUDIO_CHANNEL_OUT_STEREO = (AUDIO_CHANNEL_OUT_FRONT_LEFT | AUDIO_CHANNEL_OUT_FRONT_RIGHT);
    public static final int AUDIO_CHANNEL_OUT_QUAD = (AUDIO_CHANNEL_OUT_FRONT_LEFT
            | AUDIO_CHANNEL_OUT_FRONT_RIGHT | AUDIO_CHANNEL_OUT_BACK_LEFT | AUDIO_CHANNEL_OUT_BACK_RIGHT);
    public static final int AUDIO_CHANNEL_OUT_SURROUND = (AUDIO_CHANNEL_OUT_FRONT_LEFT
            | AUDIO_CHANNEL_OUT_FRONT_RIGHT | AUDIO_CHANNEL_OUT_FRONT_CENTER | AUDIO_CHANNEL_OUT_BACK_CENTER);
    public static final int AUDIO_CHANNEL_OUT_5POINT1 = (AUDIO_CHANNEL_OUT_FRONT_LEFT
            | AUDIO_CHANNEL_OUT_FRONT_RIGHT
            | AUDIO_CHANNEL_OUT_FRONT_CENTER
            | AUDIO_CHANNEL_OUT_LOW_FREQUENCY | AUDIO_CHANNEL_OUT_BACK_LEFT | AUDIO_CHANNEL_OUT_BACK_RIGHT);
    // matches the correct AudioFormat.CHANNEL_OUT_7POINT1_SURROUND definition
    // for 7.1
    public static final int AUDIO_CHANNEL_OUT_7POINT1 = (AUDIO_CHANNEL_OUT_FRONT_LEFT
            | AUDIO_CHANNEL_OUT_FRONT_RIGHT
            | AUDIO_CHANNEL_OUT_FRONT_CENTER
            | AUDIO_CHANNEL_OUT_LOW_FREQUENCY
            | AUDIO_CHANNEL_OUT_BACK_LEFT
            | AUDIO_CHANNEL_OUT_BACK_RIGHT | AUDIO_CHANNEL_OUT_SIDE_LEFT | AUDIO_CHANNEL_OUT_SIDE_RIGHT);
    public static final int AUDIO_CHANNEL_OUT_ALL = (AUDIO_CHANNEL_OUT_FRONT_LEFT
            | AUDIO_CHANNEL_OUT_FRONT_RIGHT
            | AUDIO_CHANNEL_OUT_FRONT_CENTER
            | AUDIO_CHANNEL_OUT_LOW_FREQUENCY
            | AUDIO_CHANNEL_OUT_BACK_LEFT
            | AUDIO_CHANNEL_OUT_BACK_RIGHT
            | AUDIO_CHANNEL_OUT_FRONT_LEFT_OF_CENTER
            | AUDIO_CHANNEL_OUT_FRONT_RIGHT_OF_CENTER
            | AUDIO_CHANNEL_OUT_BACK_CENTER
            | AUDIO_CHANNEL_OUT_SIDE_LEFT
            | AUDIO_CHANNEL_OUT_SIDE_RIGHT
            | AUDIO_CHANNEL_OUT_TOP_CENTER
            | AUDIO_CHANNEL_OUT_TOP_FRONT_LEFT
            | AUDIO_CHANNEL_OUT_TOP_FRONT_CENTER
            | AUDIO_CHANNEL_OUT_TOP_FRONT_RIGHT
            | AUDIO_CHANNEL_OUT_TOP_BACK_LEFT
            | AUDIO_CHANNEL_OUT_TOP_BACK_CENTER | AUDIO_CHANNEL_OUT_TOP_BACK_RIGHT);

    /* input channels */
    public static final int AUDIO_CHANNEL_IN_LEFT = 0x4;
    public static final int AUDIO_CHANNEL_IN_RIGHT = 0x8;
    public static final int AUDIO_CHANNEL_IN_FRONT = 0x10;
    public static final int AUDIO_CHANNEL_IN_BACK = 0x20;
    public static final int AUDIO_CHANNEL_IN_LEFT_PROCESSED = 0x40;
    public static final int AUDIO_CHANNEL_IN_RIGHT_PROCESSED = 0x80;
    public static final int AUDIO_CHANNEL_IN_FRONT_PROCESSED = 0x100;
    public static final int AUDIO_CHANNEL_IN_BACK_PROCESSED = 0x200;
    public static final int AUDIO_CHANNEL_IN_PRESSURE = 0x400;
    public static final int AUDIO_CHANNEL_IN_X_AXIS = 0x800;
    public static final int AUDIO_CHANNEL_IN_Y_AXIS = 0x1000;
    public static final int AUDIO_CHANNEL_IN_Z_AXIS = 0x2000;
    public static final int AUDIO_CHANNEL_IN_VOICE_UPLINK = 0x4000;
    public static final int AUDIO_CHANNEL_IN_VOICE_DNLINK = 0x8000;

    public static final int AUDIO_CHANNEL_IN_MONO = AUDIO_CHANNEL_IN_FRONT;
    public static final int AUDIO_CHANNEL_IN_STEREO = (AUDIO_CHANNEL_IN_LEFT | AUDIO_CHANNEL_IN_RIGHT);
    public static final int AUDIO_CHANNEL_IN_FRONT_BACK = (AUDIO_CHANNEL_IN_FRONT | AUDIO_CHANNEL_IN_BACK);
    public static final int AUDIO_CHANNEL_IN_ALL = (AUDIO_CHANNEL_IN_LEFT
            | AUDIO_CHANNEL_IN_RIGHT | AUDIO_CHANNEL_IN_FRONT
            | AUDIO_CHANNEL_IN_BACK | AUDIO_CHANNEL_IN_LEFT_PROCESSED
            | AUDIO_CHANNEL_IN_RIGHT_PROCESSED
            | AUDIO_CHANNEL_IN_FRONT_PROCESSED
            | AUDIO_CHANNEL_IN_BACK_PROCESSED | AUDIO_CHANNEL_IN_PRESSURE
            | AUDIO_CHANNEL_IN_X_AXIS | AUDIO_CHANNEL_IN_Y_AXIS
            | AUDIO_CHANNEL_IN_Z_AXIS | AUDIO_CHANNEL_IN_VOICE_UPLINK | AUDIO_CHANNEL_IN_VOICE_DNLINK);
}
