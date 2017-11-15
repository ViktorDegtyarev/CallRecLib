#include <string.h>
#include "audio.h"

bool getFunctionsLibUtils(void *p_library);

void getFunctionsLibMedia(void *p_library);

void getConstructorsAudioRecord(void *p_library);

void getFunctionsAudioSystem(void *p_library);

bool getFunctionsAudioRecord(void *p_library);

AndroidAudioRecord *audiorecord;

/**
 * Constructor of AndroidAudioRecord, the class will use some function in libmedia.so
 */
AndroidAudioRecord::AndroidAudioRecord() :
        mAudioRecord(NULL) {

}

/**
 * Destructor of AndroidAudioRecord, the class will use some function in libmedia.so
 */
AndroidAudioRecord::~AndroidAudioRecord() {
    close();
}

void AndroidAudioRecord::close() {
    if (mAudioRecord) {
        if (ar_dtor)
            ar_dtor(mAudioRecord);
        free(mAudioRecord);
        mAudioRecord = NULL;
    }
}

/**
 * Create a AudioRecord instance by it's constructor.
 *
 * @param int 			audio_source
 * @param unsigned int	samplerate
 * @param int			format
 * @param unsigned int 	channels
 * @param unsigned int 	size
 *
 * @return bool
 */
bool AndroidAudioRecord::set(int audio_source, uint32_t sampleRate, int format,
                             unsigned int channels, unsigned int size) {

    close();

    mAudioRecord = malloc(SIZE_OF_AUDIORECORD);
    *((uint32_t *) ((uintptr_t) mAudioRecord + SIZE_OF_AUDIORECORD - 4)) =
            0xbaadbaad;
    if (ar_ctor24) {
        ar_ctor24(mAudioRecord, audio_source, sampleRate, format, channels,
                  &str16, size, NULL, NULL, 0, 0, TRANSFER_DEFAULT, AUDIO_INPUT_FLAG_NONE, NULL,
                  NULL,
                  NULL);
    } else if (ar_ctor19) {
        ar_ctor19(mAudioRecord, audio_source, sampleRate, format, channels,
                  size, NULL, NULL, 0, 0, TRANSFER_DEFAULT,
                  AUDIO_INPUT_FLAG_NONE);
    } else if (ar_ctor17)
        ar_ctor17(mAudioRecord, audio_source, sampleRate, format, channels,
                  size, NULL, NULL, 0, 0);
    else if (ar_ctor16)
        ar_ctor16(mAudioRecord, audio_source, sampleRate, format, channels,
                  size, (record_flags) 0, NULL, NULL, 0, 0);
    else if (ar_ctor9)
        ar_ctor9(mAudioRecord, audio_source, sampleRate, format, channels, size,
                 0, NULL, NULL, 0, 0);
    else if (ar_ctor8)
        ar_ctor8(mAudioRecord, audio_source, sampleRate, format, channels, size,
                 0, NULL, NULL, 0);
    else
        return false;
    return true;
}


int AndroidAudioRecord::inputPrivate() {
    if (mAudioRecord && ar_input_private) {
        LOGI("start AndroidAudioRecord::inputPrivate");
//        pthread_mutex_lock(&mt_1);
        int res = ar_input_private(mAudioRecord);
//        pthread_mutex_unlock(&mt_1);
        LOGI("stop AndroidAudioRecord::inputPrivate %i", res);
        return res;
    }
    return 0;
}

/**
 * Start recording with start function in AudioRecord class.
 *
 * @return int
 */
int AndroidAudioRecord::start() {
    if (mAudioRecord) {
        if (ar_start)
            ar_start(mAudioRecord, SYNC_EVENT_NONE, 0);
        else
            ar_start_below9();
        return 1;
    }
    return 0;
}

/**
 * Stop recording with start function in AudioRecord class.
 *
 * @return int
 */
int AndroidAudioRecord::stop() {
    if (mAudioRecord && ar_stop) {
        ar_stop(mAudioRecord);
        return 1;
    }
    return 0;
}

