/*

     GEP basic Hardware Test

     Vers 1.0


********************************************************************************************************************/

const bool DEBUG_MODE = true;

#include <ILI9341_t3.h>
#include <font_Arial.h>
#include <XPT2046_Touchscreen.h>
#include <Encoder.h>
#include <Bounce.h>
#include <EEPROM.h>

#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>

#include <PCF8574.h>


#define ON  1
#define OFF 0

#define EEPROM_START_ADDR 0
#define EEPROM_SIZE       64
#define EEPROM_VALID      0x33



// TFT pins
const uint8_t TFT_DC = 20;
const uint8_t TFT_CS = 21;
const uint8_t TFT_RST = 3;
const uint8_t TFT_MOSI = 7;
const uint8_t TFT_SCLK = 14;
const uint8_t TFT_MISO = 12;

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);


//#define CS_PIN  8  // touchscreen -CS line
//XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - NULL - No interrupts or use TIRQ_PIN for 2nd parameter

#define ENCODER1_PIN1   1
#define ENCODER1_PIN2   2
#define ENCODER2_PIN1   4
#define ENCODER2_PIN2   5
Encoder parameterEncoder(ENCODER2_PIN1, ENCODER2_PIN2);
Encoder valueEncoder(ENCODER1_PIN1, ENCODER1_PIN2);


// hard-wired switches
#define Tuner_Switch_Pin 0
#define Save_Switch_Pin 17          // A3
#define Compressor_Switch_Pin 10   // D10

// de-bounce the switches
Bounce Tuner_Switch = Bounce(Tuner_Switch_Pin, 15);
Bounce Save_Switch = Bounce(Save_Switch_Pin, 15);


// instantiate the PCF8574 class for effect switches and LEDs
#define PCF8574_ADDR  0x20
PCF8574 PCF(PCF8574_ADDR);

// PCF8574 LEDs
#define LED_ON      0
#define LED_OFF     1

// PCF8574 LEDs
#define REVERB_LED      7
#define FLANGER_LED     6
#define TREMOLO_LED     5
#define WAH_WAH_LED     4

// PCF8574 Switches

#define REVERB_SWITCH   0
#define FLANGER_SWITCH  1
#define TREMOLO_SWITCH  2
#define WAH_WAH_SWITCH  3


#define WAH_WAH_POT   A1
#define BALANCE_POT   A2

// globals
bool keepLooping;


// ---------------------------------------------------------------------------



void setup()
{
  Serial.begin(57600);
  delay(2000);
  Serial.println("Teensy GEP Hardware Test Program");
  Serial.println();

  pinMode(Tuner_Switch_Pin, INPUT_PULLUP);
  pinMode(Save_Switch_Pin, INPUT_PULLUP);
  pinMode(Compressor_Switch_Pin, INPUT_PULLUP);

  // init PCF8574 with LEDS off, all switches pulled up
  PCF.begin(0xff);

  // clear lcd display
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  //Touchscreen controller startup
  // ts.begin();

  readEEPROM();
  // eraseEEPROM;
  // readEEPROM();
  
  delay(1000);
}



void loop()
{
  testSwitches();
  delay(1000);

  testLEDs();
  delay(1000);

  testPushbuttons();
  delay(1000);

  Serial.println("Testing Effects Pot");
  testPot(BALANCE_POT);
  delay(1000);

  Serial.println("Testing Wah-Wah Pot");
  testPot(WAH_WAH_POT);
  delay(1000);

  Serial.println("Rotary Encoder 1 Test");
  testEncoder(1);
  delay(1000);

  Serial.println("Rotary Encoder 2 Test");
  testEncoder(2);
  delay(1000);

  Serial.println("*** Tests complete ***");
  delay(10000);
}


// ----------------------------------------------------


void readEEPROM()
{
  // read EEPROM - Teensy 3.2 has 2048 bytes
  EEPROM.begin();

  uint8_t val;

  Serial.println("Reading EEPROM values");
  for (uint8_t i = EEPROM_START_ADDR; i < EEPROM_SIZE; i++)
  {
    val = EEPROM.read(i);
    Serial.print("EEProm [");
    Serial.print(i);
    Serial.print("] = 0X");
    Serial.println(val, HEX);
    delay(10);
  }

  Serial.println("done");
}



