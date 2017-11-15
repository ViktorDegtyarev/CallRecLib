package net.callrec.library.recorder


/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
interface AudioRecorder {
    val duration: Long
    val filePath: String
    val audioSessionId: Int

    fun isRecorded(): Boolean
    fun isPaused(): Boolean
    fun isStopped(): Boolean
    fun prepare()
    fun start()
    fun stop()
}