package net.callrec.app

import android.content.Intent

/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
interface IProcessing {
    fun onCreate()
    fun onStartCommand(intent: Intent, flags: Int, startId: Int): Int
    fun onDestroy()
}