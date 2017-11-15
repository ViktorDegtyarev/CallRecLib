#include "jni.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <android/log.h>
#include <pthread.h>


#define  LOG_TAG    "CallRecLib"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

/**Type to transfer buffer, i use TRANSFER_DEFAULT as a default parameter for construtor */
enum transfer_type {
    TRANSFER_DEFAULT, // not specified explicitly; determine from other parameters
    TRANSFER_CALLBACK,  // callback EVENT_MORE_DATA
    TRANSFER_OBTAIN, // FIXME deprecated: call obtainBuffer() and releaseBuffer()
    TRANSFER_SYNC,      // synchronous read()
};

/**A bitmask of acourstic values. It enables AGC, NS, IIR */
enum record_flags {
    RECORD_AGC_ENABLE = 0x0001,
    RECORD_NS_ENABLE = 0x0002,
    RECORD_IIR_ENABLE = 0x0004,
};

/**Input mode, use AUDIO_INPUT_FLAG_NONE as default parameter in constructor */
typedef enum {
    AUDIO_INPUT_FLAG_NONE = 0x0,        // no attributes
    AUDIO_INPUT_FLAG_FAST = 0x1, // prefer an input that supports "fast tracks"
} audio_input_flags_t;

/** Events used by AudioRecord callback function (callback_t).
 *
 * to keep in sync with frameworks/base/media/java/android/media/AudioRecord.java
 */
enum sync_event_t {
    SYNC_EVENT_SAME = -1, SYNC_EVENT_NONE = 0, SYNC_EVENT_PRESENTATION_COMPLETE,

    SYNC_EVENT_CNT,
};

