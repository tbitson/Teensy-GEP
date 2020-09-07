/*
 * basic teensy audio library sketch
 * 
 * TAL_Test version 1.0
 * 
 * 
 */


// GUItool: begin automatically generated code
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2sIn;          //xy=87,50
AudioMixer4              mixer1;         //xy=273,43
AudioEffectDelay         delay1;         //xy=658,183
AudioMixer4              mixer2;         //xy=1029,66
AudioOutputI2S           i2sOut;         //xy=1224,62
AudioConnection          patchCord1(i2sIn, 0, mixer1, 0);
AudioConnection          patchCord2(i2sIn, 1, mixer1, 1);
AudioConnection          patchCord3(mixer1, 0, mixer2, 0);
AudioConnection          patchCord4(mixer1, delay1);
AudioConnection          patchCord5(delay1, 0, mixer2, 1);
AudioConnection          patchCord6(delay1, 1, mixer2, 2);
AudioConnection          patchCord7(delay1, 2, mixer2, 3);
AudioConnection          patchCord8(mixer2, 0, i2sOut, 0);
AudioConnection          patchCord9(mixer2, 0, i2sOut, 1);
AudioControlSGTL5000     audioShield;     //xy=124,294
// GUItool: end automatically generated code


// define some basic settings
#define HEADPHONE_OUTPUT_LEVEL 0.7
#define LINEOUT_AUDIO_LEVEL    8


// globals
uint32_t loopCount;
float audioDelay = 50.0;
float inputValueAdj = 0.5;




void setup()
{
  Serial.begin(57600);
  AudioMemory(80);
  delay(2000);
  Serial.println("Teensy Audio Library Delay Test v1.0");


  // Enable the audio shield and set the output volume.
  Serial.println("Enabling audio module");
  audioShield.enable();                              // start audio board
  
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);       // use the line-in input
  audioShield.volume(HEADPHONE_OUTPUT_LEVEL);        // headphone output level
  audioShield.lineInLevel(inputValueAdj);            // input level
  audioShield.lineOutLevel(LINEOUT_AUDIO_LEVEL);     // 3.16 V p-p out
  audioShield.audioPostProcessorEnable();            // enable SGTL5000 effects processing
  audioShield.adcHighPassFilterDisable();            // Turn off ADC HP filter

  // Configure our effects
  delay1.delay(0, 100);
  delay1.delay(1, 500);
  delay1.delay(2, 2000);


  // mix left & right channels
  mixer1.gain(0, 1.0); // left input 
  mixer1.gain(1, 1.0); // right input

  // input 0 is 'dry' input 1 is with reverb
  mixer2.gain(0, 1.0);
  mixer2.gain(1, 0.0);
  mixer2.gain(2, 0.0);
  mixer2.gain(3, 0.0);

  delay(2000);
}



bool delayOn = false;


void loop()
{

  if (millis() % 10000 == 0)
  {
    if (delayOn == false)
    {
      mixer2.gain(0, 0.5);
      mixer2.gain(3, 1.0);
      Serial.println("with delay");
      delayOn = true;
    }
    else
    {
      mixer2.gain(0, 0.5);
      mixer2.gain(3, 0.0);
      Serial.println("without delay");
      delayOn = false;
    }
    // keep from entering loop too fast
    delay(10);
  }

  if (millis() % 24000 == 0)
  {
    Serial.println("=== Audio Mem Usage ===");
    Serial.print("AudioProcessor: "); Serial.print(AudioProcessorUsage());
    Serial.print("\tmax: "); Serial.print(AudioProcessorUsageMax());
    Serial.print("\tAudioMem: = "); Serial.print(AudioMemoryUsage());
    Serial.print("\tmax: "); Serial.print(AudioMemoryUsageMax());
    //Serial.print("\tanalogDelay: ");
    //Serial.print(analogDelay.processorUsage());
    Serial.println();

    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();
  }

  loopCount++;
}
