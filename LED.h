//
// LED.h - LED Controller
//
#ifndef LED_h
#define LED_h

#define FASTLED_ESP8266_D1_PIN_ORDER
#include <FastLED.h>
#include "Config.h"
#include "defaults.h"

#define UPDATES_PER_SECOND 100

extern CRGBPalette16 myRedWhiteBluePalette;
//extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};


//
// LED Library Class
class LED
{
  public:
    LED();
    void begin( Config *config );
    void loop();
    
  private:
    Config     *_config;
    CRGB       _leds[NUM_LEDS];

    CRGBPalette16 _currentPalette;
    TBlendType    _currentBlending;

    void SetupPurpleAndGreenPalette();
    void SetupBlackAndWhiteStripedPalette();
    void SetupTotallyRandomPalette();
    void ChangePalettePeriodically();
    void FillLEDsFromPaletteColors( uint8_t colorIndex);

};

#endif
