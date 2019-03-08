#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H
/*************************************************************************************************************************
  REQUIRED LIBRARIES
*************************************************************************************************************************/
    #include <Arduino.h>
    #include <WiFi.h>
    #include "..\lib\arduinoWebSockets-master\src\WebSocketsClient.h"   //WebSocket Library 
    #include <Nextion.h>            //Mod Nextion Library for SoftwareSerial https://github.com/HobbytronicsPK/Nextion 
    #include <ArduinoJson.h>        // Arduino Json Library
    #include <vector>             // vector library 
    #include <cmath>
    #include <EEPROM.h>          // Library EEPROM to save in flash memory
    #include "general_Functions.h"
    
    using namespace std;
/*************************************************************************************************************************
  USEFUL #DEFINE
*************************************************************************************************************************/
    #define USE_SERIAL Serial
    #define EEPROAM_SIZE 64
    #define MAX_SIZEOF_PASS 20
    #define MAX_SIZEOF_SSID 20
    #define MAX_SIZEOF_SERVER 20
/*************************************************************************************************************************
  GLOBAL VARIABLES
*************************************************************************************************************************/
    // For connection
    WebSocketsClient webSocket; 
    vector<String> wifiList;        // Vector that contain networks 
    char selected_SSID[20] = {0};
    char pass[20] = {0};
    char first_SSID[120] = {0};
    char second_SSID[120] = {0};
    char* st_ssid;
    char* st_pass;
    bool isAutomaticConnection = false;
    bool serverURLisSubmitted = false;
    //String url = "demo.signalk.org";
    String url;
    // General
    int x = 0;                               // Index for list who contains networks
    bool check_connectionwebsocket = false; // a boolean for check if websocket is connected
    String current_status;
    // Address for save data
    int address_pass = 0;                   // Starting address for the password of wifi  
    int address_ssid = 22;                  // Starting address for the ssid of wifi
    int ADDRESS_LASTPAGE = 60;
    int ADDRESS_COUNTERPAGE = 50;
    int ADDRESS_SERVER = 4 ;
    // Subscribe message
    // Environment
        const char* waterdepth ="{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"environment.depth.belowTransducer\"}]}";
        const char* temperature = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"environment.water.temperature\"}]}";
        const char* angle_windTrue = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"environment.wind.angleTrueWater\"}]}";
        const char* angle_windApp = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"environment.wind.angleApparent\"}]}";
        const char* speed_windTrue= "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"environment.wind.speedTrue\"}]}";
        const char* speed_windApp = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"environment.wind.speedApparent\"}]}";
    // Navigation 
        const char* current_speed = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.speedThroughWater\"}]}";
        const char* triplog = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.trip.log\"}]}"; 
        const char* totallog = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.log\"}]}";
        const char* magnetic_heading = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.headingMagnetic\"}]}";
        const char* true_heading = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.headingTrue\"}]}";
        const char* speed_over_g = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.speedOverGround\"}]}";
        const char* course_over_g_magn = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.courseOverGroundMagnetic\"}]}";
        const char* course_over_g_true = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.courseOverGroundTrue\"}]}";
        const char* cross_track_erro = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.crossTrackError\"}]}";
        const char* latitude_longitude = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.position\",\"period\":120000,\"policy\":\"fixed\"}]}";
        const char* estimated_time_way_point = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"navigation.destination.eta\"}]}";
    // Performance 
        const char* tacktrue = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"performance.tackTrue\"}]}";
        const char* tackmagnetic = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"performance.tackMagnetic\"}]}";
        const char* velocity_made_good_waypoint = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"performance.velocityMadeGoodToWaypoint\"}]}";
        const char* velocity_made_good = "{\"context\":\"vessels.self\",\"subscribe\":[{\"path\":\"performance.velocityMadeGood\"}]}";;
    // Manage the page
    vector<bool> whoclicked(3, false);      // the first element is "Environment", second is "Navigation" and the third element is "Performance"
    int lastpage = 0;                       // The last saved page, zero is the "null" element. 1,2,3 are for "Environment", "Navigation", "Performance" 
    int counterPageEnv = 0;
    int counterPageNav = 0;
    int counterPagePer = 0; 
    int savedcounter; 
/*************************************************************************************************************************
  ALL FUNCTION DECLARATION
*************************************************************************************************************************/
    // GENERAL
        void scanningWiFi();
        void connectWiFi(char ssid[],char pwd[]);
        bool showLoadPreference();
        void environment(int page);
        void navigation(int page);
        void performance(int page);
        bool subscribeTo(const char* query);
        bool unsubscribe();
        void rappresentationAngWind(float val);
        bool first_load();
        void page_loader();
        void automatic_connection();
    // WEBSOCKETEVENTS
        void webSocketConnection(WStype_t type, uint8_t * payload, size_t length);
        void webSocketUnsubscribe(WStype_t type, uint8_t * payload, size_t length);
        void webSocketFullvaluePage(WStype_t type, uint8_t * payload, size_t length);
        void webSocketGauge1Page(WStype_t type, uint8_t * payload, size_t length);
        void webSocketGauge2Page(WStype_t type, uint8_t * payload, size_t length);
        void webSocketDoubleValue(WStype_t type, uint8_t * payload, size_t length);
        void webSocketAutomaticConnection(WStype_t type, uint8_t * payload, size_t length);
