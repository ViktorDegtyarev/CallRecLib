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

#include <jni.h>
#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>
#include <malloc.h>
#include <android/log.h>
#include "audio.h"


#define TAG_NAME    "CallRecLib"

#define log_info(fmt, args...) __android_log_print(ANDROID_LOG_INFO, TAG_NAME, (const char *) fmt, ##args)
#define log_err(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG_NAME, (const char *) fmt, ##args)

#ifdef LOG_DBG
#define log_dbg log_info
#else
#define log_dbg(...)
#endif


extern "C" {
JNIEXPORT jint JNICALL
Java_net_callrec_library_fix_CallRecorderFix_stopFix(JNIEnv *env, jobject thiz);
JNIEXPORT jint JNICALL
Java_net_callrec_library_fix_CallRecorderFix_startFix(JNIEnv *env, jobject thiz, jint i2);
JNIEXPORT jint JNICALL
Java_net_callrec_library_fix_CallRecorderFix_startFix7(JNIEnv *env, jobject thiz);
JNIEXPORT jint JNICALL
Java_net_callrec_library_fix_CallRecorderFix_load(JNIEnv *env,
                                                  jobject thiz);
}

int setParam(jint i, jint i2, const char *kvp) {
    pthread_mutex_lock(&mt_1);

    as = i2;

//    LOGI("sp as: %i", as);

    string8(&str8_1, kvp);
    if (!str8_1) {
        log_info("Filed to create str8");
    } else {
        log_info("create str8 success");
    }
    //todo set your app name
    string16(&str16, "net.callrec.app");
    if (!str16) {
        log_info("Filed to create str16");
    } else {
        log_info("create str16 success");
    }

//    LOGI("kvps: %p", kvps);

    cm = (int) i;

    pthread_cond_signal(&cnd_1);
    pthread_mutex_unlock(&mt_1);

    return 0;
}

void *tasp(void *threadid) {
    while (1) {
//        LOGI("function tasp while");
        pthread_mutex_lock(&mt_1);
        if (cm == CM_D) {
//            LOGI("function: %i", cm);
            pthread_cond_wait(&cnd_1, &mt_1);
        } else if (setParameters != NULL) {
//            LOGI("function: %i", cm);
            log_info("audiosession = %i", as);
//            int inp = inputPrivate();
//            log_info("inputPrivate = %i", inp);
            log_info("%i", setParameters(as, &str8_1));
//            stop();
        }
        pthread_mutex_unlock(&mt_1);
    }
}

int startParam(jint i2, const char *kvp) {
    return setParam(1, i2, kvp);
}

int stopParam() {
    pthread_mutex_lock(&mt_1);

    cm = 0;

//    LOGI("as: %i", as);
//    LOGI("kvps: %s", kvp);

    pthread_cond_signal(&cnd_1);
    pthread_mutex_unlock(&mt_1);

//    stop();

    return 0;

//    dlclose(handle);
}

int load(JNIEnv *env, jobject thiz) {
    void *handleLibMedia;
    void *handleLibUtils;
    int result = -1;

    pthread_t newthread = (pthread_t) thiz;

    handleLibMedia = dlopen("libmedia.so", RTLD_NOW | RTLD_GLOBAL);
    if (handleLibMedia != NULL) {
        setParameters = (int (*)(int, void *)) dlsym(handleLibMedia,
                                                     "_ZN7android11AudioSystem13setParametersEiRKNS_7String8E");
        if (setParameters != NULL) {
            result = 0;
        }
    } else {
        result = -1;
    }

    handleLibUtils = dlopen("libutils.so", RTLD_NOW | RTLD_GLOBAL);
    if (handleLibUtils != NULL) {
        string8 = (CreateString8) dlsym(handleLibUtils,
                                        "_ZN7android7String8C2EPKc");

        string16 = (CreateString16) (dlsym(handleLibUtils,
                                           "_ZN7android8String16C1EPKc"));

        if (string16 == NULL) {
            log_info("String 16 not found");
        } else {
            log_info("String 16 found!!!");
        }

        if (string8 == NULL) {
            result = -1;
        }
    } else {
        result = -1;
    }

    cm = CM_D;

    int resultTh = pthread_create(&newthread, NULL, tasp, NULL);

//    LOGI("pthread_create result: %i", resultTh);

//    dlclose(handleLibMedia);
//    dlclose(handleLibUtils);

    return result;
}

JNIEXPORT jint JNICALL
Java_net_callrec_library_fix_CallRecorderFix_load(JNIEnv *env,
                                                  jobject thiz) {
    return load(env, thiz);
}

JNIEXPORT jint JNICALL
Java_net_callrec_library_fix_CallRecorderFix_startFix(JNIEnv *env, jobject thiz, jint i2) {
    return startParam(i2 + 1, kvp_def);
}

JNIEXPORT jint JNICALL
Java_net_callrec_library_fix_CallRecorderFix_stopFix(JNIEnv *env, jobject thiz) {
    return stopParam();
}

JNIEXPORT jint JNICALL
Java_net_callrec_library_fix_CallRecorderFix_startFix7(JNIEnv *env, jobject type) {
    int inpPriv = audiorecord->inputPrivate();
    LOGI("inputPrivate %i", inpPriv);
    return startParam(inpPriv, kvp_as4);
}


