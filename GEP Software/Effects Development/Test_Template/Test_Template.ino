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
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=112,179
AudioSynthNoiseWhite     noise1;         //xy=117,247
AudioMixer4              mixer1;         //xy=338,210
AudioOutputI2S           i2s2;           //xy=604,187
AudioConnection          patchCord1(i2s1, 0, mixer1, 0);
AudioConnection          patchCord2(noise1, 0, mixer1, 1);
AudioConnection          patchCord3(mixer1, 0, i2s2, 0);
AudioConnection          patchCord4(mixer1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=109,55
// GUItool: end automatically generated code


#include "hardware.h"



void setup()
{
  Serial.println("Simple effect template");
  Serial.println("Audio in left goes to one input 1 on a mixer");
  Serial.println("A noise source goes to input 2 on the mixer.");
  Serial.println("Use the Mix pot to vary the audio in");
  Serial.println("Use the WahWah pot to vary the noise level");

  // initialize encoders
  paramEncoder.write(0);
  valueEncoder.write(0);

  
  // allocate memory to Teensy Audio
  AudioMemory(10);

  // enable the audio shield
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);        // headphone volume 0 to 1.0, nom = 0.5
  sgtl5000_1.lineOutLevel(29);   // line put level 13 to 31. default 29

  // the noise source has its own level control
  noise1.amplitude(0.5);
}



float val1, val2, val3, val4;


// loop forever, reading pots & encoders then setting effects
void loop()
{
  val1 = readMixPot();
  val2 = readWahPot();

  mixer1.gain(0, val1);
  mixer1.gain(1, val2);

  // slow down loop
  delay(100);
}
