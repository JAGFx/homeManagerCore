//
// Created by CLion.
// @author: SMITH Emmanuel <hey@emmanuel-smith.me>
// @site: https://emmanuel-smith.me
// Date: 01-May-19
// Time: 10:01
//

#include "WSServer.h"

using namespace std::placeholders;

WSServer::WSServer() : gui( GUI() ) {}

// -------------------------------------
// -- Mod management

void WSServer::init() {
    // ---- Wifi setup
    for ( uint8_t t = 4; t > 0; t-- ) {
        Serial.printf( "[SETUP] BOOT WAIT %d...\n", t );
        Serial.flush();
        delay( 1000 );
    }
    
    wiFiMulti.addAP( WIFI_SSID, WIFI_PSWD );
    Serial.print( "Connecting to WIFI" );
    
    while ( wiFiMulti.run() != WL_CONNECTED ) {
        Serial.print( "." );
        delay( 100 );
    }
    
    Serial.println( "DONE" );
    
    // ---- GUI setup
    getGui()->init();
    
    // ---- Websocket setup
    webSocket.begin();
    webSocket.onEvent( std::bind( &WSServer::webSocketEvent, this, _1, _2, _3, _4 ) );
}

void WSServer::update() {
    getGui()->update();
    webSocket.loop();
}

GUI *WSServer::getGui() {
    return &gui;
}

// -------------------------------------


// -------------------------------------
// -- Websocket

void WSServer::webSocketEvent( uint8_t num, WStype_t type, uint8_t *payload, size_t length ) {
    DynamicJsonDocument payloadData( 200 );
    deserializeJson( payloadData, payload, DeserializationOption::NestingLimit( 2 ) );
    /*Serial.print("Raw");
    serializeJsonPretty(payloadData, Serial);*/
    
    DynamicJsonDocument payloadToSend( 200 );
    String              output;
    
    const char *requestType = payloadData[ "type" ];
    const char *uid         = payloadData[ "c" ];
    
    /*DynamicJsonDocument datas(200);
    deserializeJson(datas, payloadData[ "data" ]);*/
    
    
    const char *box   = payloadData[ "data" ][ "box" ];
    float      temp   = payloadData[ "data" ][ "temp" ];
    int        lux    = payloadData[ "data" ][ "lux" ];
    const char *title = payloadData[ "data" ][ "title" ];
    
    switch ( type ) {
        case WStype_DISCONNECTED:
            Serial.printf( "[%u] Disconnected!  / %u\n", num, clientCount );
            break;
        
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP( num );
            Serial.printf( "[%u] Connected from %d.%d.%d.%d -- %u url: %s\n",
                           num,
                           ip[ 0 ],
                           ip[ 1 ],
                           ip[ 2 ],
                           ip[ 3 ],
                           clientCount,
                           payload );
            
            payloadToSend[ "state" ] = "Connected";
            deserializeJson( payloadToSend, output );
            webSocket.sendTXT( num, output );
            
            //Serial.println(clientCount);
            /*if( ++clientCount > MAX_CLIENTS ){
              webSocket.disconnect( num );
            }*/
            
            Serial.println( clientCount );
        }
            break;
        
        case WStype_TEXT:
            Serial.printf( "[%u] get Text\n", num );
            
            if ( strcmp( requestType, "auth" ) == 0 && uid != NULL ) {
                if ( strcmp( uid, WEBSOCKET_ID_SMARTBOX_1 ) == 0 || strcmp( uid, WEBSOCKET_ID_SMARTBOX_2 ) == 0 ||
                     strcmp( uid, WEBSOCKET_ID_SMARTBOX_3 ) == 0 ) {
                    Serial.println( "Smartbox connected !" );
                    
                    String currentName = String( title );
                    String currentTemp = String( String( temp, 1 ) + " C" );
                    String currentLux  = String( String( lux ) + " lux" );
                    
                    /*Serial.print( "T:" );
                    Serial.println( temp );
    
                    Serial.print( "Lux:" );
                    Serial.println( lux );
    
                    Serial.print( "Box:" );
                    Serial.println( box );
    
                    Serial.print( "Title:" );
                    Serial.println( title );*/
                    
                    getGui()->updateData( GUI::SmartBox::Smart1, currentName, currentTemp, currentLux );
                    
                    serializeJson( payloadData[ "data" ], output );
                    
                    // --- Log in csv file
                    //saveDataInFileForBox( box, tempTmp, lux );
                    //logOnCLoud( uid, tempTmp, lux );
                    // --- END Log in csv file
                    
                    webSocket.broadcastTXT( output );
                }
            }
            break;
        
        case WStype_BIN:
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            break;
    }
}

// -------------------------------------