extern "C" {

#define SIZE_OF_AUDIORECORD 512


/**
 * Helper method to get an input size of buffer for one second record, it's depend on sample rate, audio format, channels mask.
 *
 *  name mangling for all android's version:
 * 		_ZN7android11AudioSystem18getInputBufferSizeEj14audio_format_tjPj
 * 		_ZN7android11AudioSystem18getInputBufferSizeEjiiPj
 * 		_ZN7android11AudioSystem18getInputBufferSizeEj14audio_format_tiPj
 *
 * @param 	unsigned int  		sample_rate
 * @param 	int 				audio_format
 * @param 	unsigned int  		channels mask
 * @param 	int*				size
 * @return 	int 				size of buffer
 */
typedef int (*AudioSystem_getInputBufferSize)(unsigned int, int, unsigned int,
                                              unsigned int *);
typedef void (*CreateString16)(void *, const char *);
typedef void (*CreateString8)(void *, const char *);
typedef int (*SetParameters)(int, void *);

/** Audio Record **/
/**
 * Constructor of AudioRecord use for API 24
 *
 *  name mangling for API 24:
 * 		_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjRKNS_8String16EmPFviPvS6_ES6_j15audio_session_tNS0_13transfer_typeE19audio_input_flags_tiiPK18audio_attributes_t
 *
 * @param 	audio_sourc_t  		audio_source
 * @param 	unsigned int 		sample_rate
 * @param 	audio_format_t  	format
 * @param 	unsigned int  		channels
 * @param	opPackageName       The package name used for app ops
 * @param	frameCount          Minimum size of track PCM buffer in frames.
 * @param	callback_function	cbf
 * @param	void*				user
 * @param	int					notificationFrames
 * @param	int 				sessionId
 * @param	transfer_type		transfer_type
 * @param   flags              See comments on audio_input_flags_t in <system/audio.h>
 * @param   uid
 * @param   pid
 * @param   pAttributes:        If not NULL, supersedes inputSource for use case selection.
 */
typedef void (*AudioRecord_ctor24)(void *, int, unsigned int, int, unsigned int, void *,
                                   unsigned long, void (*)(int, void *, void *), void *, int, int,
                                   transfer_type, int, int, int, unsigned short);
/**
 * Constructor of AudioRecord use for API 19
 *
 *  name mangling for API 19:
 * 		_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjiPFviPvS3_ES3_iiNS0_13transfer_typeE19audio_input_flags_t
 *
 * @param 	audio_sourc_t  		audio_source
 * @param 	unsigned int 		sample_rate
 * @param 	audio_format_t  	format
 * @param 	unsigned int  		channels
 * @param	int					size
 * @param	callback_function	cbf
 * @param	void*				user
 * @param	int					notificationFrames
 * @param	int 				sessionId
 * @param	transfer_type		transfer_type
 * @param	audio_input_flags_t	input_flags
 */
typedef void (*AudioRecord_ctor19)(void *, int, unsigned int, int, unsigned int,
                                   int, void (*)(int, void *, void *), void *, int, int,
                                   transfer_type,
                                   int);
/**
 * Constructor of AudioRecord use for above API 17
 *
 *  name mangling for above 19:
 * 		_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjiPFviPvS3_ES3_ii
 *
 * @param 	audio_sourc_t  		audio_source
 * @param 	unsigned int 		sample_rate
 * @param 	audio_format_t  	format
 * @param 	unsigned int  		channels
 * @param	int					size
 * @param	callback_function	cbf
 * @param	void*				user
 * @param	int					notificationFrames
 * @param	int 				sessionId
 */
typedef void (*AudioRecord_ctor17)(void *, int, unsigned int, int, unsigned int,
                                   int, void (*)(int, void *, void *), void *, int, int);

/**
 * Constructor of AudioRecord use for API 16
 *
 *  name mangling for API 16:
 * 		_ZN7android11AudioRecordC1E14audio_source_tj14audio_format_tjiNS0_12record_flagsEPFviPvS4_ES4_ii
 *
 * @param 	audio_sourc_t  		audio_source
 * @param 	unsigned int 		sample_rate
 * @param 	audio_format_t  	format
 * @param 	unsigned int  		channels
 * @param	int					size
 * @param 	record_flags 		record_flags
 * @param	callback_function	cbf
 * @param	void*				user
 * @param	int					notificationFrames
 * @param	int 				sessionId
 */
typedef void (*AudioRecord_ctor16)(void *, int, unsigned int, int, unsigned int,
                                   int, record_flags, void (*)(int, void *, void *), void *, int,
                                   int);

/**
 * Constructor of AudioRecord use for above API 9
 *
 *  name mangling for above API 9:
 * 		_ZN7android11AudioRecordC1EijijijPFviPvS1_ES1_ii
 *
 * @param 	audio_sourc_t  		audio_source
 * @param 	unsigned int 		sample_rate
 * @param 	audio_format_t  	format
 * @param 	unsigned int  		channels
 * @param	int					size
 * @param 	int 		 		record_flags
 * @param	callback_function	cbf
 * @param	void*				user
 * @param	int					notificationFrames
 * @param	int 				sessionId
 */
typedef void (*AudioRecord_ctor9)(void *, int, unsigned int, int, unsigned int,
                                  int, unsigned int, void (*)(int, void *, void *), void *, int,
                                  int);

/**
 * Constructor of AudioRecord use for API 8
 *
 *  name mangling for API 8:
 * 		_ZN7android11AudioRecordC1EijijijPFviPvS1_ES1_i
 *
 * @param 	audio_sourc_t  		audio_source
 * @param 	unsigned int 		sample_rate
 * @param 	audio_format_t  	format
 * @param 	unsigned int  		channels
 * @param	int					size
 * @param 	int 		 		record_flags
 * @param	callback_function	cbf
 * @param	void*				user
 * @param	int					notificationFrames
 */
//_ZN7android11AudioRecordC1EijijijPFviPvS1_ES1_i
typedef void (*AudioRecord_ctor8)(void *, int, unsigned int, int, unsigned int,
                                  int, unsigned int, void (*)(int, void *, void *), void *, int);

typedef void (*AudioRecord_dtor)(void *);

typedef int (*AudioRecord_start)(void *, int, int);

typedef int (*AudioRecord_start_below9)();

typedef void (*AudioRecord_stop)(void *);

typedef int (*AudioRecord_read)(void *, void *, unsigned int, bool);
typedef int (*AudioRecord_getMinFrameCount)(int *, unsigned int, int, int);
typedef int (*AudioRecord_input_private)(void *);

static AudioSystem_getInputBufferSize as_getInputBufferSize;
static CreateString16 string16;
static CreateString16 string8;
static SetParameters setParameters;

static AudioRecord_ctor24 ar_ctor24;
static AudioRecord_ctor19 ar_ctor19;
static AudioRecord_ctor17 ar_ctor17;
static AudioRecord_ctor16 ar_ctor16;
static AudioRecord_ctor9 ar_ctor9;
static AudioRecord_ctor8 ar_ctor8;
static AudioRecord_dtor ar_dtor;
static AudioRecord_start ar_start;
static AudioRecord_start_below9 ar_start_below9;
static AudioRecord_input_private ar_input_private;
static AudioRecord_stop ar_stop;
static AudioRecord_read ar_read;
static AudioRecord_getMinFrameCount ar_getMinFrameCount;

static void *str16 = 0;
static void *str8_1 = 0;
static const char *kvp_def = "input_source=4";
static const char *kvp_as4 = "input_source=4;routing=-2147483584";
static const char *kvp_as7 = "input_source=4;routing=-2147483647";

static int cm;
static const int CM_D = 0;
static int as;

static pthread_mutex_t mt_1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cnd_1;

class AndroidAudioRecord {
private:
    void *mAudioRecord; //Instance of audiorecord
public:
    void *lbuffer;
    int size;

    AndroidAudioRecord();

    virtual ~AndroidAudioRecord();

    void close();

    bool set(int audio_source, uint32_t sampleRate, int format,
             unsigned int channels, unsigned int size);

    int inputPrivate();

    int start();

    int stop();

    int read(void *buffer, int size);
};

extern AndroidAudioRecord *audiorecord;
}
