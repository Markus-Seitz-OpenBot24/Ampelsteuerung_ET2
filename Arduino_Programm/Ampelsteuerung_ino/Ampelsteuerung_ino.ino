
// Ampelsteuerung mit Arduino Mega 2560 digital + Android App (via Bluetooth)

#include <SoftwareSerial.h>
#include <FastLED.h>




// create a serial connection object for the bluetooth connection

SoftwareSerial Bluetooth = SoftwareSerial(15, 14); // pins for RX and TX for bluetooth connection


// define variables for the Arduino

const int digit_button_pins_A1_IN[] = {26, 30};
const int digit_button_pins_A2_IN[] = {27, 31};
const int digit_button_pins_A3_IN[] = {28, 32};
const int digit_button_pins_A4_IN[] = {29, 33};

int pin_count_buttons_A1 = sizeof(digit_button_pins_A1_IN) / sizeof(int);
int pin_count_buttons_A2 = sizeof(digit_button_pins_A2_IN) / sizeof(int);
int pin_count_buttons_A3 = sizeof(digit_button_pins_A3_IN) / sizeof(int);
int pin_count_buttons_A4 = sizeof(digit_button_pins_A4_IN) / sizeof(int);

char incoming_value_char;
int incoming_value_int = 0;


// define variables for the WS2812 RGB LED

#define RGB_PIN_1      5            // data pin for traffic light 1
#define RGB_PIN_2      6            // data pin for traffic light 2
#define RGB_PIN_3      7            // data pin for traffic light 3
#define RGB_PIN_4      8            // data pin for traffic light 4
#define RGB_LED_NUM_1  7            // 8 LEDs [0...7]
#define RGB_LED_NUM_2  7            // 8 LEDs [0...7]
#define RGB_LED_NUM_3  7            // 8 LEDs [0...7]
#define RGB_LED_NUM_4  7            // 8 LEDs [0...7]
#define BRIGHTNESS     50            // brightness range [0..255]
#define CHIP_SET     WS2812B        // types of RGB LEDs
#define COLOR_CODE   GRB            // sequence of colors in data stream

// define the array of LEDs for each traffic light

CRGB LED_ARR_1[RGB_LED_NUM_1];
CRGB LED_ARR_2[RGB_LED_NUM_2];
CRGB LED_ARR_3[RGB_LED_NUM_3];
CRGB LED_ARR_4[RGB_LED_NUM_4];

#define UPDATES_PER_SECOND 100




// functions

void switch_A1_green(){

  for (int i = 0; i < RGB_LED_NUM_1; i++) {

    LED_ARR_1[i] = CRGB(0, 255, 0);   // set to green
    FastLED.show();

  }
}

void switch_A1_red(){

  for (int i = 0; i < RGB_LED_NUM_1; i++) {

    LED_ARR_1[i] = CRGB(255, 0, 0);   // set to red
    FastLED.show();
    
  }
}

void switch_A2_green(){

  for (int i = 0; i < RGB_LED_NUM_2; i++) {

    LED_ARR_2[i] = CRGB(0, 255, 0);   // set to green
    FastLED.show();
    
  }
}

void switch_A2_red(){

  for (int i = 0; i < RGB_LED_NUM_2; i++) {

    LED_ARR_2[i] = CRGB(255, 0, 0);   // set to red
    FastLED.show();
    
  }
}

void switch_A3_green(){

  for (int i = 0; i < RGB_LED_NUM_3; i++) {

    LED_ARR_3[i] = CRGB(0, 255, 0);   // set to green
    FastLED.show();
    
  }
}

void switch_A3_red(){

  for (int i = 0; i < RGB_LED_NUM_3; i++) {

    LED_ARR_3[i] = CRGB(255, 0, 0);   // set to red
    FastLED.show();
    
  }
}

void switch_A4_green(){

  for (int i = 0; i < RGB_LED_NUM_4; i++) {

    LED_ARR_4[i] = CRGB(0, 255, 0);   // set to green
    FastLED.show();
    
  }
}

void switch_A4_red(){

  for (int i = 0; i < RGB_LED_NUM_4; i++) {

    LED_ARR_4[i] = CRGB(255, 0, 0);   // set to red
    FastLED.show();
    
  }
}




// setup

