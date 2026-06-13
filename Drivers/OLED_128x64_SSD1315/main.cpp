#include "MultipleI2C.h"
#include "SSD1315.h"
#include "MiniGFX.h"
#include "Icons.h"

extern "C" void app_main() {
    static MultipleI2C bus;
    bus.setup(0x3C);
    static SSD1315 oled(&bus);
    oled.init();
    MiniGFX gfx(&oled);

    oled.clear();
    gfx.drawRect(0, 0, 128, 64, true);
    gfx.drawBitmap(2,2,wifi_16x16,15,15,true);
    gfx.drawBitmap(20,3,battery_3_19x10,19,10,true);
    gfx.drawText(20,32,"EMERGENCY",true);
    oled.update();


}