package net.callrec.library.recorder.base

import net.callrec.library.recorder.AudioRecorder

/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
abstract class RecorderBase : AudioRecorder {
    protected enum class State {
        RECORD, PAUSE, STOP
    }

    protected var startTimeRecording: Long = -1
    override var duration: Long = 0
        get() {
            if (startTimeRecording.equals(-1)) return 0
            return System.currentTimeMillis() - startTimeRecording
        }

    protected var state = State.STOP

    @Throws(RecorderException::class)
    abstract override fun prepare()

    @Throws(RecorderException::class)
    abstract override fun start()

    abstract override fun stop()

    override fun isRecorded() = state == State.RECORD

    override fun isPaused() = state == State.PAUSE

    override fun isStopped() = state == State.STOP

    public class RecorderException : Exception {
        val codeError: Int

        constructor(message: String, codeError: Int) : super(message) {
            this.codeError = codeError
        }

        constructor(message: String, throwable: Throwable, codeError: Int) : super(message, throwable) {
            this.codeError = codeError
        }
    }

    public object CodeError {
        val ERROR_BUFFER_SIZE = 1
        val ERROR_BUFFER_SIZE_STEREO = 2
        val ERROR_INITIALIZE_RECORDER = 3
    }

}