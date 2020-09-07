// guitar string demo

// Special thanks to Matthew Rahtz - http://amid.fish/karplus-strong/


void strum_up(const float *chord, float velocity);
void strum_dn(const float *chord, float velocity);

#include "chords.h"


#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>


AudioSynthKarplusStrong  string1;
AudioSynthKarplusStrong  string2;
AudioSynthKarplusStrong  string3;
AudioSynthKarplusStrong  string4;
AudioSynthKarplusStrong  string5;
AudioSynthKarplusStrong  string6;
AudioMixer4              mixer1;
AudioMixer4              mixer2;
AudioOutputI2S           i2s1;
AudioConnection          patchCord1(string1, 0, mixer1, 0);
AudioConnection          patchCord2(string2, 0, mixer1, 1);
AudioConnection          patchCord3(string3, 0, mixer1, 2);
AudioConnection          patchCord4(string4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, mixer2, 0);
AudioConnection          patchCord6(string5, 0, mixer2, 1);
AudioConnection          patchCord7(string6, 0, mixer2, 2);
AudioConnection          patchCord8(mixer2, 0, i2s1, 0);
AudioConnection          patchCord9(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;

const int finger_delay = 5;
const int hand_delay = 220;

int numChords = 8;
int chordNum = 1;


// converts strings to array
AudioSynthKarplusStrong strings[6] = {string1, string2, string3, string4, string5, string6};



void setup()
{
  AudioMemory(15);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.6);

  mixer1.gain(0, 0.15);
  mixer1.gain(1, 0.15);
  mixer1.gain(2, 0.15);
  mixer1.gain(3, 0.15);
  mixer2.gain(1, 0.15);
  mixer2.gain(2, 0.15);

  delay(700);
}




void loop()
{
  const float *chord;

  // each time through the loop, play a different chord
  switch (chordNum)
  {
    case 1:
      chord = Cmajor;
      Serial.println("C major");
      break;

    case 2:
      chord = Gmajor;
      Serial.println("G major");
      break;

    case 3:
      chord = Aminor;
      Serial.println("A minor");
      break;

    case 4:
      chord = Dminor;
      Serial.println("D minor");
      break;

    case 5:
      chord = Bminor;
      Serial.println("B minor");
      break;

    case 6:
      chord = Cmajor;
      Serial.println("C major");
      break;

    case 7:
      chord = Gmajor;
      Serial.println("G major");
      break;

    case 8:
      chord = Gmajor;
      Serial.println("G major");
      break;

    default:
      chord = 1;
      return;
  }

  // then strum the 6 string several times
  for (int i = 0; i < 2; i++)
  {
    strum_up(chord, 1.0);
    delay(hand_delay);
    delay(hand_delay);

    strum_up(chord, 1.0);
    delay(hand_delay);

    strum_dn(chord, 0.8);
    delay(hand_delay);
    delay(hand_delay);

    strum_dn(chord, 0.8);
    delay(hand_delay);

    strum_up(chord, 1.0);
    delay(hand_delay);

    strum_dn(chord, 0.8);
    delay(hand_delay);
  }

  chordNum++;
  if (chordNum > numChords)
  {
    chordNum = 1;
    Serial.print("Max CPU Usage = ");
    Serial.print(AudioProcessorUsageMax(), 1);
    Serial.println("%");
    Serial.println();
  }
}


//void strum_up(const float * chord, float velocity)
//{
//  for (int i = 0; i < 6; i++)
//  {
//    strings[i].noteOn(chord[i], velocity);
//    delay(finger_delay);
//  }
//}



void strum_up(const float * chord, float velocity)
{
  string1.noteOn(chord[0], velocity);
  delay(finger_delay);

  string2.noteOn(chord[1], velocity);
  delay(finger_delay);

  string3.noteOn(chord[2], velocity);
  delay(finger_delay);

  string4.noteOn(chord[3], velocity);
  delay(finger_delay);

  string5.noteOn(chord[4], velocity);
  delay(finger_delay);

  string6.noteOn(chord[5], velocity);
  delay(finger_delay);
}



void strum_dn(const float * chord, float velocity)
{
  string1.noteOn(chord[5], velocity);
  delay(finger_delay);

  string2.noteOn(chord[4], velocity);
  delay(finger_delay);

  string3.noteOn(chord[3], velocity);
  delay(finger_delay);

  string4.noteOn(chord[2], velocity);
  delay(finger_delay);

  string5.noteOn(chord[1], velocity);
  delay(finger_delay);

  string6.noteOn(chord[0], velocity);
  delay(finger_delay);
}
