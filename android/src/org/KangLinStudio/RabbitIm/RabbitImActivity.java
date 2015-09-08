/*
* android平台消息通知java实现  
*/

package org.KangLinStudio.RabbitIm;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.app.PendingIntent;
import android.util.Log;
//import android.graphics.Bitmap;
//import android.graphics.BitmapFactory;
import org.qtproject.qt5.android.bindings.QtActivity;

public class RabbitImActivity
    extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static final String TAG = "NotificationClient";
    private static NotificationManager m_notificationManager = null;
    private static Notification.Builder m_builder = null;
    private static RabbitImActivity m_instance = null;
    private static final int m_nCount = 0;//R.drawable.icon;
    
    public RabbitImActivity()
    {
        Log.d(TAG, "NotificationClient");
        m_instance = this;
    }
    
//*
    @SuppressWarnings("deprecation")
    public static void notify(String szMessage, String szTitle) {
        if(null == m_instance){
            Log.e(TAG, "m_instance is null");
            return;
        }
        if (null == m_notificationManager) {
            m_notificationManager = 
                (NotificationManager)m_instance.getSystemService(
                    Context.NOTIFICATION_SERVICE);
        }
   
        // 创建一个Notification
        Notification notification = new Notification();
        // 设置显示在手机最上边的状态栏的图标
        notification.icon = R.drawable.icon;
        // 当前的notification被放到状态栏上的时候，提示内容
        notification.tickerText = szTitle + " " + szMessage;
        // 当前时间
        notification.when = System.currentTimeMillis();
        
        // 更多的特征属性
        notification.flags |= Notification.FLAG_AUTO_CANCEL; //在通知栏上点击此通知后自动清除此通知
        // notification.flags |= Notification.FLAG_INSISTENT; //重复发出声音，直到用户响应此通知
        // notification.flags |= Notification.FLAG_ONGOING_EVENT; //将此通知放到通知栏的"Ongoing"即"正在运行"组中
        // notification.flags |= Notification.FLAG_NO_CLEAR; //表明在点击了通知栏中的"清除通知"后，此通知不清除，
                    //经常与FLAG_ONGOING_EVENT一起使用
        // notification.number = 1; //number字段表示此通知代表的当前事件数量，它将覆盖在状态栏图标的顶部
                    //如果要使用此字段，必须从1开始
        // notification.iconLevel = ; 

        //
        // 添加LED灯提醒
        // notification.defaults |= Notification.DEFAULT_LIGHTS;
        // 或者可以自己的LED提醒模式:
        // notification.ledARGB = 0xff00ff00;
        // notification.ledOnMS = 300; //亮的时间
        // notification.ledOffMS = 1000; //灭的时间
        // notification.flags |= Notification.FLAG_SHOW_LIGHTS;
        //
        
        // notification.contentIntent:一个PendingIntent对象，当用户点击了状态栏上的图标时，该Intent会被触发
        // notification.contentView:我们可以不在状态栏放图标而是放一个view
        // notification.deleteIntent 当前notification被移除时执行的intent
        // notification.vibrate 当手机震动时，震动周期设置

        // 添加声音提示
        //notification.defaults=Notification.DEFAULT_SOUND;
        // audioStreamType的值必须AudioManager中的值，代表着响铃的模式
        //notification.audioStreamType= android.media.AudioManager.ADJUST_LOWER;
   
        //下边的两个方式可以添加音乐
        //notification.sound = Uri.parse("file:///sdcard/notification/ringer.mp3"); 
        //notification.sound = Uri.withAppendedPath(Audio.Media.INTERNAL_CONTENT_URI, "6");
        Context context = m_instance.getApplicationContext();
        if(null == context)
            Log.e(TAG, "context is null");
        Intent notificationIntent = new Intent(m_instance, RabbitImActivity.class);
        PendingIntent contentIntent = PendingIntent.getActivity(m_instance, 0,notificationIntent,  PendingIntent.FLAG_ONE_SHOT);
        // 点击状态栏的图标出现的提示信息设置
        notification.setLatestEventInfo(context, szTitle, szMessage, contentIntent);		
   
        //用mNotificationManager的notify方法通知用户生成标题栏消息通知
        m_notificationManager.notify(m_nCount, notification);
    }
  //*/       

  /* 当 android sdk >= 16 时使用  
    public static void notify(String szMessage, String szTitle)
    {
        Log.d(TAG, "notify:message:" + szMessage + ":title:" + szTitle);
      if (null == m_notificationManager) {
           m_notificationManager = 
                (NotificationManager)m_instance.getSystemService(
                Context.NOTIFICATION_SERVICE);

           m_builder = new Notification.Builder(m_instance);
           if(null == m_builder)
               Log.e(TAG, "m_builder is null");
           m_builder.setSmallIcon(R.drawable.icon);
           //Bitmap bitmap = BitmapFactory.decodeResource(this.getResources(), R.drawable.icon);
           // 设置通知栏中的通知下拉后显示的图标，图片格式为bitmap型  
           //m_builder.setLargeIcon(bitmap); 
           // 设置时间发生时间  
           m_builder.setWhen(System.currentTimeMillis()); 
           // 当通知被用户点击时，通知将被清除（好象要设置intent后才会有效）   
           m_builder.setAutoCancel(true);
           //设置高优先级  
           //m_builder.setPriority(Notification.PRIORITY_HIGH);
           // 实例化Intent  
           Intent intent = new Intent(m_instance, QtActivity.class);
           // 获取PendingIntent  
           PendingIntent pi = PendingIntent.getActivity(m_instance, 0, intent, 0);
           m_builder.setContentIntent(pi); // 设置点击通知将要启动的Inent  
       }

       if(null == m_instance)
          Log.e(TAG, "m_instance is null");
       m_builder.setTicker(szTitle + " " + szMessage); // 在状态栏上提示的文字  
       m_builder.setContentTitle(szTitle); // 在下拉栏中提示的标题  
       m_builder.setContentText(szMessage); // 在下拉栏中提示的文字  
       // m_nCount是下拉栏提示ID，每个ID对应一个下拉提示栏  
       if(null != m_notificationManager)
           m_notificationManager.notify(m_nCount, m_builder.build());
       else
           Log.e(TAG, "m_notificationManager is null");
   }
//*/
   public static void CancelAll()
   {
       if(null != m_notificationManager)
           m_notificationManager.cancelAll();
   }
}
