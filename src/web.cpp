#include "web.h"



String getContentType(String filename){
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  return "text/plain";
}




// void WEB::set_date(uint8_t * payload, size_t lenght){
//     Serial.println("Set global time on client");
//     assert(lenght == 8);
//     Sensors->amendment =  (int32_t)payload[4] * 3600000 + (int32_t)payload[5] * 60000 + (int32_t)payload[6] * 1000 + (int32_t)payload[7] * 10 - millis();
//     Sensors->date.day = payload[1];
//     Sensors->date.mounth = payload[2];
//     Sensors->date.year = payload[3];
//     Sensors->time_recalc_big();
//     if(!Sensors->date_is_ccorrect){
//         Sensors->date_is_ccorrect = true;
//         Log->open_file();
//     }
// }


void WEB::webSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch (type){
        case WStype_CONNECTED:
            Serial.println("WebSocket is Connected");
            webSocketIsConnect = true;
            break;
        case WStype_DISCONNECTED:
            Serial.println("WebSocket is Disonnected");
            break;
        case  WStype_BIN:
            Serial.println("read BIN data to server!!!");
            get_command(client_num ,payload , lenght);
            break;
    }
}

bool WEB::handleFileRead(String path) {
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path); 
  if (SPIFFS.exists(path)) { 
    File file = SPIFFS.open(path, "r"); 
    size_t sent = server.streamFile(file, contentType);
    Serial.println(sent);
    file.close(); 
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;
}

void WEB::query_file(){
    registors->change_status(0);
    if (!handleFileRead(server.uri()))
        server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
    registors->change_status(1);
}

WEB::WEB() : server(80) , webSocket(81), webSocketIsConnect(false) {

}

void WEB::start_all_server() {
    server.begin(); 
    server.onNotFound(std::bind(&WEB::query_file, this));
    Serial.println("webserver started.");
    webSocket.begin();               
    webSocket.onEvent( std::bind(&WEB::webSocketEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4) );
    Serial.println("WebSocket server started.");
   
}

void WEB::loop(){
    server.handleClient();
    webSocket.loop(); 
    ArduinoOTA.handle();
}

void WEB::wifi_init(){
    
    int number_network = WiFi.scanNetworks();
    bool find_home = 0;
    Serial.println("start connect");
    for(int i = 0; i < number_network ; i++){
        if(HOME_SSID == WiFi.SSID(i)){
            find_home = 1;
            break;
        }
    }
    if(find_home){
        WiFi.mode(WIFI_STA);
        long start_time = millis();
        WiFi.begin(HOME_SSID , HOME_PASSWORD);
        while(WiFi.status() != WL_CONNECTED && millis() - start_time < TIME_ON_CONNECT) {
            delay(250);
            Serial.println("TRY");
            }
        if(!WiFi.begin(HOME_SSID , HOME_PASSWORD)){
            find_home = 0;
            Serial.println("FAIL");
        }
        else{
            Serial.println("Connected!!! \nHOME MODE");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP()); 
        }
    }
    if(!find_home){
        WiFi.softAP(AP_SSID, AP_PASSWORD);
        Serial.println("AP MODE");
        Serial.print("IP address:\t");
        Serial.println(WiFi.softAPIP());
    }
    start_ota();
    
    
}


 void WEB::send_effect_name(uint8_t client_num){
    Serial.println("send_file_name");
    
    size_t sz = 2;
    int8_t cnt_file = 0; 
    for(uint8_t i = 0 ; i < effect_count;i++){
        sz = sz + 1 + effects->effect_name[i].length();
    }

    uint8_t data[sz];
    data[0] = 0; 
    data[1] = effect_count;
    int it = 2;

    for(uint8_t i = 0 ; i < effect_count; i++){
        data[it++] = effects->effect_name[i].length();
        for(uint8_t x : effects->effect_name[i]){
            data[it++] = x;
        }
    }
    Serial.println("effect cnt: " + String(effect_count));
    webSocket.sendBIN(client_num ,(const uint8_t *)data , sz);
}



void WEB::reset_settings(){
    
}

// void WEB::send_settings(uint8_t client_num){
//     Serial.println("send_settings");
//     uint8_t data[2];
//     memset(data , 0 , sizeof data);
//     data[0] = 1;
//     data[1] |= ((uint8_t)Log->log_is_write << 7);
//     webSocket.sendBIN(client_num , (const uint8_t *)data , 2);
// }


void WEB::reset(){
    delay(500);
    Serial.println("soft reset");
    ESP.reset();
}

void WEB::reset_into_uart(){
    delay(500);
    Serial.println("soft reboot into uart mode");
    ESP.rebootIntoUartDownloadMode();
}
void WEB::change_pin_status(uint8_t* payload){
    Serial.println("change pin status");
    if(payload[1]){

    }
    
    
}


 void WEB::get_command(uint8_t client_num , uint8_t * payload, size_t lenght){
    // assert(lenght);
    switch (payload[0]){
    case 0:
        // set_date(payload , lenght);
        break;
    case 1:
        send_effect_name(client_num);
        break;
    case 2:
        // del_file(payload, lenght);
        break;
    case 3:
        // kek
        break;
    case 4:
        // log_change_status(1);
        break;
    case 5:
        // log_change_status(0);
        break;
    case 6:
        // reset_settings();
        break;
    case 7:
        // send_settings(client_num);
        break;
    case 8:
        change_pin_status(payload);
    case 9:
        reset();
        break;
    case 10:
        reset_into_uart();
        break;
    default:
        // assert(0);
        break;
    }
 }



void WEB::start_ota(){
    ArduinoOTA.onStart([&]() {
        registors->change_status(0);
        Serial.println("OFF TIMER");
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
}
bool WEB::getWebSocketIsConnect(){
    return webSocketIsConnect;
}