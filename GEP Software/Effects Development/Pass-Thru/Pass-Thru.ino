/************************************************
 *  Effect development template
 *  
 *  Simple template to use with the GEP to
 *  create and play with new effects. Provide
 *  input adjustment using the 2 pots and 2 
 *  encoders to vary parameters.
 *  
 *  Version 1.0.1
 *  
 *  The read pots functions return 0 to 1.0
 *  The read encoder functions return an int from -512 to +512
 *  
 ************************************************/


// paste output of GUI tool here
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>


// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=191,166
AudioOutputI2S           i2s2;           //xy=558,163
AudioConnection          patchCord1(i2s1, 0, i2s2, 0);
AudioConnection          patchCord2(i2s1, 1, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=192,221
// GUItool: end automatically generated code





void setup()
{
  Serial.begin(57600);
  delay(1000);
  
  Serial.println("Pass-thru - simple test to see if audio is working");
  Serial.println("Stereo audio is routed directly from input to output");
  
  // allocate memory to Teensy Audio
  AudioMemory(10);

  // enable the audio shield
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);        // headphone volume 0 to 1.0, nom = 0.5
  sgtl5000_1.lineOutLevel(29);   // line put level 13 to 31. default 29
}



void loop()
{
  // just loop...

  // slow down loop
  delay(100);
}
