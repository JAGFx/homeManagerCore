

#include "Engine/GUI.h"

#define DEBOUNCE_DELAY_MS 1000

GUI gui;

enum BTN {
    A,
    B,
    C
};

volatile unsigned long lastMillis = 0;

bool isPressed( BTN btn ) {
    unsigned long currentMillis = millis();
    unsigned long deltaMillis   = currentMillis - lastMillis;
    bool          btnPressValue = false;
    
    if ( btn == BTN::A )
        btnPressValue = M5.BtnA.read();
    else if ( btn == BTN::B )
        btnPressValue = M5.BtnB.read();
    else if ( btn == BTN::C )
        btnPressValue = M5.BtnC.read();
    
    bool isPressed = btnPressValue && deltaMillis >= DEBOUNCE_DELAY_MS;
    
    if ( isPressed )
        lastMillis = currentMillis;
    
    return isPressed;
}

// the setup routine runs once when M5Stack starts up
void setup() {
    Serial.begin( 115200 );
    
    gui.init();
}

// the loop routine runs over and over again forever
void loop() {
    if ( isPressed( BTN::A ) ) {
        Serial.println( "A pressed" );
        gui.reset( GUI::SmartBox::Smart1 );
    }
    
    if ( isPressed( BTN::B ) ) {
        Serial.println( "B pressed" );
        gui.reset( GUI::SmartBox::Smart2 );
    }
    
    if ( isPressed( BTN::C ) ) {
        Serial.println( "C pressed" );
        gui.reset( GUI::SmartBox::Smart3 );
    }
    
    gui.display();
}
