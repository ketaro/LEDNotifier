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


#define DISPLAY_DARK 0
#define DISPLAY_IDLE 1
#define DISPLAY_RANDOM 2
#define DISPLAY_RAINBOW 3
#define DISPLAY_BLACKANDWHITE 4
#define DISPLAY_PARTY 5
#define DISPLAY_REDALERT 6



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

const TProgmemPalette16 redPalette_p PROGMEM = {
    CRGB::Red, CRGB::Red, CRGB::Red,
    CRGB::Black, CRGB::Black, CRGB::Black,
    CRGB::Red, CRGB::Red, CRGB::Red,
    CRGB::Black, CRGB::Black, CRGB::Black,
    CRGB::Red, CRGB::Red,
    CRGB::Black, CRGB::Black
};

//
// LED Library Class
class LED
{
  public:
    LED();
    void begin( Config *config );
    void loop();
    String getJSON();
    void setDisplay( uint8_t mode, uint duration );
    void setDelay( uint delay );
    void setBrightness( uint8_t value );
    
  private:
    Config     *_config;
    CRGB       _leds[NUM_LEDS];

    CRGBPalette16 _currentPalette;
    TBlendType    _currentBlending;

    uint     _displayStart;
    uint     _delay;
    uint     _duration;    // How long to stay in this mode before going back to idle
    

    void SetupPurpleAndGreenPalette();
    void SetupBlackAndWhiteStripedPalette();
    void SetupTotallyRandomPalette();
    void ChangePalettePeriodically();
    void FillLEDsFromPaletteColors( uint8_t colorIndex);

};

#endif
