/*
 * Teensy Audio Libray Test - basic example
 *  using built-in freeverb
 *  teensy 3.2: 12% prog mem, 75% ram
 *  teensy 4.0:  1% prog mem, 7% ram
 *  teensy 3.6:  3% prog mem, 19% ram
 *   v 1.01
 * 
 */

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>


// GUItool: begin automatically generated code
AudioInputI2S            i2sIn;          //xy=87,50
AudioMixer4              mixer1;         //xy=273,43
AudioAmplifier           amp2;           //xy=441,134
AudioEffectFreeverb      freeverb1;         //xy=618,134
AudioAmplifier           amp1;           //xy=778,136
AudioMixer4              mixer2;         //xy=1029,66
AudioOutputI2S           i2sOut;         //xy=1372,80
AudioConnection          patchCord1(i2sIn, 0, mixer1, 0);
AudioConnection          patchCord2(i2sIn, 1, mixer1, 1);
AudioConnection          patchCord3(mixer1, 0, mixer2, 0);
AudioConnection          patchCord4(mixer1, amp2);
AudioConnection          patchCord5(amp2, freeverb1);
AudioConnection          patchCord6(freeverb1, amp1);
AudioConnection          patchCord7(amp1, 0, mixer2, 1);
AudioConnection          patchCord8(mixer2, 0, i2sOut, 0);
AudioConnection          patchCord9(mixer2, 0, i2sOut, 1);
AudioControlSGTL5000     audioShield;    //xy=97,685
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

  delay(100);
  AudioMemory(64);


  // Enable the audio shield and set the output volume.
  Serial.println("Enabling audio module");
  
  audioShield.enable();                              // start audio board
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);       // use the line-in input
  audioShield.volume(HEADPHONE_OUTPUT_LEVEL);        // headphone output level
  audioShield.lineInLevel(inputValueAdj);            // input level
  audioShield.lineOutLevel(LINEOUT_AUDIO_LEVEL);     // 3.16 V p-p out
  //audioShield.audioPostProcessorEnable();            // enable SGTL5000 effects processing
  //audioShield.adcHighPassFilterDisable();            // Turn off ADC HP filter

  // Configure our effects
  // lower input into reverb, gains adjusted below to compensate
  amp2.gain(0.25);
  freeverb1.roomsize(0.7);
  freeverb1.damping(0.5);

  // mx left & right channels
  mixer1.gain(0, 1.0); // left input 
  mixer1.gain(1, 1.0); // right input

  // input 0 is 'dry' input 1 is with reverb
  mixer2.gain(0, 0.5);
  mixer2.gain(1, 0.0);

  // increase reverb output
  amp1.gain(4.0);

  delay(2000);
}



bool delayOn = false;


void loop()
{

  if (millis() % 10000 == 0)
  {
    if (delayOn == false)
    {
      mixer2.gain(0, 0);
      mixer2.gain(1, 1.0);
      Serial.println("with reverb");
      delayOn = true;
    }
    else
    {
      mixer2.gain(0, 0.5);
      mixer2.gain(1, 0.0);
      Serial.println("without reverb");
      delayOn = false;
    }
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
