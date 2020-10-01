/************************************************
    8-channel mixer test

    This tests the added 8-channel mixer

    Version 1.0.1

    The read pots functions return 0 to 1.0
    The read encoder functions return an int from -512 to +512

 ************************************************/


// paste output of GUI tool here

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=240.8235321044922,51.82353210449219
AudioOutputI2S           i2s2;           //xy=681.8235626220703,263.82354736328125
AudioMixer8              mixer8_1;       //xy=455.8235626220703,262.82359313964844
AudioSynthWaveformSine   sine1;          //xy=157.8235321044922,108.82353210449219
AudioSynthWaveformSine   sine2;          //xy=157.8235321044922,154.8235321044922
AudioSynthWaveformSine   sine3;          //xy=161.8235321044922,204.8235321044922
AudioSynthWaveformSine   sine4;          //xy=159.8235321044922,254.82354736328125
AudioSynthWaveformSine   sine5;          //xy=158.8235321044922,306.8235626220703
AudioSynthWaveformSine   sine6;          //xy=158.8235321044922,362.8235626220703
AudioSynthWaveformSine   sine7;          //xy=158.8235321044922,414.8235626220703
AudioSynthWaveformSine   sine8;          //xy=160.8235321044922,467.8235626220703
AudioConnection          patchCord1(sine1, 0, mixer8_1, 0);
AudioConnection          patchCord2(sine2, 0, mixer8_1, 1);
AudioConnection          patchCord3(sine3, 0, mixer8_1, 2);
AudioConnection          patchCord4(sine4, 0, mixer8_1, 3);
AudioConnection          patchCord5(sine5, 0, mixer8_1, 4);
AudioConnection          patchCord6(sine6, 0, mixer8_1, 5);
AudioConnection          patchCord7(sine7, 0, mixer8_1, 6);
AudioConnection          patchCord8(sine8, 0, mixer8_1, 7);
AudioConnection          patchCord9(mixer8_1,  0, i2s2, 0);
AudioConnection          patchCord10(mixer8_1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=282.8235321044922,557.8235321044922
// GUItool: end automatically generated code




// globals
float baseFreq = 25.0;
int channel = 0;



void setup()
{
  Serial.println("8-Channel Mixer Test");

  // allocate memory to Teensy Audio
  AudioMemory(48);

  // enable the audio shield
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);        // headphone volume 0 to 1.0, nom = 0.5
  sgtl5000_1.lineOutLevel(29);   // line put level 13 to 31. default 29

  delay(1000);

  // set all mixer channels off
  for (uint8_t i = 0; i < 8; i++)
    mixer8_1.gain(i, 0);

  // generate a sine for each mixer input (8 octaves of A)
  sine1.amplitude(0.5);
  sine1.frequency(baseFreq);
  sine2.amplitude(0.5);
  sine2.frequency(baseFreq * 2);
  sine3.amplitude(0.5);
  sine3.frequency(baseFreq * 4);
  sine4.amplitude(0.5);
  sine4.frequency(baseFreq * 8);
  sine5.amplitude(0.5);
  sine5.frequency(baseFreq * 16);
  sine6.amplitude(0.5);
  sine6.frequency(baseFreq * 32);
  sine7.amplitude(0.5);
  sine7.frequency(baseFreq * 64);
  sine8.amplitude(0.5);
  sine8.frequency(baseFreq * 128);

  delay(1000);
}





void loop()
{
  // set all channels off
  for (uint8_t i = 0; i < 8; i++)
    mixer8_1.gain(i, 0);

  Serial.print("Ramping up Channel ");
  Serial.println(channel);
  Serial.print("Channel Freq = ");
  Serial.println(baseFreq * pow(2, channel));

  // ramp up the channel under test
  for (float i = 0; i <= 1.0; i += 0.01)
  {
    mixer8_1.gain(channel, i);
    delay(50);
  }

  // leave it on for a bit
  delay(1000);

  // ramp down the channel under test
  for (float i = 1.0; i > 0; i -= 0.01)
  {
    mixer8_1.gain(channel, i);
    delay(50);
  }
  
  Serial.println("channel off");
  
  // check for silence
  delay(2000);

  // sequence to next channel
  channel++;


  if (channel > 7)
  {
    Serial.println("Testing All Channels");

    // turn each channel on in sucession
    for (uint8_t i = 0; i < 8; i++)
    {
      mixer8_1.gain(i, 1.0);
      delay(1000);
    }
    // probably sounds a mess
    delay(2000);

    // turn each channel off in sucession
    for (uint8_t i = 0; i < 8; i++)
    {
      mixer8_1.gain(i, 0);
      delay(1000);
    }

    // start over
    channel = 0;
  }
}