/*************************************************************************************************************************
  NEXTION STUFF
*************************************************************************************************************************/
// ALL OBJECTS OF NEXTION "Nextion object=(idpage, idobject, nameobject)"
    /* 
    * PAGE "splashscreen"
    */
        NexText error_status = NexText(0, 1, "error_status");
    /* 
    * PAGE "connections"
    */
        NexPage connections = NexPage(7, 0, "connections");
        NexText c0 = NexText(7, 1, "c0");
        NexText c1 = NexText(7, 2, "c1");
        NexButton back_conn = NexButton(7, 3, "back_conn");
        NexButton sx = NexButton(7, 5, "sx");
        NexButton dx = NexButton(7, 6, "dx");
        NexButton load_conn = NexButton(7, 4, "load_conn");
        NexButton refresh = NexButton(7, 7, "refresh");
    /* 
    * PAGE "keypad"
    */
        NexText pass_txt = NexText(1, 1, "pass_txt");
        NexButton enter = NexButton(1, 34, "enter");
    /* 
    * PAGE "keypad server"
    */
        NexPage keypad_serv = NexPage (8, 0, "serverkp");
        NexText serv_txt = NexText(8, 1, "serv_txt");
        NexButton serv_enter = NexButton(8, 34, "serv_enter");

    /* 
    * PAGE "check_conn"
    */
        NexPage check_conn = NexPage(2, 0, "check_conn");
        NexText checking_txt = NexText(2, 1, "checking_txt");
        NexText save_status = NexText(2, 5, "save_status");
        NexButton save_pref = NexButton(2, 3, "save_pref");
        NexButton nextb = NexButton(2, 2, "next");
        NexButton back = NexButton(2, 4, "back");
    /* 
    * PAGE "full_value"
    */
        NexPage full_value = NexPage(2, 0, "full_value");
        NexText title_full = NexText(3, 5, "title");
        NexText value = NexText(3, 6, "value");
        NexPicture unit = NexPicture(3, 7, "unit");
        NexButton full_setup = NexButton(3, 1, "setup");
        NexButton full_env = NexButton(3, 2, "env");
        NexButton full_nav = NexButton(3, 3, "nav");
        NexButton full_perf = NexButton(3, 4, "perf");
        NexButton full_status = NexButton(3, 8, "general_status");
    /* 
    * PAGE "gauge1"
    */
        NexPage gauge1 = NexPage(4, 0, "gauge1");
        NexText g1_title = NexText(4, 1, "title");
        NexText g1_value = NexText(4, 6, "g1_value");
        NexGauge g1_gauge = NexGauge(4, 7, "g1_gauge");
        NexButton g1_setup = NexButton(4, 2, "setup");
        NexButton g1_env = NexButton(4, 3, "env");
        NexButton g1_nav = NexButton(4, 4, "nav");
        NexButton g1_perf = NexButton(4, 5, "perf");
        NexButton g1_status = NexButton (4, 8, "general_status");
    /* 
    * PAGE "gauge2"
    */
        NexPage gauge2 = NexPage(5, 0, "gauge2");
        NexText g2_title = NexText(5, 1, "title");
        NexText g2_value = NexText(5, 6, "g2_value");
        NexGauge g2_gauge = NexGauge(5, 7, "g2_gauge");
        NexButton g2_setup = NexButton(5, 2, "setup");
        NexButton g2_env = NexButton(5, 3, "env");
        NexButton g2_nav = NexButton(5, 4, "nav");
        NexButton g2_perf = NexButton(5, 5, "perf");
        NexButton g2_status = NexButton(5, 8, "general_status");
    /* 
    * PAGE "double_val"
    */
        NexPage double_val = NexPage(6, 0, "double_val");
        NexText val1 = NexText(6, 3, "val1");
        NexText val2 = NexText(6, 4, "val2");
        NexText d1 = NexText(6, 10, "d1");
        NexText d2 = NexText(6, 9, "d2");
        NexText title1 = NexText(6, 1, "title1");
        NexText title2 = NexText(6, 2, "title2");
        NexButton double_setup = NexButton(6, 6, "setup");
        NexButton double_env = NexButton(6, 5, "env");
        NexButton double_nav = NexButton(6, 7, "nav");
        NexButton double_perf = NexButton(6, 8, "perf");
        NexButton double_status = NexButton(6, 9, "general_status");