/**
 * Read sound to buffer with size of buffer by using Read function of AudioRecord class
 *
 * @param  void* 		buffer
 * @param  int			size
 *
 * @return int
 */
int AndroidAudioRecord::read(void *buffer, int size) {
    if (mAudioRecord && ar_read) {
        return ar_read(mAudioRecord, buffer, size, true);
    }
    return 0;
}

bool getFunctionsLibUtils(void *p_library) {
    string8 = (CreateString8) dlsym(p_library, "_ZN7android7String8C2EPKc");
    string16 = (CreateString16) (dlsym(p_library, "_ZN7android8String16C1EPKc"));

    pthread_mutex_lock(&mt_1);

    string8(&str8_1, kvp_as4);
    if (!str8_1) {
        LOGI("Failed to create string8");
    } else {
        LOGI("Successfully created string8");
    }
    //todo set your app name
    string16(&str16, "net.callrec.app");
//    string16(&str16, "com.CallRecord");
    if (!str16) {
        LOGI("Failed to create string16");
    } else {
        LOGI("Successfully created string16");
    }

    pthread_mutex_unlock(&mt_1);

    return true;
}

void getFunctionsLibMedia(void *p_library) {
    getFunctionsAudioSystem(p_library);
    getConstructorsAudioRecord(p_library);
    getFunctionsAudioRecord(p_library);
}

bool getFunctionsAudioRecord(void *p_library) {
    ar_dtor = (AudioRecord_dtor) (dlsym(p_library, "_ZN7android11AudioRecordD1Ev"));

    ar_start = (AudioRecord_start) (dlsym(p_library,
                                          "_ZN7android11AudioRecord5startENS_11AudioSystem12sync_event_tE15audio_session_t"));

    if (!ar_start)
        ar_start_below9 = (AudioRecord_start_below9) (dlsym(p_library,
                                                            "_ZN7android11AudioRecord5startEv"));

    ar_input_private = (AudioRecord_input_private) dlsym(p_library,
                                                         "_ZNK7android11AudioRecord15getInputPrivateEv");

    if (ar_input_private) {
        LOGI("ar_input_private found!!!");
    }

    ar_stop = (AudioRecord_stop) (dlsym(p_library,
                                        "_ZN7android11AudioRecord4stopEv"));

    ar_read = (AudioRecord_read) (dlsym(p_library,
                                        "_ZN7android11AudioRecord4readEPvmb"));

    if (!ar_read)
        ar_read = (AudioRecord_read) (dlsym(p_library,
                                            "_ZN7android11AudioRecord4readEPvj"));

    if (!ar_read)
        ar_read = (AudioRecord_read) (dlsym(p_library,
                                            "_ZN7android11AudioRecord4readEPvjb"));

    ar_getMinFrameCount = (AudioRecord_getMinFrameCount) (dlsym(p_library,
                                                                "_ZN7android11AudioRecord16getMinFrameCountEPmj14audio_format_tj"));
    if (!ar_getMinFrameCount)
        ar_getMinFrameCount = (AudioRecord_getMinFrameCount) (dlsym(p_library,
                                                                    "_ZN7android11AudioRecord16getMinFrameCountEPij14audio_format_ti"));
    if (!ar_getMinFrameCount)
        ar_getMinFrameCount = (AudioRecord_getMinFrameCount) (dlsym(p_library,
                                                                    "_ZN7android11AudioRecord16getMinFrameCountEPij14audio_format_tj"));
    // check function.
    LOGI("getInputBufferSize %p", ar_getMinFrameCount);

    if (!(as_getInputBufferSize)) {
        dlclose(p_library);
        return false;
    }

    if (!((ar_ctor24 || ar_ctor19 || ar_ctor16 || ar_ctor17 || ar_ctor8 || ar_ctor9)
          && ar_dtor && (ar_start || ar_start_below9) && ar_stop && ar_read &&
          ar_getMinFrameCount)) {
        dlclose(p_library);
        return false;
    }

    return true;
}

