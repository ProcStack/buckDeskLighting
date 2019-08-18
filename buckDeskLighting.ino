// DEFAULT ADAFRUIT HEADER INFO --- 
// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    6
#define LED_PIN_DUEL    9

#define LED_COUNT 3

// Strip 1 --
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Strip 2 --
Adafruit_NeoPixel stripDuel(LED_COUNT, LED_PIN_DUEL, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


int runner=0;
void setup() {
  // Boot Strip 1 --
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  // Boot Strip 2 --
  stripDuel.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  stripDuel.show();            // Turn OFF all pixels ASAP
  stripDuel.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  rainbow(10);
  runner++;
}


void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      int pixelHue = firstPixelHue + runner + (runner + i * 65536L / strip.numPixels());
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    
    
    for(int i=0; i<stripDuel.numPixels(); i++) { // For each pixel in strip...
      int pixelHue = firstPixelHue + ((i+runner) * 636L / stripDuel.numPixels());
      stripDuel.setPixelColor(i, stripDuel.gamma32(stripDuel.ColorHSV(pixelHue)));
    }
    
    strip.show(); // Update strip with new contents
    stripDuel.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
