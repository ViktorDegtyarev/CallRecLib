package net.callrec.app

import android.media.AudioFormat
import android.media.AudioRecord

import java.io.FileNotFoundException
import java.io.IOException
import java.io.RandomAccessFile

/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
class WavRecorder(audioSource: Int, sampleRateInHz: Int, channelConfig: Int, audioEncoding: Int, filePathNoFormat: String) :
        AudioRecorderBase(audioSource, sampleRateInHz, channelConfig, audioEncoding, filePathNoFormat.plus(".wav")) {

    override fun handleThread() {
        writeAudioDataToFile()
    }

    override fun handleStop() {
        addHeaderWavFile(filePath)
    }

    private fun writeAudioDataToFile() {
        val data = ByteArray(bufferSizeInBytes)
        var os: RandomAccessFile? = null
        try {
            os = RandomAccessFile(filePath, "rw")
        } catch (e: FileNotFoundException) {
            e.printStackTrace()
        }

        var read = 0

        if (null != os) {
            while (state === RecorderBase.State.RECORD) {
                read = audioRecord!!.read(data, 0, bufferSizeInBytes)
                if (AudioRecord.ERROR_INVALID_OPERATION != read) {
                    try {
                        os.write(data)
                    } catch (e: IOException) {
                        e.printStackTrace()
                    }

                }
            }

            try {
                os.close()
            } catch (e: IOException) {
                e.printStackTrace()
            }

        }
    }

    private fun addHeaderWavFile(file: String) {
        val writeFile: RandomAccessFile

        var totalAudioLen: Long = 0
        var totalDataLen = totalAudioLen + 36
        val longSampleRate = sampleRateInHz.toLong()
        var channels = 1

        when (channelConfig) {
            AudioFormat.CHANNEL_IN_MONO -> channels = 1

            AudioFormat.CHANNEL_IN_STEREO -> channels = 2
        }

        var recorderBpp = 8
        when (audioEncoding) {
            AudioFormat.ENCODING_PCM_8BIT -> recorderBpp = 8

            AudioFormat.ENCODING_PCM_16BIT -> recorderBpp = 16
        }

        val byteRate = recorderBpp.toLong() * longSampleRate * channels.toLong() / 8

        try {
            writeFile = RandomAccessFile(file, "rw")

            totalAudioLen = writeFile.channel.size()
            totalDataLen = totalAudioLen + 36

            writeFile.seek(0)
            writeWaveFileHeader(writeFile, totalAudioLen, totalDataLen, longSampleRate,
                    channels, byteRate, recorderBpp)

            writeFile.close()

        } catch (e: FileNotFoundException) {
            e.printStackTrace()
        } catch (e: IOException) {
            e.printStackTrace()
        }

    }

    @Throws(IOException::class)
    private fun writeWaveFileHeader(out: RandomAccessFile, totalAudioLen: Long, totalDataLen: Long, longSampleRate: Long,
                                    channels: Int, byteRate: Long, recorderBPP: Int) {
        val header = ByteArray(44)
        //byte[] recBPP = intToByteArray(recorderBPP);
        header[0] = 'R'.toByte()  // RIFF/WAVE header
        header[1] = 'I'.toByte()
        header[2] = 'F'.toByte()
        header[3] = 'F'.toByte()
        header[4] = (totalDataLen and 0xff).toByte()
        header[5] = (totalDataLen shr 8 and 0xff).toByte()
        header[6] = (totalDataLen shr 16 and 0xff).toByte()
        header[7] = (totalDataLen shr 24 and 0xff).toByte()
        header[8] = 'W'.toByte()
        header[9] = 'A'.toByte()
        header[10] = 'V'.toByte()
        header[11] = 'E'.toByte()
        header[12] = 'f'.toByte()  // 'fmt ' chunk
        header[13] = 'm'.toByte()
        header[14] = 't'.toByte()
        header[15] = ' '.toByte()
        header[16] = 16  // 4 bytes: size of 'fmt ' chunk
        header[17] = 0
        header[18] = 0
        header[19] = 0
        header[20] = 1  // format = 1
        header[21] = 0
        header[22] = channels.toByte()
        header[23] = 0
        header[24] = (longSampleRate and 0xff).toByte()
        header[25] = (longSampleRate shr 8 and 0xff).toByte()
        header[26] = (longSampleRate shr 16 and 0xff).toByte()
        header[27] = (longSampleRate shr 24 and 0xff).toByte()
        header[28] = (byteRate and 0xff).toByte()
        header[29] = (byteRate shr 8 and 0xff).toByte()
        header[30] = (byteRate shr 16 and 0xff).toByte()
        header[31] = (byteRate shr 24 and 0xff).toByte()
        header[32] = (channels * recorderBPP / 8).toByte()  // block align
        header[33] = 0
        header[34] = recorderBPP.toByte()  // bits per sample
        header[35] = 0
        header[36] = 'd'.toByte()
        header[37] = 'a'.toByte()
        header[38] = 't'.toByte()
        header[39] = 'a'.toByte()
        header[40] = (totalAudioLen and 0xff).toByte()
        header[41] = (totalAudioLen shr 8 and 0xff).toByte()
        header[42] = (totalAudioLen shr 16 and 0xff).toByte()
        header[43] = (totalAudioLen shr 24 and 0xff).toByte()

        out.write(header, 0, 44)
    }

    private fun intToByteArray(value: Int): ByteArray {
        val result = ByteArray(4)

        result[3] = (value and 0xFF).toByte()
        result[2] = (value shr 8 and 0xFF).toByte()
        result[1] = (value shr 16 and 0xFF).toByte()
        result[0] = (value shr 24 and 0xFF).toByte()

        return result
    }
}
