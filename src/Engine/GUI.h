//
// Created by CLion.
// @author: SMITH Emmanuel <hey@emmanuel-smith.me>
// @site: https://emmanuel-smith.me
// Date: 27-Apr-19
// Time: 10:05
//

#ifndef DEV_GUI_H
#define DEV_GUI_H

#include <M5Stack.h>


class GUI {
public:
    enum SmartBox {
        Smart1,
        Smart2,
        Smart3,
    };
    
    void init();
    
    void display();
    
    void reset( SmartBox smartBox );
    
    void updateData( SmartBox smartBox, const String name, const String temp, const String lux );

private:
    SmartBox currentSmartBox = SmartBox::Smart1;
    String   currentName     = "Windows";
    String   currentTemp     = "25.5 C";
    String   currentLux      = "1054 lux";
    
    bool needToRefresh = false;
};


#endif //DEV_GUI_H