void setup() {

  // define pinmode for Bluetooth RX and TX

  //pinMode(15, INPUT);
  //pinMode(14, OUTPUT);


  // start serial connection

  Serial.begin(9600);
  delay(1000);
  Serial.println("initialized serial monitor...");
  Bluetooth.begin(9600);
  delay(1000);
  Serial.println("initialized bluetooth serial...");


  // setup for the WS2812

  Serial.println("WS2812 LED-strip initialize...");
  FastLED.addLeds<CHIP_SET, RGB_PIN_1, COLOR_CODE>(LED_ARR_1, RGB_LED_NUM_1);
  FastLED.addLeds<CHIP_SET, RGB_PIN_2, COLOR_CODE>(LED_ARR_2, RGB_LED_NUM_2);
  FastLED.addLeds<CHIP_SET, RGB_PIN_3, COLOR_CODE>(LED_ARR_3, RGB_LED_NUM_3);
  FastLED.addLeds<CHIP_SET, RGB_PIN_4, COLOR_CODE>(LED_ARR_4, RGB_LED_NUM_4);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);


  // define digital button pins as inputs

    // for A1

  for (int i = 0; i < pin_count_buttons_A1; i++) {
    pinMode(digit_button_pins_A1_IN[i], INPUT_PULLUP); // activated internal pullup-resistor due to EMV -> inverted logic...
  }

    // for A2

  for (int i = 0; i < pin_count_buttons_A2; i++) {
    pinMode(digit_button_pins_A2_IN[i], INPUT_PULLUP); // activated internal pullup-resistor due to EMV -> inverted logic...
  }

    // for A3

  for (int i = 0; i < pin_count_buttons_A3; i++) {
    pinMode(digit_button_pins_A3_IN[i], INPUT_PULLUP); // activated internal pullup-resistor due to EMV -> inverted logic...
  }

    // for A4

  for (int i = 0; i < pin_count_buttons_A4; i++) {
    pinMode(digit_button_pins_A4_IN[i], INPUT_PULLUP); // activated internal pullup-resistor due to EMV -> inverted logic...
  }

}




// loop

void loop() {

  //---SWITCHING THE TRAFFIC LIGHTS VIA DIGITAL BUTTONS--->

  // check if any buttons have been pressed and if so, switch the traffic lights accordingly ->

    // check/switch A1

  for (int i = 0; i < pin_count_buttons_A1; i++) {

    if (digitalRead(digit_button_pins_A1_IN[i]) != true) {

      int pin_number;
      pin_number = digit_button_pins_A1_IN[i];
      Serial.print("pressed button ");
      Serial.print(pin_number);
      Serial.println(" at A1");

      switch (pin_number) {

        case 26:
        switch_A1_green();
        Serial.println("switched A1 to green!");
        break;
        case 30:
        switch_A1_red();
        Serial.println("switched A1 to red!");
        break;

      }

    }

  }

    // check/switch A2

  for (int i = 0; i < pin_count_buttons_A2; i++) {

    if (digitalRead(digit_button_pins_A2_IN[i]) != true) {

      int pin_number;
      pin_number = digit_button_pins_A2_IN[i];
      Serial.print("pressed button ");
      Serial.print(pin_number);
      Serial.println(" at A2");

      switch (pin_number) {

        case 27:
        switch_A2_green();
        Serial.println("switched A2 to green!");
        break;
        case 31:
        switch_A2_red();
        Serial.println("switched A2 to red!");
        break;

      }

    }

  }

    // check/switch A3

  for (int i = 0; i < pin_count_buttons_A3; i++) {

    if (digitalRead(digit_button_pins_A3_IN[i]) != true) {

      int pin_number;
      pin_number = digit_button_pins_A3_IN[i];
      Serial.print("pressed button ");
      Serial.print(pin_number);
      Serial.println(" at A3");

      switch (pin_number) {

        case 28:
        switch_A3_green();
        Serial.println("switched A3 to green!");
        break;
        case 32:
        switch_A3_red();
        Serial.println("switched A3 to red!");
        break;

      }

    }

  }

    // check/switch A4

  for (int i = 0; i < pin_count_buttons_A4; i++) {

    if (digitalRead(digit_button_pins_A4_IN[i]) != true) {

      int pin_number;
      pin_number = digit_button_pins_A4_IN[i];
      Serial.print("pressed button ");
      Serial.print(pin_number);
      Serial.println(" at A4");

      switch (pin_number) {

        case 29:
        switch_A4_green();
        Serial.println("switched A4 to green!");
        break;
        case 33:
        switch_A4_red();
        Serial.println("switched A4 to red!");
        break;

      }

    }

  }




  //---SWITCHING THE TRAFFIC LIGHTS VIA BLUETOOTH APP--->

  // check if data is available on the bluetooth serial connection ->
  // only execute if data is available; otherwise just move on...

  if (Serial.available() > 0) {

    incoming_value_char = Serial.read();
    Serial.print("value received as char -> ");
    Serial.println(incoming_value_char);

    incoming_value_int = (int)incoming_value_char;

    Serial.print("value as int -> ");
    Serial.println(incoming_value_int);

    switch (incoming_value_int) {

      case 49:                // 49 is ASCII Code for "1"
      switch_A1_green();
      Serial.println("switched A1 to green!");
      break;

      case 50:                // 50 is ASCII Code for "2"
      switch_A1_red();
      Serial.println("switched A1 to red!");
      break;

      case 51:                // 51 is ASCII Code for "3"
      switch_A2_green();
      Serial.println("switched A2 to green!");
      break;

      case 52:                // 52 is ASCII Code for "4"
      switch_A2_red();
      Serial.println("switched A2 to red!");
      break;

      case 53:                // 53 is ASCII Code for "5"
      switch_A3_green();
      Serial.println("switched A3 to green!");
      break;

      case 54:                // 54 is ASCII Code for "6"
      switch_A3_red();
      Serial.println("switched A3 to red!");
      break;

      case 55:                // 55 is ASCII Code for "7"
      switch_A4_green();
      Serial.println("switched A4 to green!");
      break;

      case 56:                // 56 is ASCII Code for "8"
      switch_A4_red();
      Serial.println("switched A4 to red!");
      break;

    }

  }

}