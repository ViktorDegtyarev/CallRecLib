package net.callrec.app

import android.app.Service
import android.content.Intent
import net.callrec.library.recorder.base.RecorderBase
import org.jetbrains.anko.notificationManager

/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
abstract class ProcessingBaseNotification(val service: Service) : ProcessingBase(service.applicationContext) {
    open val notifyDefId: Int = 1
    open val notifyErrId: Int = 2
    private var notifyWaitDestroy = false

    abstract fun getNotificationOk(): INotification<*>
    abstract fun getNotificationWait(): INotification<*>?
    abstract fun getNotificationUpdate(): INotification<*>
    abstract fun getNotificationErr(e: Exception): INotification<*>
    abstract fun getNotificationErr(e: RecorderBase.RecorderException): INotification<*>
    abstract fun getNotificationErr(e: ProcessingException): INotification<*>

    override fun prepareService(intent: Intent) {
        super.prepareService(intent)
        if (getPauseBeforeRecord() > 0 || !getCheckRulesRecord()) {
            val notification = getNotificationWait()
            if (notification != null) {
                service.notificationManager.notify(notifyDefId, notification.build())
                notifyWaitDestroy = true
            }
        }
    }

    override fun onStartRecord() {
        super.onStartRecord()
        service.startForeground(notifyDefId, getNotificationOk().build())
    }

    override fun onDestroy() {
        super.onDestroy()
        if (notifyWaitDestroy) service.notificationManager.cancel(notifyDefId)
    }

    override fun onRecorderError(e: Exception) {
        super.onRecorderError(e)
        service.stopForeground(true)
        service.notificationManager.notify(notifyErrId, getNotificationErr(e).build())
    }

    override fun onRecorderError(e: RecorderBase.RecorderException) {
        super.onRecorderError(e)
        service.stopForeground(true)
        service.notificationManager.notify(notifyErrId, getNotificationErr(e).build())
    }

    override fun onRecorderError(e: ProcessingException) {
        super.onRecorderError(e)
        service.stopForeground(true)
        service.notificationManager.notify(notifyErrId, getNotificationErr(e).build())
    }

    fun updateNotification() {
        service.notificationManager.notify(notifyDefId, getNotificationUpdate().build())
    }
}