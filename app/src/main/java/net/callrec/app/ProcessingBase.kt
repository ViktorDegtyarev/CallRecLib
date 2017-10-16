package net.callrec.app

import android.app.Service
import android.content.Context
import android.content.Intent
import android.media.AudioFormat
import android.os.Build
import android.os.Handler
import net.callrec.library.fix.RecorderHelper

/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
abstract class ProcessingBase(val context: Context) : IProcessing {
    object Constants {
        var ACTION_RECEIVER_UPDATE_NOTIFICATION: String = "ACTION_RECEIVER_UPDATE_NOTIFICATION"
    }

    protected var recorder: AudioRecorder? = null
    protected val recorderRun = RecorderRunnable()
    lateinit var recHandler: Handler
    protected var recordingStartedFlag: Boolean = false

    protected var phoneNumber: String = ""
    protected var typeCall: Int = -1

    protected var formatFile: String = ""
    protected var typeRecorder: TypeRecorder? = null
    protected var audioSource = -1
    protected var outputFormat: Int = 0
    protected var encoder: Int = 0
    protected var stereoChannel: Boolean = false
    protected var samplingRate: Int = 0
    protected var audioEncodingBitRate: Int = 0
    protected var filePathNoFormat: String = ""

    protected var forcedStart: Boolean = false

    abstract fun isServiceOn(): Boolean
    abstract fun getPauseBeforeRecord(): Int
    abstract fun getCheckRulesRecord(): Boolean
    abstract fun prepareAudioPreferences()
    abstract fun stopThisService()
    private fun isFirstStart(startId: Int) = startId <= 1

    @Throws(ProcessingException::class)
    abstract fun makeOutputFile(): String

    @Throws(Exception::class)
    private fun startRecorder() {
        val recorderHelper = RecorderHelper.getInstance()
        var startFixWavFormat = false

        makeOutputFile()
        prepareAudioPreferences()

        when (typeRecorder) {
            TypeRecorder.WAV -> {
                val channelConfig = if (stereoChannel) AudioFormat.CHANNEL_IN_STEREO else AudioFormat.CHANNEL_IN_MONO
                recorder = RecorderFactory.createWavRecorder(audioSource, samplingRate, channelConfig,
                        AudioFormat.ENCODING_PCM_16BIT, filePathNoFormat)

                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
                    recorderHelper.startFixCallRecorder(context, recorder!!.audioSessionId)
                    startFixWavFormat = true
                }
            }
        }

        recorder!!.start()

        recordingStartedFlag = true

        onStartRecord()

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN && startFixWavFormat) {
            recorderHelper.stopFixCallRecorder()
        }
    }

    private fun stopRecorder() {
        if (recorder == null) return

        if (recorder!!.isRecorded()) {
            recorder!!.stop()
            onStopRecord()
        }
    }

    protected open fun prepareService(intent: Intent) {
        phoneNumber = intent.getStringExtra(IntentKey.PHONE_NUMBER)
        typeCall = intent.getIntExtra(IntentKey.TYPE_CALL, -1)
    }

    protected open fun handleFirstStart(intent: Intent): Int {
        prepareService(intent)

        if (forcedStart) {
            startRecord(0)
        } else {
            getPauseBeforeRecord()

            if (!getCheckRulesRecord()) {
                onCheckRulesRecord(false)
                return Service.START_NOT_STICKY
            }

            startRecord(getPauseBeforeRecord() * 1000)
        }

        return Service.START_REDELIVER_INTENT
    }

    protected open fun handleNoFirstStart(intent: Intent) {
        if (forcedStart) {
            if (recorder == null || recorder!!.isRecorded()) {
                startRecord(0)
            }
        }
    }

    protected open fun startRecord(delayMS: Int) {
        recHandler.removeCallbacks(recorderRun)

        onPreStartRecord()

        if (delayMS == 0) {
            recHandler.post(recorderRun)
        } else {
            recHandler.postDelayed(recorderRun, delayMS.toLong())
            onWaitStartRecord()
        }
    }

    protected open fun stopRecord() {
        recHandler.removeCallbacks(recorderRun)
        stopRecorder()
    }

    open protected fun onCheckRulesRecord(check: Boolean) {}
    open protected fun onWaitStartRecord() {}
    open protected fun onStartRecord() {}
    open protected fun onStopRecord() {}
    open protected fun onRecorderError(e: Exception) {}
    open protected fun onRecorderError(e: RecorderBase.RecorderException) {}

    open protected fun onRecorderError(e: ProcessingException) {}

    open protected fun onPreStartRecord() {
    }

    override fun onCreate() {
        recHandler = Handler()
    }

    override fun onStartCommand(intent: Intent, flags: Int, startId: Int): Int {
        if (!isServiceOn()) {
            stopThisService()
            return Service.START_NOT_STICKY
        }

        forcedStart = intent.getBooleanExtra(IntentKey.FORCED_START, false)

        if (isFirstStart(startId)) return handleFirstStart(intent)
        handleNoFirstStart(intent)
        return Service.START_NOT_STICKY
    }

    override fun onDestroy() {
        stopRecord()
    }

    public object IntentKey {
        val PHONE_NUMBER = "PHONE_NUMBER"
        val TYPE_CALL = "TYPE_CALL"
        val FORCED_START = "FORCED_START"
    }

    public object TypeCall {
        val INC = 1
        val OUT = 2
    }

    enum class TypeRecorder { WAV }

    inner class RecorderRunnable : Runnable {
        override fun run() {
            try {
                startRecorder()
            } catch (e: RecorderBase.RecorderException) {
                e.printStackTrace()
                onRecorderError(e)
                stopThisService()
            } catch (e: ProcessingException) {
                e.printStackTrace()
                onRecorderError(e)
                stopThisService()
            }
        }
    }

    public class ProcessingException : Exception {
        val code: Int

        constructor(message: String, codeError: Int) : super(message) {
            this.code = codeError
        }

        constructor(message: String, throwable: Throwable, codeError: Int) : super(message, throwable) {
            this.code = codeError
        }
    }

    public object CodeError {
        val ERROR_CREATE_FOLDER = 4
        val ERROR_PATH_IS_EMPTY = 5
        val ERROR_FILE_IS_EXIST = 6
        val ERROR_CREATE_FILE = 7
    }
}