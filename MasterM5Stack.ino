/*
 * WebSocketServer.ino
 *
 *  Created on: 22.05.2015
 *
 */

// -----------------------------------------
// --- Includes

#include <M5Stack.h>

#include <Arduino.h>
#include <ArduinoJson.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <WebSocketsServer.h>


// -----------------------------------------
// --- Init - Websocket

WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(8080);

DynamicJsonBuffer jsonBuffer(200);

const char ID_SMARTBOX_1[26] = "CW2$(M6uC6izC{m!{X7vm46@8";
const char ID_SMARTBOX_2[26] = "xEvw3:64~a3}<7iV@7@ACX5Yp";
const char ID_SMARTBOX_3[26] = "h!,DkUZBg<Qd[6v8:b264*5Z6";

uint8_t clientCount = 0;
const uint8_t MAX_CLIENTS = 2;


// -----------------------------------------
// --- Init - M5Stack

String currentSmartBox = "smart1";
String currentName = "----";
String currentTemp = "--.-- °C";
String currentLux = "---- lux";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
String formattedDate;


// -----------------------------------------
// --- Methods


// --- webSocketEvent
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  jsonBuffer.clear();
  
  JsonObject& payloadData = jsonBuffer.parseObject(payload);
  JsonObject& payloadToSend = jsonBuffer.createObject();
  String output;
  String dataStr;
  const char* requestType = payloadData[ "type" ];
  const char* uid = payloadData[ "c" ];
  
  const JsonObject& datas = payloadData[ "data" ];
  const char* box = datas[ "box" ];
  const char* temp = datas[ "temp" ];
  const char* lux = datas[ "lux" ];
  const char* title = datas[ "title" ];
 
  switch(type) {
      case WStype_DISCONNECTED:
          Serial.printf("[%u] Disconnected!  / %u\n", num, clientCount);
          /*if( clientCount > 0 )
            clientCount--;*/
            
          break;
          
      case WStype_CONNECTED:
        {
          IPAddress ip = webSocket.remoteIP(num);
          Serial.printf("[%u] Connected from %d.%d.%d.%d -- %u url: %s\n", num, ip[0], ip[1], ip[2], ip[3], clientCount, payload);
          
          payloadToSend["state"] = "Connected";
          //payloadToSend.printTo(output);
          webSocket.sendTXT(num, output);
          
          //Serial.println(clientCount);
          /*if( ++clientCount > MAX_CLIENTS ){
            webSocket.disconnect( num );
          }*/
          
          Serial.println(clientCount);
        }
        break;

      case WStype_TEXT:
        
          Serial.printf("[%u] get Text\n", num);
          //payloadData.prettyPrintTo(Serial);
          //Serial.println(payloadData.size());
          //Serial.println(length);
        
        
        //datas.prettyPrintTo(Serial);
        
        if( strcmp (requestType, "auth" ) == 0 && uid != NULL ){
          if( strcmp (uid,ID_SMARTBOX_1) == 0 || strcmp (uid,ID_SMARTBOX_2) == 0 || strcmp (uid,ID_SMARTBOX_3) == 0 ){
            Serial.println( "Smartbox connected !" );
            String tempTmp = String( temp );
            String luxTmp = String( lux );

            // --- Log in csv file
            saveDataInFileForBox( box, tempTmp, lux );            
            // --- END Log in csv file
            
            if( currentSmartBox.equals( box ) ){
              currentName = String( title );
              currentTemp = String( String( tempTmp.toFloat(), 2 ) + " °C");
              currentLux = String( luxTmp + " lux" );
              
              displayPane();
            }
            
            datas.printTo(dataStr);
            
            webSocket.broadcastTXT(dataStr);
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

// --- displayPane
void displayPane(  ){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.fillRect(0, 0, 320, 30, DARKGREY);
  
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  
  int x = 260;
  int y = 226;

  // Name of current smart box
  M5.Lcd.drawString(currentName, 160, 15, 2);
  
  // Current temp
  M5.Lcd.drawString(currentTemp, 160, 100, 4);

  // Current luminosity
  M5.Lcd.drawString(currentLux, 160, 150, 2);

  // Show current smart box 
  if( currentSmartBox.equals( "smart1" ) )
    M5.Lcd.fillTriangle(62, 220, 78, 220, 70, 226, WHITE);
  
  if( currentSmartBox.equals( "smart2" ) )
    M5.Lcd.fillTriangle(157, 220, 173, 220, 165, 226, WHITE);
  
  if( currentSmartBox.equals( "smart3" ) )
    M5.Lcd.fillTriangle(252, 220, 268, 220, 260, 226, WHITE);

  /*M5.Lcd.drawLine(x - 5, y, x + 5, y, GREEN);
  M5.Lcd.drawLine(x, y - 5, x, y + 5, GREEN);*/
  Serial.println(formattedDate);
}

// --- resetPane
void resetPane(){
  currentName = "----";
  currentTemp = "--.-- °C";
  currentLux = "---- lux";
  displayPane();
}

// --- createSystFolder
void saveDataInFileForBox( String currentBox, String tempTmp, String lux ){
  formattedDate = timeClient.getFormattedDate();
  String yearStr = formattedDate.substring(0, 4);
  String monthStr =  formattedDate.substring(5, 7);
  String pathFile = "/" + yearStr + "/" + monthStr + "/" + currentBox + ".csv";
  
  File file;
  char logLine[50];
  
  Serial.println("// ------------ Log for: " + currentBox );
  Serial.println( "\t> Path: " + pathFile );
  
  if( !SD.exists( "/" + yearStr) ){
    Serial.println("\t> \"" + yearStr + "\" do not exist. Created");
    SD.mkdir( "/" + yearStr );
  }

  if( !SD.exists( "/" + yearStr + "/" + monthStr) ){
    Serial.println("\t> \"" + yearStr + "/" + monthStr + "\" do not exist. Created");
    SD.mkdir( "/" + yearStr + "/" + monthStr );
  }

  Serial.print( "\t> Writing..." );

  if( !SD.exists(pathFile) )
    file = SD.open(pathFile.c_str(), FILE_WRITE);
  else
    file = SD.open(pathFile.c_str(), FILE_APPEND);
  
  sprintf( logLine, "%s,%s,%s", formattedDate.c_str(), tempTmp.c_str(), lux.c_str() );
  
  if(file.println( logLine )){
      Serial.println("DONE");
  } else {
      Serial.println("FAIL");
  }
  file.close();

  Serial.println("// END ------------ Log for: " + currentBox );
}

// -----------------------------------------
// --- Setup

void setup() {
  // initialize the M5Stack object
  M5.begin();
  
  // Serial.begin(921600);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println();
  Serial.println();
  
  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  
  WiFiMulti.addAP("Norway", "523653542194681AA7215F5896");
  Serial.print("Conneccting to WIFI");
  
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("DONE");

  timeClient.begin();
  timeClient.setTimeOffset(3600);
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  displayPane();
  Serial.println( "Ready !!!!!!" );
}


// -----------------------------------------
// --- Loop

void loop() {
  if( M5.BtnA.wasPressed() ){
    currentSmartBox = "smart1";
    resetPane();
  }

  if( M5.BtnB.wasPressed() ){
    currentSmartBox = "smart2";
    resetPane();
  }

  if( M5.BtnC.wasPressed() ){
    currentSmartBox = "smart3";
    resetPane();
  }
  
  M5.update();
  timeClient.update();
  webSocket.loop();
}
