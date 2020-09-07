/************************************************
    Effect development template

    Simple template to use with the GEP to
    create and play with new effects. Provide
    input adjustment using the 2 pots and 2
    encoders to vary parameters.

    Version 1.0

    The read pots functions return 0 to 1.0
    The read encoder functions return -512.0 to +512.0, scale as needed

 ************************************************/


// paste output of GUI tool here

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=112,179
AudioEffectWaveshaper    waveshape1;     //xy=186,328
AudioMixer4              mixer1;         //xy=338,210
AudioOutputI2S           i2s2;           //xy=604,187
AudioConnection          patchCord1(i2s1, 0, mixer1, 0);
AudioConnection          patchCord2(i2s1, 0, waveshape1, 0);
AudioConnection          patchCord3(waveshape1, 0, mixer1, 1);
AudioConnection          patchCord4(mixer1, 0, i2s2, 0);
AudioConnection          patchCord5(mixer1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=109,55
// GUItool: end automatically generated code



#include "hardware.h"



void setup()
{
  Serial.println("distortion effect using wavetable");
  Serial.println("Audio input (left) goes to waveshape effect");
  Serial.println("MixPot adjusts Dry audio level");
  Serial.println("WahPot adjusts Wet audio level");
  Serial.println("Alter the shape array in the code");


  // initialize encoders
  paramEncoder.write(0);
  valueEncoder.write(0);


  // allocate memory to Teensy Audio
  AudioMemory(20);

  // enable the audio shield
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);        // headphone volume 0 to 1.0, nom = 0.5
  sgtl5000_1.lineOutLevel(20);   // line put level 13 (highest) to 31 (lowest). default 29

  // ref from docs:
  // shape(array, length);
  // Configure the waveform shape. Array is a list of float numbers, given in order.
  // The first number maps to input -1.0. The last maps to input +1.0.
  // The numbers represent the desired output level at each of these input levels.
  // Length must be 2, 3, 5, 9, 17, 33, 65, 129, 257, 513, 1025, 2049, 4097, 8193, 16385, or 32769.

  // test hard clipping at 50%
  const int len = 17;
  float arr[len] = { -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5 };
  waveshape1.shape(arr, len);
}



float val1, val2;
int val3, val4;


// loop forever, reading pots & encoders then setting effects
void loop()
{
  val1 = readMixPot();
  val2 = readWahPot();

  Serial.print("Dry = "); Serial.println(val1, 1);
  Serial.print("Wet = "); Serial.println(val2, 1);

  mixer1.gain(0, val1);
  mixer1.gain(1, val2);

  // slow down loop
  delay(100);
}
