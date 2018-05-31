/*
* android平台消息通知java实现  
*/

package org.KangLinStudio.RabbitIm;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.app.PendingIntent;
import android.util.Log;
import android.os.Bundle;
import android.os.PowerManager;
import android.view.WindowManager;
//import android.graphics.Bitmap;
//import android.graphics.BitmapFactory;
import org.qtproject.qt5.android.bindings.QtActivity;

public class RabbitImActivity
    extends org.qtproject.qt5.android.bindings.QtActivity
{
    private static final String TAG = "RabbitImActivity";
    private static NotificationManager m_notificationManager = null;
    private static final int m_nNotificationCount = 0; //R.drawable.icon;
    private static Notification.Builder m_builder = null;
    private static RabbitImActivity m_instance = null;
    private PowerManager.WakeLock m_WakeLock = null;
    private String m_szKey;
    /** 通知栏点击事件对应的ACTION */
    private final static String ACTION_ON_CHICK = "org.KangLinStudio.RabbitIm.RabbitImActivity.OnClick";
    private OnChickBroadcastReceiver m_Receiver = null;
    
    public static native void OnChickNotification(String key);

    public RabbitImActivity()
    {
        Log.d(TAG, "NotificationClient");
        m_instance = this;
    }
        
    @Override
    public void onCreate(Bundle savedInstanceState) {                    
        super.onCreate(savedInstanceState);
        initReceiver();
    }

    @Override
    protected void onDestroy() {
        if (null != m_Receiver)
            unregisterReceiver(m_Receiver);
        super.onDestroy();
    }

/* 当 android sdk < 16 时使用 
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
        Intent notificationIntent = new Intent(ACTION_ON_CHICK);
        //传递参数，标志此时是通知栏点击激活窗口  
        notificationIntent.putExtra("onChick", "true"); 
        //PendingIntent的位标识符：
        //FLAG_ONE_SHOT   表示返回的PendingIntent仅能执行一次，执行完后自动取消
        //FLAG_NO_CREATE     表示如果描述的PendingIntent不存在，并不创建相应的PendingIntent，而是返回NULL
        //FLAG_CANCEL_CURRENT      表示相应的PendingIntent已经存在，则取消前者，然后创建新的PendingIntent，这个有利于数据保持为最新的，可以用于即时通信的通信场景
        //FLAG_UPDATE_CURRENT     表示更新的PendingIntent
        PendingIntent contentIntent = PendingIntent.getBroadcast(m_instance, 0,
                             notificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);
        // 点击状态栏的图标出现的提示信息设置
        notification.setLatestEventInfo(context, szTitle, szMessage, contentIntent);		
        //用mNotificationManager的notify方法通知用户生成标题栏消息通知
        m_notificationManager.notify(m_nNotificationCount, notification);
    } //*/       

  //* 当 android sdk >= 16 时使用  
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
           Intent notificationIntent = new Intent(ACTION_ON_CHICK);
           //传递参数，标志此时是通知栏点击激活窗口  
           notificationIntent.putExtra("onChick", "true");           
           // 获取PendingIntent  
           PendingIntent pi = PendingIntent.getActivity(m_instance, 0, notificationIntent, 0);
           m_builder.setContentIntent(pi); // 设置点击通知将要启动的Inent  
       }

       if(null == m_instance)
          Log.e(TAG, "m_instance is null");
       m_builder.setTicker(szTitle + " " + szMessage); // 在状态栏上提示的文字  
       m_builder.setContentTitle(szTitle); // 在下拉栏中提示的标题  
       m_builder.setContentText(szMessage); // 在下拉栏中提示的文字  
       // m_nCount是下拉栏提示ID，每个ID对应一个下拉提示栏  
       if(null != m_notificationManager)
           m_notificationManager.notify(m_nNotificationCount, m_builder.build());
       else
           Log.e(TAG, "m_notificationManager is null");
   } //*/

   public static void CancelAll()
   {
       if(null != m_notificationManager)
           m_notificationManager.cancelAll();
   }
   
   /** 带的通知栏点击广播接收 */
   private void initReceiver(){
       m_Receiver = new OnChickBroadcastReceiver();
       IntentFilter intentFilter = new IntentFilter();
       intentFilter.addAction(ACTION_ON_CHICK);
       registerReceiver(m_Receiver, intentFilter);
   }
   
   /** 通知栏点击事件接收器 */
   private class OnChickBroadcastReceiver extends BroadcastReceiver{
       
       @Override
       public void onReceive(Context context, Intent intent) {
           String action = intent.getAction();
           if(!action.equals(ACTION_ON_CHICK)) return;
           //显示窗口  
           Intent it1 = new Intent(context, RabbitImActivity.class);
           it1.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED); //两种都可以
           //it1.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_SINGLE_TOP);
           context.getApplicationContext().startActivity(it1);
           OnChickNotification(m_szKey);
       }       
   }

   /** 禁用锁屏功能
    *  @para bWake:true,禁用锁屏，false允许锁屏
    *  @return boolean 成功返回 true，否则返回 false
    */
   public static boolean EnableWake(boolean bWake){
       if(null == m_instance)
           return false;
       if(null == m_instance.m_WakeLock) {
           PowerManager pManager = ((PowerManager) m_instance.getSystemService(POWER_SERVICE));
           m_instance.m_WakeLock = pManager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK
                   | PowerManager.ON_AFTER_RELEASE, TAG);
       }
       if(null == m_instance.m_WakeLock) return false;
       if(bWake)
           m_instance.m_WakeLock.acquire();
       else
       {   
           m_instance.m_WakeLock.release();
           m_instance.m_WakeLock = null;
       }
       return true;
   }
       
}