// NEXTION FUNCTIONS
    /*
    * BASE FUNCTIONS
    */
        // SETUP FUNCTION
            void setupPushCallback(void *ptr)
            {
                if(unsubscribe)
                {
                    scanningWiFi();
                }
                else 
                {
                    USE_SERIAL.println("Failed to unsubscribe");
                }
            }
        // ENVIRONMENT FUNCTION
            void environmentPushCallback(void *ptr)
            {
                if (unsubscribe())
                {
                    lastpage=1;
                    USE_SERIAL.println(EEPROM.writeUInt(ADDRESS_LASTPAGE,lastpage));
                    EEPROM.commit();
                    //USE_SERIAL.println(EEPROM.readUInt(ADDRESS_LASTPAGE));
                    // check if we had here
                    if (whoclicked[0] == false)
                    {
                        // Set env on true
                        whoclicked[0] = true;
                        // Set "per" and "nav" on false
                        whoclicked[1] = false;
                        whoclicked[2] = false;
                        environment(counterPageEnv);
                    }
                    else
                    {
                        counterPageEnv++;
                        EEPROM.writeUInt(ADDRESS_COUNTERPAGE,counterPageEnv);
                        EEPROM.commit();
                        USE_SERIAL.println(EEPROM.readUInt(ADDRESS_COUNTERPAGE));
                        environment(counterPageEnv);
                    }
                }
                else 
                {
                    USE_SERIAL.println("Failed to unsubscribe");
                }
                
            }
        // NAVIGATION FUNCTION
            void navigationPushCallback(void *ptr)
            {
                lastpage=2;
                USE_SERIAL.println(lastpage);
                EEPROM.writeUInt(ADDRESS_LASTPAGE,lastpage);
                USE_SERIAL.println("Prima del commit:");
                USE_SERIAL.println(EEPROM.readUInt(ADDRESS_LASTPAGE));
                EEPROM.commit();
                USE_SERIAL.println("dopo:");
                USE_SERIAL.println(EEPROM.readUInt(ADDRESS_LASTPAGE));
                if (unsubscribe())
                {
                    // check if we had here
                    if (whoclicked[1] == false)
                    {
                        // Set nav on true
                        whoclicked[1] = true;
                        // Set "per" and "nav" on false
                        whoclicked[0] = false;
                        whoclicked[2] = false;
                        navigation(counterPageNav);
                    }
                    else
                    {
                        counterPageNav++;
                        EEPROM.writeUInt(ADDRESS_COUNTERPAGE,counterPageNav);
                        EEPROM.commit();
                        USE_SERIAL.println(EEPROM.readUInt(ADDRESS_COUNTERPAGE));
                        navigation(counterPageNav);
                    }
                }
                else 
                {
                    USE_SERIAL.println("Failed to unsubscribe");
                }
            }
        // PERFORMANCE FUNCTION
            void performancePushCallback(void *ptr)
            {
                lastpage=3;
                EEPROM.writeUInt(ADDRESS_LASTPAGE,lastpage);
                EEPROM.commit();
                if (unsubscribe())
                {
                    // check if we had here
                    if (whoclicked[3] == false)
                    {
                        // Set nav on true
                        whoclicked[3] = true;
                        // Set "per" and "nav" on false
                        whoclicked[1] = false;
                        whoclicked[2] = false;
                        performance(counterPagePer);
                    }
                    else
                    {
                        counterPagePer++;
                        EEPROM.writeUInt(ADDRESS_COUNTERPAGE,counterPagePer);
                        EEPROM.commit();
                        USE_SERIAL.println(EEPROM.readUInt(ADDRESS_COUNTERPAGE));
                        performance(counterPagePer);
                    }
                }
                else 
                {
                    USE_SERIAL.println("Failed to unsubscribe");
                }
            }
        // STATUS FUNCTION
            void general_statusPushCallback(void *ptr)
            {
                int orange = 64512;
                int green = 9859;
                int red = 59392;

                if (WiFi.status() == WL_CONNECTED)
                {
                    char buf[10] = {0};
                    String cmd;
                    
                    utoa(orange, buf, 10);
                    cmd += "general_status";
                    cmd += ".bco=";
                    cmd += buf;
                    sendCommand(cmd.c_str());
                    
                    cmd="";
                    cmd += "ref ";
                    cmd += "general_status";
                    sendCommand(cmd.c_str());
                }
                else 
                {
                    char buf[10] = {0};
                    String cmd;
                    
                    utoa(red, buf, 10);
                    cmd += "general_status";
                    cmd += ".bco=";
                    cmd += buf;
                    sendCommand(cmd.c_str());
                    
                    cmd="";
                    cmd += "ref ";
                    cmd += "general_status";
                    sendCommand(cmd.c_str());
                }
                if (check_connectionwebsocket == true)
                {
                    char buf[10] = {0};
                    String cmd;
                    
                    utoa(green, buf, 10);
                    cmd += "general_status";
                    cmd += ".bco=";
                    cmd += buf;
                    sendCommand(cmd.c_str());
                    
                    cmd="";
                    cmd += "ref ";
                    cmd += "general_status";
                    sendCommand(cmd.c_str());
                }
            }
    /*
    * OTHER FUNCTIONS
    */
    /* PAGE "connections" */////////////////////////////////////////
        // FUNCTION FOR CONNECT AT "c0" SSID
            void c0Button(void *ptr)
            {
                strcpy(selected_SSID,first_SSID);
            }
        // FUNCTION FOR CONNECT AT "c1" SSID
            void c1Button(void *ptr)
            {
                strcpy(selected_SSID,second_SSID);
            }
        // FUNCTION TO GO BACK FROM PAGE "connections"
            void back_connButton(void *ptr)
            {
                USE_SERIAL.print("lastpage in back: ");
                USE_SERIAL.println(lastpage);
                // Can use this button only if is not the first setup
                if(lastpage!=0)
                {
                    switch (lastpage)
                    {
                        case 1: 
                        {
                            USE_SERIAL.println("env");
                            environment(counterPageEnv);
                        }
                        break;
                        case 2: 
                        {
                            USE_SERIAL.println("nav");
                            navigation(counterPageNav);
                        }
                        break;
                        case 3: 
                        {
                            USE_SERIAL.println("per");
                            performance(counterPagePer);
                        }
                        break;
                    }
                }
                
            }
        // FUNCTION TO SCROLL THE LIST OF WIFI (Left)
            void sxButton(void *ptr)
            {
                USE_SERIAL.println("bb button");
                int max = wifiList.size();
                x = abs(x-2);
                USE_SERIAL.print("risultato x: ");
                USE_SERIAL.println(x);
                memset(first_SSID, 0, sizeof(first_SSID));
                strncpy(first_SSID, wifiList[x%max].c_str(), sizeof(first_SSID));
                USE_SERIAL.print("first_SSID: ");
                USE_SERIAL.println(first_SSID);
                c0.setText(first_SSID);

                memset(second_SSID, 0, sizeof(second_SSID));
                strncpy(second_SSID, wifiList[abs((x-1))%max].c_str(), sizeof(second_SSID));
                USE_SERIAL.print("second_SSID: ");
                USE_SERIAL.println(second_SSID);
                c1.setText(second_SSID);
            }
        //FUNCTION TO SCROLL THE LIST OF WIFI (Right)
            void dxButton(void *ptr)
            {
                USE_SERIAL.println("bn button");
                int max = wifiList.size();
                x = x+2;
                USE_SERIAL.print("risultato x: ");
                USE_SERIAL.println(x);
                memset(first_SSID, 0, sizeof(first_SSID));
                strncpy(first_SSID, wifiList[x%max].c_str(), sizeof(first_SSID));
                USE_SERIAL.print("first_SSID: ");
                USE_SERIAL.println(first_SSID);
                c0.setText(first_SSID);

                memset(second_SSID, 0, sizeof(second_SSID));
                strncpy(second_SSID, wifiList[(x+1)%max].c_str(), sizeof(second_SSID));
                USE_SERIAL.print("second_SSID: ");
                USE_SERIAL.println(second_SSID);
                c1.setText(second_SSID);
            }
        // FUNCTION TO LOAD CONNECTION PREFERENCE
            void load_connButton(void *ptr)
            {
                c0.setText("Loading...");
                c1.setText("");
                String stored_ssid = EEPROM.readString(address_ssid);
                vector<String>::iterator it = wifiList.begin();
                while (it != wifiList.end())
                {
                    // check if the ssid storage are in wifilist
                    if (stored_ssid==*it)
                    {
                        USE_SERIAL.println("trovato!");
                        c0.setText("Loaded! wait a moment");
                        c1.setText("");
                        USE_SERIAL.println(*it + " " + stored_ssid);
                        String stored_pass = EEPROM.readString(address_pass);
                        url = EEPROM.readString(ADDRESS_SERVER);
                        st_ssid = strdup(stored_ssid.c_str());
                        st_pass = strdup(stored_pass.c_str());
                        USE_SERIAL.printf("Eccoli: %s %s \n", st_ssid, st_pass);
                        //check_conn.show();
                        //connectWiFi(st_ssid, st_pass);
                        isAutomaticConnection=true;
                        keypad_serv.show();
                        break;
                    }
                    USE_SERIAL.println(*it);
                    it++;
                }
                c0.setText("failed, click on refresh");
                c1.setText("");
            }
        // FUNCTION TO REFRESH THE CONNECTIONS
            void refreshButton(void *ptr)
            {
                scanningWiFi();
            }
    /* PAGE "keypad" */////////////////////////////////////////
        // THE FUNCTION THAT CONNECT TO WIFI WHEN PRESS THE ENTER BUTTON
            void enterButton(void *ptr)
            {
                memset(pass, 0 ,sizeof(pass));
                isAutomaticConnection=false;
                pass_txt.getText(pass,sizeof(pass));
                USE_SERIAL.print("Il SSID è: ");
                USE_SERIAL.println(selected_SSID);
                USE_SERIAL.print("La pass inserita è: ");
                USE_SERIAL.println(pass);
            }
    /* PAGE "keypad server" */////////////////////////////////////////
        // THE FUNCTION FOR ADD THE SERVER URL
    void enterServerButton(void *ptr)
            {
                char insert_url[30]; 
                serv_txt.getText(insert_url,sizeof(insert_url));
                url = insert_url;
                USE_SERIAL.print("il server inserito è: ");
                USE_SERIAL.println(url);                
                if (isAutomaticConnection==false)
                {   
                    USE_SERIAL.println("automaticConnection = false ");
                    connectWiFi(selected_SSID,pass);
                }
                else
                {
                    serverURLisSubmitted = true;
                    checking_txt.setText("Connecting to Wsc...");
                    // Connect to the websocket
                    webSocket.begin(url, 80, "/signalk/v1/stream?subscribe=none");
                    webSocket.onEvent(webSocketAutomaticConnection);
                }
                   
            }
    /* PAGE "check_conn" */////////////////////////////////////////
        // FUNCTION FOR SAVE THE WIFI PREFERENCE
            void save_prefButton(void *ptr)
            {
                if (strlen(pass) <= MAX_SIZEOF_PASS && strlen(selected_SSID) <= MAX_SIZEOF_SSID && url.length() <= MAX_SIZEOF_SERVER )
                {
                    EEPROM.writeString(address_pass, pass);
                    EEPROM.writeString(address_ssid, selected_SSID);
                    //const char *urlTosave = url.c_str();
                    USE_SERIAL.println(EEPROM.readString(address_pass));
                    USE_SERIAL.println(EEPROM.readString(address_ssid));
                    USE_SERIAL.println("Saved!");
                    save_status.setText("Saved!");
                    EEPROM.commit();
                }
                else 
                {
                    USE_SERIAL.println("Error! Can't save!");
                    save_status.setText("Error! Can't save!");
                }
            }
        // FUNCTION OF NEXT BUTTON
            void nextbButton(void *ptr)
            {
                if(lastpage!=0)
                {
                    switch (lastpage)
                    {
                        case 1: 
                        {
                            whoclicked[0]=true;
                            environment(counterPageEnv);
                        }
                        break;
                        case 2: 
                        {
                            whoclicked[1]=true;
                            navigation(counterPageNav);
                        }
                        break;
                        case 3: 
                        {
                            whoclicked[2]=true;
                            performance(counterPagePer);
                        }
                        break;
                    }
                }
                else
                {
                    full_value.show();
                }
            }
    
    // **** NEXTION OBJECTS LIST **** 
    NexTouch *nex_listen_list[] =
    {
        // "connections" page
        &c0,
        &c1,
        &sx,
        &dx,
        &load_conn,
        &refresh,
        &back_conn,
        // "keypad" page
        &enter,
        //"keypad server" page
        &serv_enter,
        // "check_conn" page
        &nextb,
        &save_pref,
        // "full_value" page
        &full_setup,
        &full_env,
        &full_nav,
        &full_perf,
        &full_status,
        // "gauge1" page
        &g1_setup,
        &g1_env,
        &g1_nav,
        &g1_perf,
        &g1_status,
        // "gauge2" page
        &g2_setup,
        &g2_env,
        &g2_nav,
        &g2_perf,
        &g2_status,
        // "double_value" page
        &double_setup,
        &double_env,
        &double_nav,
        &double_perf,
        &double_status,
        // end
        NULL
    };