void getFunctionsAudioSystem(void *p_library) {
    setParameters = (int (*)(int, void *)) dlsym(p_library,
                                                 "_ZN7android11AudioSystem13setParametersEiRKNS_7String8E");

    as_getInputBufferSize =
            (AudioSystem_getInputBufferSize) (dlsym(p_library,
                                                    "_ZN7android11AudioSystem18getInputBufferSizeEj14audio_format_tjPm"));

    if (!as_getInputBufferSize)
        as_getInputBufferSize =
                (AudioSystem_getInputBufferSize) (dlsym(p_library,
                                                        "_ZN7android11AudioSystem18getInputBufferSizeEjiiPj"));

    if (!as_getInputBufferSize)
        as_getInputBufferSize =
                (AudioSystem_getInputBufferSize) (dlsym(p_library,
                                                        "_ZN7android11AudioSystem18getInputBufferSizeEj14audio_format_tiPj"));
}

void getConstructorsAudioRecord(void *p_library) {/** Audio Record **/
    ar_ctor24 =
            (AudioRecord_ctor24) (dlsym(p_library,
                                        "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjRKNS_8String16EmPFviPvS6_ES6_j15audio_session_tNS0_13transfer_typeE19audio_input_flags_tiiPK18audio_attributes_t"));

    if (!ar_ctor24)
        ar_ctor24 =
                (AudioRecord_ctor24) (dlsym(p_library,
                                            "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjRKNS_8String16EjPFviPvS6_ES6_j15audio_session_tNS0_13transfer_typeE19audio_input_flags_tiiPK18audio_attributes_t"));
    if (!ar_ctor24)
        ar_ctor24 =
                (AudioRecord_ctor24) (dlsym(p_library,
                                            "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjRKNS_8String16EjPFviPvS6_ES6_jiNS0_13transfer_typeE19audio_input_flags_tiiPK18audio_attributes_t"));

    if (!ar_ctor24)
        ar_ctor24 =
                (AudioRecord_ctor24) (dlsym(p_library,
                                            "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjRKNS_8String16EjPFviPvS6_ES6_j15audio_session_tNS0_13transfer_typeE19audio_input_flags_tjiPK18audio_attributes_t"));

    if (!ar_ctor24)
        ar_ctor19 =
                (AudioRecord_ctor19) (dlsym(p_library,
                                            "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjiPFviPvS3_ES3_iiNS0_13transfer_typeE19audio_input_flags_t"));
    if (!ar_ctor19)
        ar_ctor17 =
                (AudioRecord_ctor17) (dlsym(p_library,
                                            "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjiPFviPvS3_ES3_ii"));

    if (!ar_ctor17)
        ar_ctor16 =
                (AudioRecord_ctor16) (dlsym(p_library,
                                            "_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjiNS0_12record_flagsEPFviPvS4_ES4_ii"));

    if (!ar_ctor16)
        ar_ctor9 = (AudioRecord_ctor9) (dlsym(p_library,
                                              "_ZN7android11AudioRecordC1EijijijPFviPvS1_ES1_ii"));

    if (!ar_ctor9)
        ar_ctor8 = (AudioRecord_ctor8) (dlsym(p_library,
                                              "_ZN7android11AudioRecordC1EijijijPFviPvS1_ES1_i"));
}

