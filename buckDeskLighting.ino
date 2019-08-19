////////////////////////////////////////////////////////////////////////////
// LED Desk Lighting; 2019                                                //
// Written by Kevin Edzenga                                               //
//                                                                        //
// This file uses LED blending I wrote some time back,                    //
// There may be variation between that repo code and this files,          //
// Everyday is a learning experience!                                     //
//                                                                        //
// ProcStack / Arduino / LED_Color_Blending                               //
// https://github.com/ProcStack/Arduino/tree/master/LED_Color_Blending    //
//                                                                        //
// The line -   strip1.show();                                             //
// Is where the NeoPixels values are set,                                 //
//   you can use ' byte fadeR, fadeG, fadeB; ' variables                  //
//   how ever you'd like once blended.                                    //
//                                                                        //
// For ease of use, NeoPixels are amazing-                                //
// https://www.adafruit.com/category/168                                  //
//                                                                        //
// Required headers---                                                    //
// https://github.com/adafruit/Adafruit_NeoPixel                          //
//                                                                        //
// For the breadboard layout, 300-500 ohm resistor on pin 6 & pin 9       //
//  Using two resistors just reduces stress on the resistor itself.       //
//  Doing this with a single 1/4 watt resistor shouldn't be a problem.    //
// 1000-4700 uf 16v capacitor should be used on VCC / GND                 //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

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
 #include <avr/power.h>
#endif

#include <math.h> // Linux
// Windows seems to use a capital M, you may need to change this per OS
// #include <Math.h>

// LED Strip
#define PIN 6 // This is your SPI out to your NeoPixel strip
#define LED_PIN_1    6
#define LED_PIN_2    9
#define LED_COUNT    3

#define TAU PI*2.0


// For prepping maximum blended values; I'm using a 3 dimensional array (Below)
// The first array is the number of lights
// The second array is the maximum levels of blending
// The third array is R, G, B respectively
#define I_BLEND_COUNT 10
#define BLEND_COUNT 10.0
int lightVals[LED_COUNT][I_BLEND_COUNT][3];

int irunner=0;
float runner=0;
float mmRGB=200; // 255 is the max brightness an LED can be, but you can set it anywhere between 0-255

// Strip 1 --
Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN_1, NEO_GRB + NEO_KHZ800);
// Strip 2 --
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN_2, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


void setup() {
  // Boot Strip 1 --
  strip1.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip1.show();            // Turn OFF all pixels ASAP
  strip1.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  // Boot Strip 2 --
  strip2.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip2.show();            // Turn OFF all pixels ASAP
  strip2.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  irunner++;
  runner++;
  
  if(irunner%200<100){
    ledFader(10); // Fade 'random' colors through your string
  }else{
    ledRun(10); // Run white leds down your string, showing the fade off rates
  }
}


//
void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip1.numPixels(); i++) { // For each pixel in strip...
      int pixelHue = firstPixelHue + irunner + (irunner + i * 65536L / strip1.numPixels());
      strip1.setPixelColor(i, strip1.gamma32(strip1.ColorHSV(pixelHue)));
    }
    
    
    for(int i=0; i<strip2.numPixels(); i++) { // For each pixel in strip...
      int pixelHue = firstPixelHue + ((i+irunner+150L) * 1636L / strip2.numPixels());
      strip2.setPixelColor(i, strip2.gamma32(strip2.ColorHSV(pixelHue)));
    }
    
    strip1.show(); // Update strip with new contents
    strip2.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
//



void ledRun(uint8_t wait) {
  float i, j, colorMode, div;
  byte fadeR, fadeG, fadeB;
  
  // To use the test fade mode, leave alwaysOn=0
  int alwaysOn=0;
  int colorFor=50; // How long it will be adding colors into the blend
  int fadeOutFor=150; // How long it will be adding 0's into the colors; to show the color fade out
  
  for(i=0;i< strip1.numPixels(); i++) {
    if(i == irunner%LED_COUNT){
      fadeR=255;
      fadeG=255;
      fadeB=255;
    }else{
      fadeR=0;
      fadeG=0;
      fadeB=0;
    }
    smoothRGB(i, &fadeR,&fadeG,&fadeB);
    strip1.setPixelColor(i, strip1.Color(  int(fadeR),   int(fadeG), int(fadeB)));
    strip2.setPixelColor(i, strip1.Color(  int(fadeR),   int(fadeG), int(fadeB)));
  }
  strip1.show();
  strip2.show();
  delay(wait);
}

