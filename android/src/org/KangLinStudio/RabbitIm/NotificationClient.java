/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtAndroidExtras module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

package org.KangLinStudio.RabbitIm;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
//import android.graphics.Bitmap;
//import android.graphics.BitmapFactory;

public class NotificationClient extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static NotificationManager m_notificationManager;
    private static Notification.Builder m_builder;
    private static NotificationClient m_instance;
    private static final int m_nCount = 0;
    
    public NotificationClient()
    {
        m_instance = this;
    }

    public static void notify(String szMessage, String szTitle)
    {
       if (m_notificationManager == null) {
           m_notificationManager = (NotificationManager)m_instance.getSystemService(Context.NOTIFICATION_SERVICE);
           m_builder = new Notification.Builder(m_instance);
           m_builder.setSmallIcon(R.drawable.icon);
           //Bitmap bitmap = BitmapFactory.decodeResource(this.getResources(), R.drawable.icon);
           //m_builder.setLargeIcon(bitmap);// 设置通知栏中的通知下拉后显示的图标，图片格式为bitmap型
           m_builder.setWhen(System.currentTimeMillis());//设置时间发生时间  
           m_builder.setAutoCancel(true);//当通知被用户点击时，通知将被清除（好象要设置intent后才会有效）   
           //实例化Intent  
           Intent intent = new Intent(m_instance, NotificationClient.class);  
           //获取PendingIntent  
           PendingIntent pi = PendingIntent.getActivity(m_instance, 0, intent, 0);  
           m_builder.setContentIntent(pi);// 设置点击通知将要启动的Inent
       }

       m_builder.setTicker(szTitle + " " + szMessage);//在状态栏上提示的文字  
       m_builder.setContentTitle(szTitle);//在下拉栏中提示的标题  
       m_builder.setContentText(szMessage);//在下拉栏中提示的文字  
       m_notificationManager.notify(m_nCount, m_builder.build());//m_nCount是下拉栏提示ID，每个ID对应一个下拉提示栏
   }

   public static void CancelAll()
   {
       m_notificationManager.cancelAll();
   }
}
