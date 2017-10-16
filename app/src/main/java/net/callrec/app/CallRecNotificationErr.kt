package net.callrec.app

import android.app.Notification
import android.app.PendingIntent
import android.content.Intent
import android.support.v4.app.NotificationCompat


/**
 * Created by Viktor Degtyarev on 16.10.17
 * E-mail: viktor@degtyarev.biz
 */
class CallRecNotificationErr(val processing: ProcessingBase) : INotification<ProcessingBase> {
    override fun build(): Notification {
        val builder = NotificationCompat.Builder(processing.context)

        builder.setOngoing(true)
        builder.setAutoCancel(true)

        val notifyIntent = Intent(processing.context, MainActivity::class.java)
        notifyIntent.flags = Intent.FLAG_ACTIVITY_SINGLE_TOP

        val pendingIntent = PendingIntent.getActivity(processing.context, 0, notifyIntent, PendingIntent.FLAG_UPDATE_CURRENT)

        builder.setSmallIcon(R.mipmap.ic_launcher)
                .setContentTitle(processing.context.getString(R.string.app_name))
                .setContentText("Recording error")
                .setContentIntent(pendingIntent)

        return builder.build()
    }

}