extern "C" {
/**
 * Init function. Open system library (libmedia.so) and get some useful function int AudioRecord class.
 */
jboolean Java_net_callrec_library_recorder_AudioRecordNative_nativeInit(JNIEnv *pEnv,
                                                                          jclass pThis) {
    void *p_library;
    p_library = dlopen("libmedia.so", RTLD_NOW);

    void *p_libraryutils;
    p_libraryutils = dlopen("libutils.so", RTLD_NOW);

    if (!p_library || !p_libraryutils) {
        LOGI("Error: %s", dlerror());
        return JNI_FALSE;
    }

    getFunctionsLibMedia(p_library);
    getFunctionsLibUtils(p_libraryutils);

    audiorecord = new AndroidAudioRecord();

    return JNI_TRUE;
}


/**
 * Destroy instance of AndroidAudioRecord class
 */
jboolean Java_net_callrec_library_recorder_AudioRecordNative_nativeDestroy(JNIEnv *pEnv,
                                                                             jclass pThis) {
    audiorecord->close();
    audiorecord = NULL;
    return JNI_TRUE;
}

/**
 * Set some parameter for instance of AndroidAudioRecord class.
 *
 * @param jint 		audiosourc
 * @param jint 		samplerate
 * @param jint 		audioformat
 * @param jint		channels
 * @param jint		size
 *
 * @return jboolean
 */
jboolean Java_net_callrec_library_recorder_AudioRecordNative_nativeCreate(JNIEnv *pEnv,
                                                                            jclass pThis,
                                                                            jint audiosource,
                                                                            jint samplerate,
                                                                            jint audioformat,
                                                                            jint channels,
                                                                            jint size) {
    audiorecord->set(audiosource, (uint32_t) samplerate, audioformat, (unsigned int) channels,
                     (unsigned int) size);
    //todo very strange bug, if you remove the next line then an error in the function audiorecord->set when called ar_ctor24!!!
    audiorecord->inputPrivate();
//
//    LOGI("inputPrivate %i", inpPriv);
//
//    LOGI("set_parameters %i", setParameters(inpPriv, &str8_1));

    LOGI("nativeCreate");
    return true;
}

/**
 * Start record by calling start function of AndroidAudioRecord class.
 *
 * @return jint
 */
jint Java_net_callrec_library_recorder_AudioRecordNative_nativeStart(JNIEnv *pEnv,
                                                                       jclass pThis) {
    return audiorecord->start();
}

/**
 * Stop record by calling stop function of AndroidAudioRecord class.
 *
 * @return int
 */
int Java_net_callrec_library_recorder_AudioRecordNative_nativeStop(JNIEnv *pEnv,
                                                                     jclass pThis) {
    free(audiorecord->lbuffer);
    return audiorecord->stop();
}
//_ZN7android10AudioTrackC1E
/**
 * Read sound to buffer by calling read function of AndroidAudioRecord class, and convert it from void* to jbyteArray.
 *
 * @param  jbyteArray 		buffer
 * @param  jint				bufferSize
 *
 * @return jbyteArray
 */
jbyteArray Java_net_callrec_library_recorder_AudioRecordNative_nativeRead(JNIEnv *pEnv,
                                                                            jclass pThis,
                                                                            jbyteArray buffer,
                                                                            jint bufferSize) {
    audiorecord->lbuffer = malloc(bufferSize);
    audiorecord->size = bufferSize;

    audiorecord->read(audiorecord->lbuffer, bufferSize);

    jbyte *tmp = new jbyte[audiorecord->size];

    memcpy(tmp, audiorecord->lbuffer, audiorecord->size);

    jbyteArray lbuffer = pEnv->NewByteArray(audiorecord->size);

    pEnv->SetByteArrayRegion(lbuffer, 0, audiorecord->size, tmp);

    free(tmp);
    return lbuffer;
}

/**
 * Helper method to get an input size of buffer for one second record, it's use the method of AudioSystem class.
 * @param 	jint			samplerate
 *
 * @param 	jint 			audioformat
 * @param 	jint  			channels
 *
 * @return 	unsiged int		size
 */
jint Java_net_callrec_library_recorder_AudioRecordNative_nativeGetBufferSize(JNIEnv *pEnv,
                                                                               jclass pThis,
                                                                               jint samplerate,
                                                                               jint audioformat,
                                                                               jint channels) {
    unsigned int size = 0;
    if (as_getInputBufferSize) {
        as_getInputBufferSize(samplerate, audioformat, channels, &size);
    }
    LOGI("buffer size %d", size);
    return size;
}

jint Java_net_callrec_library_recorder_AudioRecordNative_nativeGetMinFrame(JNIEnv *pEnv,
                                                                           jclass pThis,
                                                                           jint samplerate,
                                                                           jint audioformat,
                                                                           jint channels) {
    int size = 0;
    if (ar_getMinFrameCount) {
        ar_getMinFrameCount(&size, samplerate, audioformat, channels);
    }
    LOGI("buffer size %d", size);
    return size;
}
}