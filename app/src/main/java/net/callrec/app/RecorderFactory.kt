package net.callrec.app

import net.callrec.library.recorder.NativeWavRecorder
import net.callrec.library.recorder.AudioRecorder
import net.callrec.library.recorder.WavRecorder


/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
object RecorderFactory {
    fun createWavRecorder(audioSource: Int, sampleRateInHz: Int, channelConfig: Int, audioEncoding: Int,
                          filePathNoFormat: String): AudioRecorder {
        return WavRecorder(audioSource, sampleRateInHz, channelConfig, audioEncoding, filePathNoFormat)
    }

    fun createNativeWavRecorder(audioSource: Int, sampleRateInHz: Int, channelConfig: Int, audioEncoding: Int,
                                filePathNoFormat: String): AudioRecorder {
        return NativeWavRecorder(audioSource, sampleRateInHz, channelConfig, audioEncoding, filePathNoFormat)
    }
}
