// delay test


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>



// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=158,74
AudioEffectEnvelope      envelope1;      //xy=232,156
AudioEffectDelay         delay1;         //xy=393,238
AudioMixer4              mixer1;         //xy=532,205
AudioOutputI2S           i2s1;           //xy=611,61
AudioConnection          patchCord1(sine1, envelope1);
AudioConnection          patchCord2(envelope1, 0, i2s1, 0);
AudioConnection          patchCord3(envelope1, 0, i2s1, 1);
AudioControlSGTL5000     audioControl;     //xy=195,272
// GUItool: end automatically generated code



void setup()
{
  Serial.println("Super simple test tone generator");
  Serial.println("Generates tone on L + R channels");

  // allocate memory
  AudioMemory(10);

  // enable the audio shield
  audioControl.enable();
  audioControl.volume(0.5);        // 0 to 1.0, nom = 0.5
  audioControl.lineOutLevel(29);   // 13 to 31. default 29

  // generate sine wave, use envelop to gate
  sine1.frequency(1000);
  sine1.amplitude(0.5);
}



void loop()
{
  // play note
  Serial.println("Playing Note");
  envelope1.noteOn();
  delay(36);
  envelope1.noteOff();

  delay(500);

  // play note
  envelope1.noteOn();
  delay(36);
  envelope1.noteOff();
  delay(500);
  
  printAudioMemUsage();

  delay(5000);
}




void printAudioMemUsage()
{
  Serial.print("AudioProcessor: "); Serial.print(AudioProcessorUsage());
  Serial.print("  max: ");          Serial.println(AudioProcessorUsageMax());
  Serial.print("AudioMem      : "); Serial.print(AudioMemoryUsage());
  Serial.print("  max: ");          Serial.println(AudioMemoryUsageMax());
  Serial.println();
}
