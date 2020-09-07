// delay demo using external memory
//
// Requires added optional SRAM memory
// IC install on Audio Board



#include "hardware.h"

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>



// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=144.2857208251953,112.85714721679688
AudioEffectEnvelope      envelope1;      //xy=218.2857208251953,194.85714721679688
AudioEffectDelayExternal delayExt1;      //xy=373,349
AudioMixer4              mixer1;         //xy=579.2857055664062,267.8571472167969
AudioOutputI2S           i2s1;           //xy=597.2857208251953,99.85714721679688
AudioConnection          patchCord1(sine1, envelope1);
AudioConnection          patchCord2(envelope1, 0, i2s1, 0);
AudioConnection          patchCord3(envelope1, delayExt1);
AudioConnection          patchCord4(delayExt1, 0, mixer1, 0);
AudioConnection          patchCord5(delayExt1, 1, mixer1, 1);
AudioConnection          patchCord6(delayExt1, 2, mixer1, 2);
AudioConnection          patchCord7(mixer1, 0, i2s1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=181.2857208251953,310.8571472167969
// GUItool: end automatically generated code


// Use these with the audio adaptor board
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14


// globals
float delayms;
float lastPot;



void setup()
{
  Serial.println("Demo of delay using external memory. A single note is");
  Serial.println("played and a 3 equal-length delays are applied with dimishing");
  Serial.println("volume. Use the Mix Pot to adjust delay");
  Serial.println();

  // allocate enough memory for the delay
  AudioMemory(140);

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);

  // enable the audio shield
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);


  // configure a sine wave for the chirp
  // the original is turned on/off by an envelope effect
  // and output directly on the left channel
  sine1.frequency(500);
  sine1.amplitude(0.5);

  // set default delay
  delayms = 100;
  setDelay(delayms);


}



void setDelay(uint16_t delayms)
{
  // create 3 delay taps, which connect through a
  // mixer to the right channel output

  Serial.print("base delay = ");
  Serial.println(delayms);

  delayExt1.delay(0, delayms);
  delayExt1.delay(1, delayms * 2);
  delayExt1.delay(2, delayms * 3);

    // set volume levels of delays
  mixer1.gain(0, 1.0);   // delay 1
  mixer1.gain(1, 0.8);   // delay 2
  mixer1.gain(2, 0.6);   // delay 3
}


float val1, val2;




void loop()
{

  // read pot for about 5 seconds
  // pot value ranges 0 to 1023, scale to 50 to 2000 ms
  for (uint16_t loops = 0; loops < 50; loops++)
  {
    val1 = readMixPot;

    // don't update on small changes (read noise)
    if (abs(pot - lastPot) > 2)
    {
    // max delay for ext memory is ~1.8 seconds total
    // with 3 channels, it will be less but gonna allow 2 seconds
      pot *= 2000.0
      delayms = constrain(pot, 10, 2000);
      lastPot = pot;

      Serial.print("base delay = ");
      Serial.println(delayms);
    }
    delay(100);
  }

  // set the desired delay values
  setDelay(delayms);
  delay(100);

  // play note
  Serial.println("Playing Note");
  envelope1.noteOn();
  delay(36);
  envelope1.noteOff();

  delay(4000);

    printAudioMemUsage();
}



// did we provide sufficient memory?
void printAudioMemUsage()
{
  Serial.print("AudioProcessor: "); Serial.print(AudioProcessorUsage());
  Serial.print("  max: ");          Serial.println(AudioProcessorUsageMax());
  Serial.print("AudioMem      : "); Serial.print(AudioMemoryUsage());
  Serial.print("  max: ");          Serial.println(AudioMemoryUsageMax());
  Serial.println();
}