void eraseEEPROM()
{
  Serial.println("erasing EEPROM");
  // write fixed pattern out
  for (uint8_t i = EEPROM_START_ADDR; i < EEPROM_SIZE; i++)
  {
    EEPROM.write(i, 0x00);
    Serial.println(i);
    delay(10);
  }
  Serial.println("done");
}



void testSwitches()
{

  Serial.println("Toggle Tuner, Save, and Compressor switches");
  Serial.println("Press all 3 to exit");
  delay(1000);

  Serial.println("Tuner\tSave\tCompressor");

  keepLooping = true;
  while (keepLooping)
  {
    int ts = digitalRead(Tuner_Switch_Pin);
    int ss = digitalRead(Save_Switch_Pin);
    int cs = digitalRead(Compressor_Switch_Pin);

    Serial.print(ts); Serial.print("\t");
    Serial.print(ss); Serial.print("\t");
    Serial.println(cs);


    if (ts == false && ss == false && cs == false)
      keepLooping = false;
    else
      delay(500);
  }

  Serial.println("Done");
}


void testLEDs()
{
  Serial.println("PFC LED Test");
  delay(1000);

  // sequence LEDs 4 times
  for (int num = 0; num < 3; num++)
  {
    PCF.write(REVERB_LED, LED_ON);
    delay(1000);
    PCF.write(REVERB_LED, LED_OFF);
    delay(200);

    PCF.write(FLANGER_LED, LED_ON);
    delay(1000);
    PCF.write(FLANGER_LED, LED_OFF);
    delay(200);

    PCF.write(TREMOLO_LED, LED_ON);
    delay(1000);
    PCF.write(TREMOLO_LED, LED_OFF);
    delay(200);

    PCF.write(WAH_WAH_LED, LED_ON);
    delay(1000);
    PCF.write(WAH_WAH_LED, LED_OFF);
    delay(200);
  }

  Serial.println("Done");
}



String testPushbuttons()
{
  keepLooping = true;
  while (keepLooping)
  {
    uint8_t val = ~PCF.read8() & 0x0F;
    Serial.print("Switch Byte = 0x0"); Serial.println(val, HEX);

    if (val == 0x01)
      Serial.println("Reverb Switch");

    else if (val == 0x02)
      Serial.println("Flanger Switch");

    else if (val == 0x04)
      Serial.println("Tremolo Switch");

    else if (val == 0x08)
      Serial.println("Wah_Wah Switch");

    if (digitalRead(Save_Switch_Pin) == 0)
      keepLooping = false;

    delay(500);
  }
}


void testPot(int pin)
{
  Serial.println("Rotate Pot to Test");
  Serial.println("Press 'SAVE' to exit");

  keepLooping = true;
  while (keepLooping)
  {
    int val = analogRead(pin);
    float volts = (float)val * 3.3 / 1023.0;

    Serial.print("value = "); Serial.print(val);
    Serial.print("   Volts = "); Serial.println(volts, 2);

    if (digitalRead(Save_Switch_Pin) == 0)
      keepLooping = false;
    else
      delay(500);
  }

  Serial.println("Done");
}



void testEncoder(int encoder)
{
  int32_t val;

  Serial.println("Rotate Encoder CW and CCW to Test");
  Serial.println("Press 'SAVE' to exit");

  // set encoders to 0
  parameterEncoder.write(0);
  valueEncoder.write(0);

  keepLooping = true;
  while (keepLooping)
  {
    if (encoder == 1)
      val = parameterEncoder.read();
    else if (encoder == 2)
      val = valueEncoder.read();

    Serial.print("Encoder value = "); Serial.println(val);

    if (digitalRead(Save_Switch_Pin) == 0)
      keepLooping = false;
    else
      delay(500);
  }

  Serial.println("Done");
}
