//
// Created by CLion.
// @author: SMITH Emmanuel <hey@emmanuel-smith.me>
// @site: https://emmanuel-smith.me
// Date: 03-May-19
// Time: 12:07
//

#ifndef DEV_CLOUDSAVER_H
#define DEV_CLOUDSAVER_H


#include "IMod.h"
#include "Settings.h"
#include <ArduinoHttpClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

class CloudSaver : public IMod {
public:
    CloudSaver();
    
    // Mod management
    virtual void init();
    
    virtual void update();
    
    void logInCloud( String currentName, String tempTmp, String lux );
    // ----

private:
    // Request sender
    WiFiClient wifi;
    HttpClient client    = HttpClient( wifi, CLOUD_SAVER_ENDPOINT_ADDR, CLOUD_SAVER_ENDPOINT_PORT );
    // ----
    
    // NTP Server
    WiFiUDP   ntpUDP;
    NTPClient timeClient = NTPClient( ntpUDP );
    // ----
};


#endif //DEV_CLOUDSAVER_H