void ledFader(uint8_t wait) {
  float i, j, colorMode, div;
  byte fadeR, fadeG, fadeB;
  
  // To use the test fade mode, leave alwaysOn=0
  int alwaysOn=0;
  int colorFor=50; // How long it will be adding colors into the blend
  int fadeOutFor=150; // How long it will be adding 0's into the colors; to show the color fade out
  
  for(i=0;i< strip1.numPixels(); i++) {
    if( int(floor(runner))%(colorFor+fadeOutFor) < colorFor || alwaysOn ){
      fadeR= max(0,min(mmRGB, (sin(i/(TAU)+sin(runner/35.515)*3.1415+sin(-runner/330.5)*30+sin(-runner/170.9)*25))*1.25*mmRGB ));
      fadeG= max(0,min(mmRGB, (cos(i/(TAU)-sin(-runner/25.58)*3.1415+sin(-runner/250.8)*30-cos(runner/102.2)*20))*1.25*mmRGB ));
      fadeB= max(0,min(mmRGB, (sin(i/(TAU)-cos(runner/50.45)*3.1415+sin(runner/300.1)*30-cos(-runner/158.3)*30))*1.25*mmRGB ));
    }else{
      fadeR=0;
      fadeG=0;
      fadeB=0;
    }      
    smoothRGB(i, &fadeR,&fadeG,&fadeB);
    strip1.setPixelColor(i, strip1.Color(  int(fadeR),   int(fadeG), int(fadeB)));
    strip2.setPixelColor(i, strip2.Color(  int(fadeR),   int(fadeG), int(fadeB)));
  }
  strip1.show();
  strip2.show();
  delay(wait);
}



////////////////////////////////////////////////////////////////////////////
// Blending Function                                                      //
////////////////////////////////////////////////////////////////////////////

// smoothRGB( # LED to blend in lightVals array, Red val, Green val, Blue val)
void smoothRGB(int i, byte *fadeR, byte *fadeG, byte *fadeB){
  float dispVal, curvVal;
  int x,c,divCount;
  int runAdd=1;
  int blender=I_BLEND_COUNT-1;
  runAdd=(blender-1);
  int prevRunAdd=runAdd;
  for(c=0; c<3; c++){
    runAdd=prevRunAdd;
    if(c==0){
      curvVal=max(0,min(mmRGB, (*fadeR) ));
    }else if(c==1){
      curvVal=max(0,min(mmRGB, (*fadeG) ));
    }else{
      curvVal=max(0,min(mmRGB, (*fadeB) ));
    }
    runAdd+=I_BLEND_COUNT;
    curvVal=(curvVal+lightVals[i][blender][c]*BLEND_COUNT)/(BLEND_COUNT+1);
    dispVal+=curvVal*runAdd;
    divCount=runAdd;
    lightVals[i][blender][c]=curvVal;
    for(x=0;x<blender-1;x++){
      runAdd=(x+1);
      divCount+=runAdd;
      curvVal=lightVals[min(blender-1,i+1)][x][c];
      curvVal=(curvVal+lightVals[i][x][c]*(BLEND_COUNT-1))/BLEND_COUNT;
      dispVal+=curvVal*runAdd;
      lightVals[i][x][c]=curvVal;
    }
    dispVal/=divCount;
    if(c==0){
      *fadeR=max(0,min(mmRGB, float(dispVal)));
    }else if(c==1){
      *fadeG=max(0,min(mmRGB, float(dispVal)));
    }else{
      *fadeB=max(0,min(mmRGB, float(dispVal)));
    }
  }
}
