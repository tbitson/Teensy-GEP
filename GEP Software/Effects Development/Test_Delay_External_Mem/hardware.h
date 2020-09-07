
#include <Encoder.h>



// rotary encoder pins
#define ENCODER1_PIN1   1
#define ENCODER1_PIN2   2
#define ENCODER2_PIN1   4
#define ENCODER2_PIN2   5

// on-board potentiometers - could be pedal inputs
#define WAH_WAH_POT   A1
#define MIX_POT       A2


// create instances of rotory encoders
Encoder valueEncoder(ENCODER1_PIN1, ENCODER1_PIN2);
Encoder paramEncoder(ENCODER2_PIN1, ENCODER2_PIN2);


float readMixPot()
{
  float pot;

  pot = (float)analogRead(MIX_POT);
  pot /= 1023.0;
  pot = constrain(pot, 0, 1.0);
  Serial.print("Mix Pot = "); Serial.println(pot, 2);
  return pot;
}


float readWahPot()
{
  float pot;

  pot = (float)analogRead(WAH_WAH_POT);
  pot /= 1023.0;
  pot = constrain(pot, 0, 1.0);
  Serial.print("Wah Pot = "); Serial.println(pot, 2);
  return pot;
}







int readParamEncoder()
{
  int encoder;

  // read encoder & divide results by 4 to slow down
  encoder = paramEncoder.read();
  encoder = constrain(encoder, -512, 512);
  Serial.print("Param Encoder = "); Serial.println(encoder);

  return encoder;
}



int readValueEncoder()
{
  int encoder;

  // read encoder & divide results by 4 to slow down
  encoder = valueEncoder.read();
  encoder = constrain(encoder, -512, 512);
  Serial.print("Value Encoder = "); Serial.println(encoder);

  return encoder;
}




void printAudioMemUsage()
{
  Serial.print("AudioProcessor: "); Serial.print(AudioProcessorUsage());
  Serial.print("  max: ");          Serial.println(AudioProcessorUsageMax());
  Serial.print("AudioMem      : "); Serial.print(AudioMemoryUsage());
  Serial.print("  max: ");          Serial.println(AudioMemoryUsageMax());
  Serial.println();
}
