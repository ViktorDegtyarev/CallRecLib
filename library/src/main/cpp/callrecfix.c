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

typedef jint (*lspr)(JNIEnv *env, jobject thiz);

typedef int (*lstr)(void *, void *);

lstr fstr = NULL;

typedef int (*lasp)();

lasp audioSetParameters;

int cmd;

const int CM_D = 0;
//int CMD_SET_PARAMETER = 0;

pthread_mutex_t mt;
pthread_cond_t cnd;

int audioSession;
void *kvps;
char *kvp;

void *toString8(const char *val) {
    void *val2;
    void *result;

    val2 = malloc(0x400u);
    result = memset(val2, 0, 0x400u);
//    __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "memset V2: %p", &val2);
    if (fstr != NULL) {
        int res = fstr(val2, val);
//        __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "fstr res: %i", res);
//        __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "fstr v2: %p", &val2);
        result = val2;
    }
    return result;
}

int setParam(jint i, jint as) {
    pthread_mutex_lock(&mt);

    audioSession = (int) (as + 1);

//    __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "sp as: %i", as);

    kvp = "input_source=4";
    kvps = toString8(kvp);

//    __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "kvps: %p", kvps);

    cmd = (int) i;

    pthread_cond_signal(&cnd);
    pthread_mutex_unlock(&mt);

    return 0;
}

void *taskAudioSetParam(void *threadid) {
    while (1) {
//        __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "function taskAudioSetParam while");
        pthread_mutex_lock(&mt);
        if (cmd == CM_D) {
//            __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "function: %i", cm);
            pthread_cond_wait(&cnd, &mt);
        } else if (audioSetParameters != NULL) {
//            __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "function: %i", cm);
            audioSetParameters(audioSession, kvps);
        }
        pthread_mutex_unlock(&mt);
    }
}

int startParam(jint i2) {
    return setParam(1, i2);
}

int stopParam() {
    pthread_mutex_lock(&mt);

    cmd = 0;

//    __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "as: %i", as);
//    __android_log_print(ANDROID_LOG_DEBUG, APPNAME, "kvps: %s", kvp);

    pthread_cond_signal(&cnd);
    pthread_mutex_unlock(&mt);

    return 0;

//    dlclose(handle);
}

int load(JNIEnv *env, jobject thiz) {
    void *handleLibMedia;
    void *handleLibUtils;
    int result = -1;
    lspr func = NULL;

//    pthread_t newthread = (pthread_t) thiz;
    pthread_t newthread = (pthread_t) thiz;

    handleLibMedia = dlopen("libmedia.so", RTLD_NOW | RTLD_GLOBAL);
    if (handleLibMedia != NULL) {
        func = dlsym(handleLibMedia, "_ZN7android11AudioSystem13setParametersEiRKNS_7String8E");
        if (func != NULL) {
//            result = func(env, thiz);
            result = 0;
        }
        audioSetParameters = (lasp) func;
    } else {
        result = -1;
    }

    handleLibUtils = dlopen("libutils.so", RTLD_NOW | RTLD_GLOBAL);
    if (handleLibUtils != NULL) {
        fstr = dlsym(handleLibUtils, "_ZN7android7String8C2EPKc");
        if (fstr == NULL) {
            result = -1;
        }
    } else {
        result = -1;
    }

    cmd = CM_D;

    int resultTh = pthread_create(&newthread, NULL, taskAudioSetParam, NULL);

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
Java_net_callrec_library_fix_CallRecorderFix_startFix(JNIEnv *env, jobject thiz, jint as) {
    return startParam(as);
}

JNIEXPORT jint JNICALL
Java_net_callrec_library_fix_CallRecorderFix_stopFix(JNIEnv *env, jobject thiz) {
    return stopParam();
}


