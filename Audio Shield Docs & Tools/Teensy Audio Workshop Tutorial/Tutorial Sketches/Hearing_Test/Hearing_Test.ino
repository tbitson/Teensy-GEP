// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
//
// Part 1-2: Test Hardware
//
// Simple beeping is pre-loaded on the Teensy, so
// it will create sound and print info to the serial
// monitor when plugged into a PC.
//
// This program is supposed to be pre-loaded before
// the workshop, so Teensy+Audio will beep when
// plugged in.

/****
   connections
   Button 1 -> Pin 2 (D0)
   Button 2 -> Pin 3 (D1)
   Button 3 -> Pin 4 (D2)

   Button common -> Pin 1 (Ground)

   Pot 1 Wiper -> Pin 18 (A2)
   Pot 2 Wiper -> Pin 19 (A3)

   -- CW / right = increasing voltage:
   Pot 1 & 2 CW -> Pin 26 (+3.3V)
   Pot 1 & 2 CCW  -> Pin 1 (Ground)


*/

#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <Bounce.h>

AudioSynthWaveform    waveform1;
AudioOutputI2S        i2s1;
AudioConnection       patchCord1(waveform1, 0, i2s1, 0);
AudioConnection       patchCord2(waveform1, 0, i2s1, 1);
AudioControlSGTL5000  sgtl5000_1;

Bounce button0 = Bounce(0, 15);
Bounce button1 = Bounce(1, 15);
Bounce button2 = Bounce(2, 15);

int count = 1;
int a1history = 0, a2history = 0, a3history = 0;




void setup()
{
  AudioMemory(10);

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

  Serial.begin(57600);
  Serial.println("Version 1.1 for Dennis & Tim\n");

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.3);

  waveform1.begin(WAVEFORM_SINE);
  delay(1000);

  button0.update();
  button1.update();
  button2.update();

  a1history = analogRead(A1);
  a2history = analogRead(A2);
  a3history = analogRead(A3);

  Serial.println("Press Button 1 to start");
  while (wait(200) == false);
}






#define START_FREQ 	1000
#define END_FREQ 		18000
#define STEP_SIZE		100
#define ITERATIONS 	(END_FREQ - START_FREQ) / STEP_SIZE

int freqArray[ITERATIONS];
float ampArray[ITERATIONS];
int butArray[ITERATIONS];

int freq = START_FREQ;
float amp = 1.0;
int pos = 0;




void loop()
{
  Serial.print("Beep #");
  Serial.println(count);
  count = count + 1;

  Serial.print("Audio Freq: "); Serial.println(freq);
  Serial.print("Audo Amp  : "); Serial.println(amp, 2);

  waveform1.frequency(freq);
  waveform1.amplitude(amp);

  if (wait(500))
    butArray[pos] = 1;
  else
    butArray[pos] = 0;

  freqArray[pos] = freq;
  ampArray[pos] = amp;

  waveform1.amplitude(0);
  wait(1500);

  // now bump up freq
  freq += STEP_SIZE;
  pos++;

  if (freq > END_FREQ)
    quit();

  Serial.println();
}



void quit()
{
  for (int i = 0; i < ITERATIONS; i++)
  {
    Serial.print("Audio Freq  : "); Serial.print(freqArray[i]);
    Serial.print("  Audo Amp  : "); Serial.print(ampArray[i], 2);
    Serial.print("  Response  : "); Serial.println(butArray[i]);
  }


	Serial.println("\n\n\n");

	// excel export
    for (int i = 0; i < ITERATIONS; i++)
  {
    Serial.print(freqArray[i]);
    Serial.print(",");
    Serial.print(ampArray[i], 2);
    Serial.print(","); 
    Serial.println(butArray[i]); 
  }

  // loop forever
  while (1) ;
}





boolean wait(unsigned int milliseconds)
{
  elapsedMillis msec = 0;
  bool buttonPressed = false;

  while (msec <= milliseconds)
  {
    button0.update();

    if (button0.fallingEdge())
    {
      Serial.println("Button (pin 0) Pressed");
      buttonPressed = true;
    }
  }
  return buttonPressed;
}


