//
// Created by CLion.
// @author: SMITH Emmanuel <hey@emmanuel-smith.me>
// @site: https://emmanuel-smith.me
// Date: 01-May-19
// Time: 10:01
//

#ifndef DEV_WSSERVER_H
#define DEV_WSSERVER_H


#include "IMod.h"
#include "Settings.h"
#include "GUI.h"
#include <WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

class WSServer : public IMod {
public:
    explicit WSServer();
    
    // Websocket
    static const uint8_t MAX_CLIENTS = 2;
    // ----
    
    // Mod management
    virtual void init();
    
    virtual void update();
    
    GUI *getGui();
    // ----

private:
    // Mod management
    GUI gui;
    // ----
    
    // Websocket
    WiFiMulti        wiFiMulti;
    WebSocketsServer webSocket       = WebSocketsServer( WEBSOCKET_PORT );
    uint8_t          clientCount     = 0;
    
    void webSocketEvent( uint8_t num, WStype_t type, uint8_t *payload, size_t length );
    // ----
};

#endif //DEV_WSSERVER_H
