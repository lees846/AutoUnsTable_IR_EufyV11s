/*
 * Modified by Shayla Lee Feb 2024
 * 
 * 
 *  ************************************************************************************
 * SendRawDemo.cpp - demonstrates sending IR codes with sendRaw
 *
 * This example shows how to send a RAW signal using the IRremote library.
 * The example signal is actually a 32 bit NEC signal.
 * Remote Control button: LGTV Power On/Off.
 * Hex Value: 0x20DF10EF, 32 bits
 *
 * If it is a supported protocol, it is more efficient to use the protocol send function
 * (here sendNEC) to send the signal.
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 ************************************************************************************
 * MIT License
 *
 * Copyright (c) 2020-2022 Armin Joachimsmeyer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ************************************************************************************
 */
#include <Arduino.h>

#define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.

//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition
//#define NO_LED_FEEDBACK_CODE      // Saves 566 bytes program memory
//#define USE_OPEN_DRAIN_OUTPUT_FOR_SEND_PIN // Use or simulate open drain output mode at send pin. Attention, active state of open drain is LOW, so connect the send LED between positive supply and send pin!

#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <IRremote.hpp>

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
#if defined(__AVR_ATmega328PU__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/|| defined(SERIALUSB_PID) || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.println(F("Send IR signals at pin " STR(IR_SEND_PIN)));

    IrSender.begin(); // Start with IR_SEND_PIN as send pin and enable feedback LED at default feedback LED pin
}

void loop() {
  /**
   * Sample IrSender code
   * IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &downRawData[0], 48, PROTOCOL_IS_LSB_FIRST, <RepeatPeriodMillis>, <numberOfRepeats>);
   * 
   * [0] to [6] are local to the remote, and can be used for all 4 tested signals for the Eufy (up right down left)
   * "&downRawData[0]" needs to be specific to the uint32_t name 
   * [8] doesn't change between button signals
   * PROTOCOL_IS_LSB_FIRST: not sure but works
   * <RepeatPeriodMillis>: 
   *  ms between signal sends
   *  needs to be >200 or so to be smooth
   *  ~500 starts to look jumpy but would be quite chaotic
   * <numberOfRepeats>: 
   *  Might start at 0 (i.e. 3 -> 4 signals sent)
   * 
   * One send of forward/UP is around 200, 4 for any of the other button sends
   *  
  */
  /*
  Serial.println(F("Send Eufy UP Command"));
  Serial.flush();
  uint32_t genRawData[]={hex};
  IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &whichThing[index], 48, PROTOCOL_IS_LSB_FIRST, 0, 3);
  delay(500); // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
  */
  
  // Send UP arrow signal
  Serial.println(F("Send Eufy UP Command"));
  Serial.flush();
  uint32_t upRawData[]={0x70A8F416, 0x5D00};
  IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &upRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 3);
  delay(500); // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
  
  // Send RIGHT arrow signal
  Serial.println(F("Send Eufy RIGHT Command"));
  Serial.flush();
  uint32_t rightRawData[]={0xB4687616, 0x9800};
  IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &rightRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 200, 4);
  delay(500);

  // Send LEFT arrow signal
  Serial.println(F("Send Eufy LEFT Command"));
  Serial.flush();
  uint32_t leftRawData[]={0xD4687C16, 0xE700};
  IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &leftRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 500, 3);
  delay(500);

  // Send DOWN arrow signal
  Serial.println(F("Send Eufy DOWN Command"));
  Serial.flush();
  uint32_t downRawData[]={0x34687E16, 0x1400};
  IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &downRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 500, 3);
  delay(500);

  delay(3000);

}

// array of moves[size] [up, right, down, left]
// 

// in loop
  // 
