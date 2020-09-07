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
AudioInputI2S            i2s1;           //xy=933.7646484375,188
AudioMixer4              mixer1;         //xy=1221.7646484375,207
AudioEffectDelayExternal delayExt1;      //xy=1221.823486328125,365.82362365722656
AudioOutputI2S           i2s2;           //xy=1698.7646484375,196
AudioConnection          patchCord1(i2s1, 0, mixer1, 0);
AudioConnection          patchCord2(mixer1, delayExt1);
AudioConnection          patchCord3(mixer1, 0, i2s2, 0);
AudioConnection          patchCord4(delayExt1, 0, mixer1, 1);
AudioConnection          patchCord5(delayExt1, 1, mixer1, 2);
AudioConnection          patchCord6(delayExt1, 2, mixer1, 3);
AudioControlSGTL5000     sgtl5000_1;     //xy=930.7646484375,64
// GUItool: end automatically generated code



#include "hardware.h"



void setup()
{
  Serial.println("Basic External Delay Test with 4 delaysn and feedback");
  Serial.println("Audio in left goes to mixer 1 channel 0");
  Serial.println("Mixer 1 is connect to mixer 3 ch 0 that drives L&R outputs");
  Serial.println("Mixer 1 output also is connected to the delayExt input");
  Serial.println("4 delay output channels are summed via mixer 2 inputs 0,1,2,3");
  Serial.println("The output of mixer 2 is connected to mixer 3 ch 1 and is feed back to");
  Serial.println("the delay input using mixer 1 ch 1");
  Serial.println("Default config is dry & 1 channel of delay with 250 ms delay, no feedback");
  Serial.println("Use the mix pot to adjust dry/wet mix");
  Serial.println("Use the wah-Wah pot to adjust delay");

  // initialize encoders (not used)
  paramEncoder.write(0);
  valueEncoder.write(0);

  
  // allocate memory to Teensy Audio
  AudioMemory(48);

  // enable the audio shield
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);        // headphone volume 0 to 1.0, nom = 0.5
  sgtl5000_1.lineOutLevel(29);   // line put level 13 to 31. default 29

  // mixer 1 - input & delay feedback
  mixer1.gain(0, 0.5);  // input at 50%
  mixer1.gain(1, 0);    // feeback off

  // mixer 2 - 4 channels of delay available
  mixer2.gain(0, 1.0);  // ch 0 on at 100%
  mixer2.gain(1, 0);    // ch 1 off
  mixer2.gain(2, 0);    // ch 2 off
  mixer2.gain(3, 0);    // ch 3 off

  // dry/wet mix
  mixer3.gain(0, 1.0);  // 100% dry
  mixer3.gain(1, 0.5);  // 50% wet

}



float val1, val2;


// loop forever, reading pots & setting effects
void loop()
{
  val1 = readMixPot();
  val2 = readWahPot();

  mixer3.gain(0, val1);         // 0 to 100%
  mixer3.gain(1, 1.0 - val1);   // 100% to 0

  // adjust delay ch 0 from 0 to 1.5 seconds
  delayext1.delay(0) = vals * 1.5;

  // slow down loop
  delay(100);
}
