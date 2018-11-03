// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of
// FastLED compact palettes are at the bottom of this file.

#include "LED.h"
#include "defaults.h"

LED::LED() {

}


//
// Public Functions
//

void LED::begin( Config *config ) {
    // Keep a reference to the config
    _config = config;
  
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(_leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness( BRIGHTNESS );

    // Display for 10 seconds for going to idle
    _currentPalette = RainbowColors_p;
    _currentBlending = LINEARBLEND;

    _displayStart = millis();
    _duration = 10;
    _delay = 100;

    Serial.println( "[LED] begin complete." );
}


void LED::loop() {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex );
    
    FastLED.show();
    FastLED.delay( _delay );

    if (_duration > 0 && millis() > _displayStart + _duration*1000) {
      // Switch to "idle" display
      Serial.println( "[LED] Switching to idle mode" );
      setDisplay( DISPLAY_IDLE, 0 );
      _duration = 0;
      _displayStart = millis();
    }
}


String LED::getJSON() {
  String jsonstr = "{"
                   "\"leds\": " + String(NUM_LEDS) + ", "
                   "\"delay\": " + String(_delay) + " "
                   "}";

  return jsonstr;
}

void LED::setDelay( uint delay ) {
    _delay = delay;
    Serial.println( "[LED] delay: " + String( _delay ) );
}


void LED::setBrightness( uint8_t value ) {
    FastLED.setBrightness( value );
    Serial.println( "[LED] Brightness: " + String( value ) );
}

void LED::setDisplay( uint8_t mode, uint duration ) {
    Serial.println( "[LED] setDisplay " + String(mode) + " for " + String(duration) );
    switch (mode) {
        case DISPLAY_IDLE:
            _currentPalette = CloudColors_p;
            _currentBlending = LINEARBLEND;
            break;
        case DISPLAY_DARK:
            // 'black out' all 16 palette entries...
            fill_solid( _currentPalette, 16, CRGB::Black);
            break;
        case DISPLAY_RAINBOW:
            _currentPalette = RainbowStripeColors_p;
            _currentBlending = LINEARBLEND;
            break;
        case DISPLAY_RANDOM:
            SetupTotallyRandomPalette();
            _currentBlending = LINEARBLEND;
            break;
        case DISPLAY_BLACKANDWHITE:
            SetupBlackAndWhiteStripedPalette();
            _currentBlending = LINEARBLEND; 
            break;
        case DISPLAY_PARTY:
            _currentPalette = PartyColors_p;
            _currentBlending = LINEARBLEND;
            break;
        case DISPLAY_REDALERT:
            _currentPalette = redPalette_p;
            _currentBlending = NOBLEND;
            break;

    }

    _duration = duration;
    _displayStart = millis();
}


//
// Private Functions
//

void LED::FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        _leds[i] = ColorFromPalette( _currentPalette, colorIndex, brightness, _currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void LED::ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { _currentPalette = RainbowColors_p;         _currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { _currentPalette = RainbowStripeColors_p;   _currentBlending = NOBLEND;  }
        if( secondHand == 15)  { _currentPalette = RainbowStripeColors_p;   _currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();              _currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();               _currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();        _currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();        _currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { _currentPalette = CloudColors_p;           _currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { _currentPalette = PartyColors_p;           _currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { _currentPalette = myRedWhiteBluePalette_p; _currentBlending = NOBLEND;  }
        if( secondHand == 55)  { _currentPalette = myRedWhiteBluePalette_p; _currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void LED::SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        _currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void LED::SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( _currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    _currentPalette[0] = CRGB::White;
    _currentPalette[4] = CRGB::White;
    _currentPalette[8] = CRGB::White;
    _currentPalette[12] = CRGB::White;
}

// This function sets up a palette of purple and green stripes.
void LED::SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    _currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}



// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
