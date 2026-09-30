#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

//Bibliotecas

    #include <Adafruit_GFX.h>
    //#include <Adafruit_SSD1306.h>
    #include <Adafruit_SH110X.h>

//Configuracao de instancia

    #define SCREEN_WIDTH 128 // OLED display width, in pixels
    #define SCREEN_HEIGHT 64 // OLED display height, in pixels
    #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
    //#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
    #define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
    //Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#endif