#pragma once

#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>




#include "memory.h"
#include "registers.h"
#include "effects.h"

#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoOTA.h>



#define TIME_ON_CONNECT 10000
#define max_client  10
const String HOME_SSID = "OpenWrtHome";
const String HOME_PASSWORD = "cuprum22";
const String AP_SSID = "led_cube";
const String AP_PASSWORD = "12345678";




class WEB{
public:
    WEB();
    
    ESP8266WebServer server;
    WebSocketsServer webSocket;
    REGISTERS *registors;
    EFFECTS *effects;

    
    bool webSocket_it_connect;
    void check_client();
    void start_all_server();
    void loop();
    void wifi_init();
    bool handleFileRead(String path);
    void query_file();
    void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght);
    void send_string(String data);
    void get_command(uint8_t client_num , uint8_t * payload, size_t lenght);
    void send_effect_name(uint8_t client_num);
    void del_file(uint8_t * payload, size_t lenght);
    
    void set_date(uint8_t * payload, size_t lenght);
    void change_pin_status(uint8_t * payload);
    void reset_settings();// tmp no aviable)
    void send_settings(uint8_t client_num);// tmp no aviable)
    void start_ota();
    void reset();
    void reset_into_uart();
};

