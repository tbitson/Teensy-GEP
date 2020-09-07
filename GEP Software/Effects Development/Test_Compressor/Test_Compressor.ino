/* DAP AVC example; AVC is SGTL5000 equiv of AGC


  // configure the compressor with current settings
  // gain: 0 = 0dB, 1 = 6dB, 2 = 12dB
  // response (integration time): 0 = 0ms, 1 = 25ms, 2 = 50 ms, 3 = 100 ms (values > 3 permissible)
  // hardLimit: 0 = use "soft knee', 1 = hard limit (don't allow values > threshold)
  // threshold: 0 to -96 in dBFS
  // attack: controls decrease in gain in dB per second  (0 to 1.0 ???)
*/


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=137.8235321044922,65.82353210449219
AudioAnalyzePeak         peak1;          //xy=405.8235321044922,11.823532104492188
AudioOutputI2S           i2s2;           //xy=519.8235321044922,70.82353210449219
AudioConnection          patchCord1(i2s1, 0, i2s2, 0);
AudioConnection          patchCord2(i2s1, 0, peak1, 0);
AudioConnection          patchCord3(i2s1, 1, i2s2, 1);
AudioControlSGTL5000     audioShield;    //xy=156.8235626220703,143
// GUItool: end automatically generated code



// default values
bool enabled = false;
int gain = 2;
int response = 1;
int limit = 0;
float threshold = -18;
float attack = 0.5;
float decay = 0.5;
uint8_t lineInLevel = 8;




void setup()
{
  Serial.begin(57600);
  delay(2000);
  Serial.println("Compressor Test");


  // allocate memory to audio system
  AudioMemory(20);

  // Enable the audio shield and set the input &  output levels
  audioShield.enable();
  delay(1000);
  audioShield.inputSelect(AUDIO_INPUT_LINEIN);
  audioShield.volume(0.7);

  //0 = 3.12,  15 = 0.24 vpp
  // lower = more sensitive;
  audioShield.lineInLevel(lineInLevel);

  // enable processing
  audioShield.audioPostProcessorEnable();
  audioShield.audioPreProcessorEnable();

  // load the defaults
  audioShield.autoVolumeControl(gain, response, limit, threshold, attack, decay);
  audioShield.autoVolumeDisable();

  Serial.println("Compressor disabled");
  Serial.println("Uses serial commands to ajust parameters");
  Serial.println("ready");
  Serial.println("Enter '?' for help");

}




void loop()
{
  // check for incoming data on serial port
  if (Serial.available())
  {
    // read 1st character, preserving the rest
    char c = Serial.read();

    // filter out non-printable characters
    if (c > 32 && c < 127)
    {
      Serial.print("Serial Cmd ->"); Serial.println(c);
      Serial.println();

      switch (c)
      {
        case 'g':
          gain += 1;
          if (gain > 2)
            gain = 0;
          break;

        case 'r':
          response += 1;
          if (response > 3)
            response = 0;
          break;


        case 'l':
          if (limit == 0)
            limit = 1;
          else
            limit = 0;
          break;


        case 't':
          threshold -= 5;
          if (threshold < -96)
            threshold = -96;
          break;

        case 'T':
          threshold += 5;
          if (threshold > 0)
            threshold = 0;
          break;

        case 'a':
          attack += 0.1;
          attack = constrain(attack, 0, 5);
          break;

        case 'A':
          attack -= 0.1;
          attack = constrain(attack, 0, 5);
          break;

        case 'd':
          decay += 0.1;
          decay = constrain(decay, 0, 5);
          break;

        case 'D':
          decay -= 0.1;
          decay = constrain(decay, 0, 5);
          break;

        case '0':
          audioShield.autoVolumeDisable();
          Serial.println("Comp OFF");
          enabled = false;
          break;

        case '1':
          audioShield.autoVolumeEnable();
          Serial.println("Comp ON");
          enabled = true;
          break;

        case '+':
          lineInLevel += 1;
          lineInLevel = constrain(lineInLevel, 0, 15);
          audioShield.lineInLevel(lineInLevel);
          break;

        case '-':
          lineInLevel -= 1;
          lineInLevel = constrain(lineInLevel, 0, 15);
          audioShield.lineInLevel(lineInLevel);
          break;

        case '?':
          Serial.println("0 : compressor disable");
          Serial.println("1 : compressor enable");
          Serial.println("g : gain adjust");
          Serial.println("r : response adjust");
          Serial.println("l : limit type toggle");
          Serial.println("t : threshold increase (-5)");
          Serial.println("T : threshold decrease (+5)");
          Serial.println("a : attack +");
          Serial.println("A : attack -");
          Serial.println("d : decay +");
          Serial.println("d : decay -");
          Serial.println("+ : line in sensitivity increase");
          Serial.println("- : line in sensitivity decrease");
          Serial.println();
          break;

        default:
          Serial.println("I'm sorry Dave, I didn't get that");
      }

      Serial.print("status    = "); Serial.println(enabled);
      Serial.print("gain      = "); Serial.println(gain);
      Serial.print("response  = "); Serial.println(response);
      Serial.print("limit     = "); Serial.println(limit);
      Serial.print("threshold = "); Serial.println(threshold);
      Serial.print("attack    = "); Serial.println(attack);
      Serial.print("decay     = "); Serial.println(decay);
      Serial.print("level     = "); Serial.println(lineInLevel);
      Serial.println();

      audioShield.autoVolumeControl(gain, response, limit, threshold, attack, decay);
    }
    else
      delay(200);
  }

  if (peak1.available())
  {
    Serial.print("Vpp = ");
    Serial.println(peak1.readPeakToPeak());
    delay(500);
  }
}
