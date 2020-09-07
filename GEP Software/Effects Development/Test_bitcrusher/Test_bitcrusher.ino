/************************************************
    Effect development template

    Simple template to use with the GEP to
    create and play with new effects. Provide
    input adjustment using the 2 pots and 2
    encoders to vary parameters.

    Version 1.0

    The read pots functions return a float from 0 to 1.0
    The read encoder functions return an int from -512 to +512, scale as needed

 ************************************************/


// paste output of GUI tool here
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=113,187
AudioEffectBitcrusher    bitcrusher1;    //xy=219,291
AudioMixer4              mixer1;         //xy=415,193
AudioOutputI2S           i2s2;           //xy=604,187
AudioConnection          patchCord1(i2s1, 0, mixer1, 0);
AudioConnection          patchCord2(i2s1, 1, bitcrusher1, 0);
AudioConnection          patchCord3(bitcrusher1, 0, mixer1, 1);
AudioConnection          patchCord4(mixer1, 0, i2s2, 0);
AudioConnection          patchCord5(mixer1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=109,55
// GUItool: end automatically generated code

#include "hardware.h"



void setup()
{
  Serial.println("Simple effect template");
  Serial.println("Audio input (left) goes to one input 1 on a mixer");
  Serial.println("A noise source goes to input 2 on the mixer.");
  Serial.println("Use the Mix pot to vary the audio in");
  Serial.println("Use the WahWah pot to vary the noise level");
  Serial.println("Substitute the noise effect for one of the several other");

  // initialize encoders
  paramEncoder.write(0);
  valueEncoder.write(0);


  // allocate memory to Teensy Audio
  AudioMemory(200);

  // enable the audio shield
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);        // headphone volume 0 to 1.0, nom = 0.5
  sgtl5000_1.lineOutLevel(20);   // line put level 13 (highest) to 31 (lowest). default 29

}



float val1, val2;
int val3, val4;


// loop forever, reading pots & encoders then setting effects
void loop()
{
  val1 = readMixPot();
  val2 = readWahPot();

  mixer1.gain(0, val1);
  mixer1.gain(1, val2);

  // read encoder and map to 1 to 16
  val3 = readParamEncoder();
  int bits = map(val3, -512, 512, 1, 16);
  Serial.print("bits = "); Serial.println(bits);
  bitcrusher1.bits(bits);

//  val4 = readValueEncoder();
//  int sampleRate = map(val4, -512, 512, 0, 44200);
//  sampleRate = constrain(sampleRate, 1, 44100);
//  Serial.print("sampleRate = "); Serial.println(sampleRate);
//  bitcrusher1.sampleRate(44100);




  // slow down loop
  delay(4000);
  Serial.println();
  printAudioMemUsage();
}
