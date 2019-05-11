//
// Created by CLion.
// @author: SMITH Emmanuel <hey@emmanuel-smith.me>
// @site: https://emmanuel-smith.me
// Date: 03-May-19
// Time: 12:07
//

#include "CloudSaver.h"

CloudSaver::CloudSaver() {}


// -------------------------------------
// -- Mod management

void CloudSaver::init() {
    timeClient.begin();
}

void CloudSaver::update() {
    timeClient.update();
}

void CloudSaver::logInCloud( String currentName, String tempTmp, String lux ) {
    String              postHeader = String( "/HomeManager/happy/happy.php?b=" + currentName + "&a=improve" );
    String              dataStr;
    DynamicJsonDocument payload( 200 );
    
    String formattedDate = timeClient.getFormattedDate();
    String dateStr       = formattedDate.substring( 0, 10 );
    String timeStr       = formattedDate.substring( 11, 19 );
    
    payload[ "t" ] = tempTmp;
    payload[ "l" ] = lux;
    payload[ "d" ] = dateStr + " " + timeStr;
    serializeJson( payload, dataStr );
    
    /*Serial.println( "------------URL" );
    Serial.println( postHeader );
    Serial.println( "------------DATA" );
    Serial.println( dataStr );*/
    
    client.post( postHeader, "application/json", dataStr );
    
    /*int statusCode = client.responseStatusCode();
    String response = client.responseBody();
    Serial.println( "------------RESPONSE" );
    Serial.println( statusCode );
    Serial.println( response );*/
}

// -------------------------------------
