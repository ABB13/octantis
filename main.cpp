#include "general_Functions.h"

#define USE_SERIAL Serial

void setup() 
{
  USE_SERIAL.begin(115200);
  if (!EEPROM.begin(EEPROAM_SIZE)) 
  {
    USE_SERIAL.println("Failed to initialise EEPROM");
    USE_SERIAL.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  nexInit();
  // "connections" Page
  c0.attachPush(c0Button);
  c1.attachPush(c1Button);
  sx.attachPush(sxButton);
  dx.attachPush(dxButton);
  load_conn.attachPush(load_connButton);
  refresh.attachPush(refreshButton);
  back_conn.attachPush(back_connButton);
  // "keypad" Page
  enter.attachPush(enterButton);
  // "check_conn" Page
  save_pref.attachPush(save_prefButton);
  nextb.attachPush(nextbButton);
  // "full_value" Page
  full_setup.attachPush(setupPushCallback);
  full_env.attachPush(environmentPushCallback);
  full_nav.attachPush(navigationPushCallback);
  full_perf.attachPush(performancePushCallback);
  full_status.attachPush(general_statusPushCallback);
  // "gauge1" Page
  g1_setup.attachPush(setupPushCallback);
  g1_env.attachPush(environmentPushCallback);
  g1_nav.attachPush(navigationPushCallback);
  g1_perf.attachPush(performancePushCallback);
  g1_status.attachPush(general_statusPushCallback);
  // "gauge2" Page
  g2_setup.attachPush(setupPushCallback);
  g2_env.attachPush(environmentPushCallback);
  g2_nav.attachPush(navigationPushCallback);
  g2_perf.attachPush(performancePushCallback);
  g2_status.attachPush(general_statusPushCallback);
  // "double_val" Page
  double_setup.attachPush(setupPushCallback);
  double_env.attachPush(environmentPushCallback);
  double_nav.attachPush(navigationPushCallback);
  double_perf.attachPush(performancePushCallback);
  double_status.attachPush(general_statusPushCallback);

  USE_SERIAL.println("Initialized...");
  // Load the lastpage and the counter page
  if(first_load())
  {
    delay(2000);
    automatic_connection();
  }
  else
  {
    error_status.setText("Load fault!");
    delay(2000);
    // If it doesn't have saved page, this is the first start
    connections.show();
    scanningWiFi();
  }
  
}

void loop() 
{
  nexLoop(nex_listen_list);
  if (WiFi.status() == WL_CONNECTED)
  {
    webSocket.loop();
  }
}