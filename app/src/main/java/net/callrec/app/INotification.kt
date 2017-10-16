package net.callrec.app

import android.app.Notification

/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
interface INotification<T> {
    fun build(): Notification
}