/*************************************************************************************************************************
  GENERAL FUNCTIONS
*************************************************************************************************************************/
    // FUNCTION TO SCAN THE WIFI 
    void scanningWiFi()
    {
        WiFi.mode(WIFI_STA);
        
        USE_SERIAL.println("Scanning...");
        c0.setText("Scanning...");
        c1.setText(" ");
        // Scanning networks, WiFi.scanNetworks will return the number of networks found
        int n = WiFi.scanNetworks();
        if (n==0)
        {
            USE_SERIAL.println("No networks found");
            c0.setText("No networks found");
        }
        // If there is some networks It will save in a list
        else 
        {
            for (int i=0 ; i < n ; ++i)
            {
                wifiList.push_back(WiFi.SSID(i));
            }
        }

        // That show all networks found 
        USE_SERIAL.println("WIFI TROVATI");
        USE_SERIAL.println();
        for (int i=0 ;i<wifiList.size(); i++)
        {
            USE_SERIAL.print(i);
            USE_SERIAL.print(")");
            USE_SERIAL.print(wifiList[i]);
            USE_SERIAL.println();
        }

        // Show on display the first 2 networks found
        // 1 
        int mod =x%n;
        strncpy(first_SSID, wifiList[mod].c_str(), sizeof(first_SSID));
        c0.setText(first_SSID);
        // 2 
        mod = (x+1)%n;
        strncpy(second_SSID, wifiList[mod].c_str(), sizeof(second_SSID));
        c1.setText(second_SSID);
    }
    // FUNCTION FOR CONNECT TO WIFI
    void connectWiFi(char ssid[],char pwd[])
    {
        WiFi.begin(ssid,pwd);

        int cont=0;
        while(WiFi.status() != WL_CONNECTED && WiFi.status() != WL_CONNECT_FAILED)
        {
            checking_txt.setText("Connecting to WiFi...");
            delay(1000);
            cont ++;
            if (cont > 10)
            {
            // After 10 second show the result of timeout 
            checking_txt.setText("Timeout for connect to wifi");
            // exit from while
            break;
            }
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            serverURLisSubmitted = true;
            checking_txt.setText("Connected to the WiFi");
            current_status="Connected to the WiFi";
            delay(2000);
            checking_txt.setText("Connecting to the server..");
            // Connect to the websocket
            webSocket.begin(url, 80, "/signalk/v1/stream?subscribe=none");
            webSocket.onEvent(webSocketConnection);
        }
        else 
        {
            delay(3000);
            checking_txt.setText("Try again");
        }
    }
    // FUNCTION TO MANAGE RIGHT PAGE AND THE RIGHT VALUE OF ENVIRONMENT
    void environment(int page)
    {
        // Check if the value go over the range of the switch case
        if (page > 5)
        {
            page = 0;
            counterPageEnv=0;
        }
        USE_SERIAL.print("page: ");
        USE_SERIAL.println(page);
        switch (page)
        {
            case 0: // Water Depth
            {
                USE_SERIAL.println("WD");
                full_value.show();
                value.setText("---");
                title_full.setText("Water Depth");
                unit.setPic(6);
                if (subscribeTo(waterdepth))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 1: // Sea Temperature
            {
                USE_SERIAL.println("ST");
                full_value.show();
                value.setText("---");
                title_full.setText("Sea Temperature");
                unit.setPic(5);
                if (subscribeTo(temperature))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 2: // true wind angle 
            {
                USE_SERIAL.println("Angletrue");
                gauge2.show();
                g2_title.setText("True Wind Angle");
                if (subscribeTo(angle_windTrue))
                {
                    webSocket.onEvent(webSocketGauge2Page);
                }
                else 
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 3: // apparent wind angle
            {
                USE_SERIAL.println("angleapparent");
                gauge2.show();
                g2_title.setText("Apparent Wind Angle");
                if (subscribeTo(angle_windApp))
                {
                    webSocket.onEvent(webSocketGauge2Page);
                }
                else 
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 4: // true wind speed
            {
                USE_SERIAL.println("windspeed");
                full_value.show();
                value.setText("---");
                title_full.setText("True Wind Speed");
                unit.setPic(7);
                if (subscribeTo(speed_windTrue))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else 
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 5: // apparent wind speed
            {
                USE_SERIAL.println("apparentwindspeed");
                full_value.show();
                value.setText("---");
                title_full.setText("Apparent Wind Speed");
                unit.setPic(7);
                if (subscribeTo(speed_windApp))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else 
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
        }
    }
    // FUNCTION TO MANAGE THE RIGHT PAGE AND THE RIGHT VALUE OF NAVIGATION
    void navigation(int page)
    {
        if (page > 12)
        {
            page = 0;
            counterPageNav = 0;
        }
        switch (page)
        {
            case 0: // current speed
            {
                full_value.show();
                title_full.setText("Current Boat Speed");
                value.setText("---");
                unit.setPic(7);
                if (subscribeTo(current_speed))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");                    }
                }
            break;
            case 1: // average speed
            {
                full_value.show();
                title_full.setText("Average Speed");
                value.setText("---");
                /*unit.setPic(7);
                if (subscribeTo(waterdepth))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }*/
            }
            break;
            case 2: // maximum speed
            {
                full_value.show();
                title_full.setText("Maximum Speed");
                value.setText("---");

            }
            break;
            case 3: // trip log
            {
                full_value.show();
                title_full.setText("Trip Log");
                value.setText("---");
                unit.setPic(6);
                if (subscribeTo(triplog))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 4: // total log
            {
                full_value.show();
                title_full.setText("Total Log");
                value.setText("---");
                unit.setPic(6);
                if (subscribeTo(totallog))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 5: // magnetic heading
            {
                gauge1.show();
                g1_title.setText("Magnetic Heading");
                if (subscribeTo(magnetic_heading))
                {
                    webSocket.onEvent(webSocketGauge1Page);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 6: // true heading
            {
                gauge1.show();
                g1_title.setText("True Heading");
                if (subscribeTo(true_heading))
                {
                     webSocket.onEvent(webSocketGauge1Page);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                } 
            }
            break;
            case 7: // speed over ground 
            {
                full_value.show();
                title_full.setText("Speed Over Ground");
                value.setText("---");
                unit.setPic(7);
                if (subscribeTo(speed_over_g))
                {
                     webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 8: // course over ground magnetic
            {
                gauge1.show();
                g1_title.setText("Course Over Ground Magnetic");
                if (subscribeTo(course_over_g_magn))
                {
                    webSocket.onEvent(webSocketGauge1Page);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 9: // course over ground true 
            {
                gauge1.show();
                g1_title.setText("Course Over Ground True");
                if (subscribeTo(course_over_g_true))
                {
                    webSocket.onEvent(webSocketGauge1Page);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 10: // cross track error
            {
                full_value.show();
                title_full.setText("Cross Track Error");
                value.setText("---");
                unit.setPic(6);
                if (subscribeTo(cross_track_erro))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 11: // latitude and longitude
            {
                double_val.show();
                if (subscribeTo(latitude_longitude))
                {
                    webSocket.onEvent(webSocketDoubleValue);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 12: // estimated time to way point
            {
                full_value.show();
                title_full.setText("Estimated Time To WayPoint");
                value.setText("---");
                //unit.setPic(6); ??????????????????????????????????????????????
                if (subscribeTo(estimated_time_way_point))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
        }
    }
    // FUNCTION TO MANAGE THE RIGHT PAGE AND THE RIGHT VALUE OF PERFORMANCE
    void performance(int page)
    {
        if (page > 3)
        {
            USE_SERIAL.println("entrato nel page mag 2");
            page = 0;
            counterPagePer = 0;
        }

        switch (page)
        {
            case 0: //true tack
            {
                gauge1.show();
                g1_title.setText("True Tack");
                if (subscribeTo(tacktrue))
                {
                    webSocket.onEvent(webSocketGauge1Page);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 1: // magnetic tack
            {
                gauge1.show();
                g1_title.setText("Magnetic Tack");
                if (subscribeTo(tackmagnetic))
                {
                    webSocket.onEvent(webSocketGauge1Page);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 2: // velocity made good to waypoint
            {
                full_value.show();
                title_full.setText("VMG to waypoint");
                value.setText("---");
                unit.setPic(7);
                if (subscribeTo(velocity_made_good_waypoint))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
            break;
            case 3: // velocity made good 
            {
                full_value.show();
                title_full.setText("VMG");
                value.setText("---");
                unit.setPic(7);
                if (subscribeTo(velocity_made_good))
                {
                    webSocket.onEvent(webSocketFullvaluePage);
                }
                else
                {
                    USE_SERIAL.println("ErrorSubscribe");
                }
            }
        }
    }
    // FUNCTION TO SEND A SUBSCRIBE
    bool subscribeTo(const char* query)
    {
       return webSocket.sendTXT(query);
    }
    // FUNCTION TO UNSUBSCRIBE
    bool unsubscribe()
    {
        // query for unsubscribe
        const char* query = "{\"context\":\"*\",\"unsubscribe\":[{\"path\":\"*\"}]}";
        return webSocket.sendTXT(query);
    }
    // FUNCTION TO RAPPRESENTATE THE ANGLE OF WIND ON NEXTION
    void rappresentationAngWind(float val)
    {
        // Converting
        char vwat[4];
      // Convert the radiant to degree
      float Degree = val * 180/PI;
      int result;
      // if negative
      if (Degree < 0)
      {
        //USE_SERIAL.println("Neg:");
        if (abs(Degree) <= 90 && abs(Degree) >= 0)
        {
          //USE_SERIAL.println("1");
          result=Degree+90;
        }
        if (abs(Degree) <= 180 && abs(Degree) >= 90 )
        {
          //USE_SERIAL.println("2");
          result=(360-Degree)+90;
        }
        //USE_SERIAL.println(result);
      }
      // positive 
      else 
      {
        result= Degree+90;
        //USE_SERIAL.println(result);
      }
      dtostrf(Degree, 3, 3, vwat);
      // Send the value and the result of the gauge on nextion
      g2_value.setText(vwat);
      g2_gauge.setValue(result);
    }
    // FUNCTION TO LOAD THE SAVED PAGE
    bool first_load()
    {
        lastpage = EEPROM.readUInt(ADDRESS_LASTPAGE);
        USE_SERIAL.print("lastpage: ");
        USE_SERIAL.println(lastpage);
        savedcounter = EEPROM.readUInt(ADDRESS_COUNTERPAGE);
        USE_SERIAL.print("counterpage: ");
        USE_SERIAL.println(savedcounter);
        // check if the data are right or not 
        if ((lastpage < 4 && lastpage > 0) && (savedcounter >= 0))
        {
            switch (lastpage)
            {
                case 1:
                {
                    USE_SERIAL.println("1");
                    counterPageEnv = savedcounter;
                }
                break;
                case 2:
                {
                    USE_SERIAL.println("2");
                    counterPageNav = savedcounter;
                }
                break;
                case 3:
                {
                    USE_SERIAL.println("3");
                    counterPagePer = savedcounter;
                }
                break;
            }

            USE_SERIAL.println("return");
            return true;
        }
        else 
        {
            USE_SERIAL.println("return fault");
            return false; 
        }
    }
    // FUNCTION TO CHOOSE THE RIGHT PAGE
    void page_loader()
    {
        switch (lastpage)
        {
            case 1: 
            {
                USE_SERIAL.println("env");
                whoclicked[0]=true;
                environment(counterPageEnv);
            }
            break;
            case 2: 
            {
                USE_SERIAL.println("nav");
                whoclicked[1]=true;
                navigation(counterPageNav);
            }
            break;
            case 3: 
            {
                whoclicked[2]=true;
                performance(counterPagePer);
            }
            break;
        }
    }
    // FUNCTION TO CONNECT AT A WIFI NETWORK SAVED
    void automatic_connection()
    {
        int cont=0;
        String stored_ssid = EEPROM.readString(address_ssid);
        String stored_pass = EEPROM.readString(address_pass);
        String url = EEPROM.readString(ADDRESS_SERVER);
        st_ssid = strdup(stored_ssid.c_str());
        st_pass = strdup(stored_pass.c_str());
        USE_SERIAL.printf("Eccoli: %s %s \n", st_ssid, st_pass);
        USE_SERIAL.println("prima del show");
        //flag=0;
        //keypad_serv.show();
        WiFi.begin(st_ssid,st_pass);
        error_status.setText("Connecting to Wifi...");
        while(WiFi.status() != WL_CONNECTED && WiFi.status() != WL_CONNECT_FAILED)
        {
            delay(1000);
            cont ++;
            if (cont > 10)
            {
                // exit from while
                break;
            }
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            isAutomaticConnection = true;
            error_status.setText("Connected to WiFi");
            delay(1000);
            keypad_serv.show();
            /*USE_SERIAL.println("sto nell'if");
            error_status.setText("Connecting to Wsc...");
            // Connect to the websocket
            webSocket.begin(url, 80, "/signalk/v1/stream?subscribe=none");
            webSocket.onEvent(webSocketAutomaticConnection);*/
        }
        else
        {
            error_status.setText("Load fault!");
            delay(2000);
            connections.show();
            scanningWiFi();
        }

    }
/*************************************************************************************************************************
  WEBSOCKET EVENTS
*************************************************************************************************************************/
void webSocketConnection(WStype_t type, uint8_t * payload, size_t length)
{
    switch(type) 
    {
        case WStype_DISCONNECTED:
        {
            serverURLisSubmitted=false;
        checking_txt.setText("Problem with WebScoket"); 
        USE_SERIAL.printf("[WSc] Disconnected!\n");
        check_connectionwebsocket = false; 
        //printStatus("Disconnected Wsc");
        }
        break;
        case WStype_CONNECTED: 
        {
        check_connectionwebsocket = true;
        USE_SERIAL.println("Connected to the Wsc!");
        checking_txt.setText("Connected to the Wsc!");
        }
        break;
        case WStype_TEXT:
        {     
        //USE_SERIAL.printf("[WSc] get text: %s\n", payload);
        }
        break;
        case WStype_ERROR:
        {
            serverURLisSubmitted=false;
        USE_SERIAL.println("[WSc] ERROR");
        checking_txt.setText("Wsc ERROR");
        }
        break;
  }
}

void webSocketUnsubscribe(WStype_t type, uint8_t * payload, size_t length)
{
    switch(type) 
    {
        case WStype_DISCONNECTED:
        {
            serverURLisSubmitted=false;
        USE_SERIAL.printf("[WSc] Disconnected!\n");
        check_connectionwebsocket = false; 
        
        }
        break;
        case WStype_CONNECTED: 
        {
        check_connectionwebsocket = true;
        USE_SERIAL.println("Connected to the Wsc!");
        }
        break;
        case WStype_TEXT:
        {     
            USE_SERIAL.printf("[WSc] get text: %s\n", payload);
        }
        break;
        case WStype_ERROR:
        {
            serverURLisSubmitted=false;
        USE_SERIAL.println("[WSc] ERROR");
        }
        break;
  }
}

void webSocketSubscribe(WStype_t type, uint8_t * payload, size_t length)
{
    switch(type) 
    {
        case WStype_DISCONNECTED:
        {
            serverURLisSubmitted=false;
            USE_SERIAL.printf("[WSc] Disconnected!\n");
            check_connectionwebsocket = false; 
        }
        break;
        case WStype_CONNECTED: 
        {
            check_connectionwebsocket = true;
            USE_SERIAL.println("Connected to the Wsc!");
        }
        break;
        case WStype_TEXT:
        {   
            
        }
        break;
        case WStype_ERROR:
        {
            serverURLisSubmitted=false;
            USE_SERIAL.println("[WSc] ERROR");
        }
        break;
  }
}

void webSocketFullvaluePage(WStype_t type, uint8_t * payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
    {
    serverURLisSubmitted=false;
      USE_SERIAL.printf("[WSc] Disconnected!\n");
      check_connectionwebsocket = false; 
      //printStatus("Disconnected Wsc");
    }
    break;
    case WStype_TEXT:
    {
      // Parsing
      //USE_SERIAL.printf("GET: %s\n", payload);
      StaticJsonBuffer<1200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);
      JsonObject& updates0 = root["updates"][0];
      float updates0_values0_value = updates0["values"][0]["value"];
      // Converting
      char wd[4];
      dtostrf(updates0_values0_value, 3, 3, wd);
      // Send the value on nextion
      value.setText(wd);
    }
    break;
    case WStype_ERROR:
    {
        serverURLisSubmitted=false;
    }
    break;
  }
}
void webSocketGauge2Page(WStype_t type, uint8_t * payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
    {
        serverURLisSubmitted=false;
      USE_SERIAL.printf("[WSc] Disconnected!\n");
      check_connectionwebsocket = false; 
      //printStatus("Disconnected Wsc");
    }
    break;
    case WStype_TEXT:
    {
      // Parsing
      //USE_SERIAL.printf("GET: %s\n", payload);
      StaticJsonBuffer<1200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);
      JsonObject& updates0 = root["updates"][0];
      float updates0_values0_value = updates0["values"][0]["value"];
      rappresentationAngWind(updates0_values0_value);
    }
    break;
    case WStype_ERROR:
    {
        serverURLisSubmitted=false;
    }
    break;
  }
}
void webSocketGauge1Page(WStype_t type, uint8_t * payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
    {
      serverURLisSubmitted=false;
      USE_SERIAL.printf("[WSc] Disconnected!\n");
      check_connectionwebsocket = false; 
    }
    break;
    case WStype_TEXT:
    {
      // Parsing
      //USE_SERIAL.printf("GET: %s\n", payload);
      StaticJsonBuffer<1200> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(payload);
      JsonObject& updates0 = root["updates"][0];
      float updates0_values0_value = updates0["values"][0]["value"];
      // Converting
      char degval[4];
      // Convert the radiant to degree
      float Degree = updates0_values0_value * 180/PI;
      if(Degree<270)
      {
         dtostrf(Degree, 2, 1, degval);
         g1_value.setText(degval);
         g1_gauge.setValue(Degree+90);
      }
      else 
      {
        dtostrf(Degree, 2, 1, degval);
        g1_value.setText(degval);
        g1_gauge.setValue(Degree-270);
      }
    }
    break;
    case WStype_ERROR:
    {
        serverURLisSubmitted=false;
    }
    break;
  }
}
void webSocketDoubleValue(WStype_t type, uint8_t * payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
    {
        serverURLisSubmitted=false;
      USE_SERIAL.printf("[WSc] Disconnected!\n");
      check_connectionwebsocket = false; 
      //printStatus("Disconnected Wsc");
    }
    break;
    case WStype_TEXT:
    {
        // Parsing
        StaticJsonBuffer<1200> jsonBuffer;
        JsonObject& root = jsonBuffer.parseObject(payload);
        JsonObject& updates0 = root["updates"][0];
        const char* updates0_values0_path = updates0["values"][0]["path"];
        // If the data received is for longitude and latitude
        if (strcmp(updates0_values0_path,"navigation.position") == 0)
        {
                title1.setText("longitude");
                title2.setText("latitude");
                float updates0_values0_value_longitude = updates0["values"][0]["value"]["longitude"];
                float updates0_values0_value_latitude = updates0["values"][0]["value"]["latitude"];
                if (updates0_values0_value_longitude < 0)
                {
                    d1.setText("Ovest");
                }
                else 
                {
                    d1.setText("Est");
                }
                if (updates0_values0_value_latitude < 0)
                {
                    d2.setText("Sud");
                }
                else
                {
                    d2.setText("Nord");
                }
                char longi[4];
                char lat[4];
                dtostrf(updates0_values0_value_latitude, 3, 3, lat);
                dtostrf(updates0_values0_value_longitude, 3, 3, longi);
                // Send the value on nextion
                val1.setText(longi);
                val2.setText(lat);
        }
      
    }
    break;
    case WStype_ERROR:
    {
        serverURLisSubmitted=false;
    }
    break;
  }
}
void webSocketAutomaticConnection(WStype_t type, uint8_t * payload, size_t length)
{
    switch(type) 
    {
        case WStype_DISCONNECTED:
        {
            serverURLisSubmitted=false;
        check_connectionwebsocket = false; 
        checking_txt.setText("Problem with WebScoket"); 
        USE_SERIAL.printf("[WSc] Disconnected!\n");
        delay(1500);
        page_loader();
        //printStatus("Disconnected Wsc");
        }
        break;
        case WStype_CONNECTED: 
        {
        check_connectionwebsocket = true;
        USE_SERIAL.println("Connected to the Wsc!");
        checking_txt.setText("Connected to the Wsc!");
        delay(1500);
        page_loader();
        }
        break;
        case WStype_TEXT:
        {     
        //USE_SERIAL.printf("[WSc] get text: %s\n", payload);
        }
        break;
        case WStype_ERROR:
        {
            serverURLisSubmitted=false;
            check_connectionwebsocket = false; 
            USE_SERIAL.println("[WSc] ERROR");
            checking_txt.setText("Wsc ERROR");
            delay(1500);
            page_loader();
        }
        break;
  }
}
#endif /* GENERAL_FUNCTIONS_H */