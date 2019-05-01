//
// Created by CLion.
// @author: SMITH Emmanuel <hey@emmanuel-smith.me>
// @site: https://emmanuel-smith.me
// Date: 27-Apr-19
// Time: 10:05
//

#include "GUI.h"

GUI::GUI() {}

// -------------------------------------
// -- Mod management

void GUI::init() {
    M5.begin();
    needToRefresh = true;
    update();
}

void GUI::update() {
    if ( needToRefresh ) {
        M5.Lcd.fillScreen( BLACK );
        M5.Lcd.fillRect( 0, 0, 320, 50, DARKGREY );
        
        M5.Lcd.setCursor( 0, 0 );
        M5.Lcd.setTextDatum( MC_DATUM );
        M5.Lcd.setTextColor( WHITE );
        
        // Name of current smart box
        M5.Lcd.setTextSize( 3 );
        M5.Lcd.drawString( currentName, 160, 0, 2 );
        
        // Current temp
        M5.Lcd.setTextSize( 4 );
        M5.Lcd.drawString( currentTemp, 160, 110, 4 );
        
        // Current luminosity
        M5.Lcd.setTextSize( 2 );
        M5.Lcd.drawString( currentLux, 160, 180, 4 );
        
        // Show current smart box
        if ( currentSmartBox == SmartBox::Smart1 )
            M5.Lcd.fillTriangle( 62, 220, 78, 220, 70, 226, WHITE );
        
        if ( currentSmartBox == SmartBox::Smart2 )
            M5.Lcd.fillTriangle( 157, 220, 173, 220, 165, 226, WHITE );
        
        if ( currentSmartBox == SmartBox::Smart3 )
            M5.Lcd.fillTriangle( 252, 220, 268, 220, 260, 226, WHITE );
        
        M5.update();
        needToRefresh = false;
    }
}

// -------------------------------------


// -------------------------------------
// -- GUI

void GUI::reset( SmartBox smartBox ) {
    currentSmartBox = smartBox;
    currentName     = "----";
    currentTemp     = "--.- C";
    currentLux      = "---- lux";
    
    needToRefresh = true;
    
    update();
}

void GUI::updateData( SmartBox smartBox, const String name, const String temp, const String lux ) {
    currentSmartBox = smartBox;
    currentName     = name;
    currentTemp     = temp;
    currentLux      = lux;
    
    needToRefresh = true;
    update();
}

// -------------------------------------
