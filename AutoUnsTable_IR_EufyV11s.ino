/*
 * Shayla Lee Feb 2024
 * 
 * This code is the protocol which controls UnsTable 
 * 
 *************************************************************************************
 * This code uses Arduino-IRremote Library https://github.com/Arduino-IRremote/Arduino-IRremote 
 * and this file is based on SendRawDemo.cpp, referencing ReceiveAndSendDistanceWidth.cpp, 
 * starting on February 15, 2024
 * 
 * The signals in this file address a Eufy Robovac 11s, collected using RecieveDemo.ino from 
 * the same library.
 * 
 *************************************************************************************
 * 
 *
 */
#include <Arduino.h>

#define DISABLE_CODE_FOR_RECEIVER // Disables restarting receiver after each send. Saves 450 bytes program memory and 269 bytes RAM if receiving functions are not used.

//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition
//#define NO_LED_FEEDBACK_CODE      // Saves 566 bytes program memory
//#define USE_OPEN_DRAIN_OUTPUT_FOR_SEND_PIN // Use or simulate open drain output mode at send pin. Attention, active state of open drain is LOW, so connect the send LED between positive supply and send pin!

#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <IRremote.hpp>

const String robotMoves[] = {"up", "right", "down", "left"};

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);   // Feedback on the arduino board builtin LED for when a signal is sent
    Serial.begin(115200);
    
    #if defined(__AVR_ATmega328PU__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/|| defined(SERIALUSB_PID) || defined(ARDUINO_attiny3217)
       delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
    #endif
    // Just to know which program is running on the Arduino
    Serial.println(("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.println(("Send IR signals at pin " STR(IR_SEND_PIN)));

    IrSender.begin(); // Start with IR_SEND_PIN as send pin and enable feedback LED at default feedback LED pin
}

void loop() {
  // Select two random integers between 0 and length of robotMoves[]
  const int firstMove = int(random(4));
  const int secondMove = int(random(4));
  Serial.println(firstMove);
  Serial.println(secondMove);

  // Move UnsTable depending on which index was chosen for each move
  moveUnsTable(firstMove);
  moveUnsTable(secondMove);

  // Wait 3 seconds between each set of moves
  delay(3000);

}

// Sends IR signal to UnsTable depending on which move is passed
void moveUnsTable(int whichMove) {
  if(robotMoves[whichMove] == "up"){
    moveUp();
  }  
  else if(robotMoves[whichMove] == "right"){
    turnRight();
  }  
  else if(robotMoves[whichMove] == "down"){
    moveDown();
  }
  else if(robotMoves[whichMove] == "left"){
    turnLeft();
  }
  
  // add logic for each new element in robotMoves
}

// Send UP arrow signal to Eufy from IR LED
void moveUp() {
  Serial.println(("Send Eufy UP Command"));
  Serial.flush();
  uint32_t upRawData[]={0x70A8F416, 0x5D00};
  IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &upRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 0, 3);
  delay(500); // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
}

// Send RIGHT arrow signal to Eufy from IR LED
void turnRight() {
  Serial.println(("Send Eufy RIGHT Command"));
  Serial.flush();
  uint32_t rightRawData[]={0xB4687616, 0x9800};
  IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &rightRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 200, 4);
  delay(500);
}

// Send DOWN arrow signal to Eufy from IR LED
void moveDown() {
  Serial.println(("Send Eufy DOWN Command"));
  Serial.flush();
  uint32_t downRawData[]={0x34687E16, 0x1400};
  IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &downRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 500, 3);
  delay(500);
}

// Send LEFT arrow signal to Eufy from IR LED
void turnLeft() {
  Serial.println(("Send Eufy LEFT Command"));
  Serial.flush();
  uint32_t leftRawData[]={0xD4687C16, 0xE700};
  IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &leftRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 500, 3);
  delay(500);
}


/**
   * Sample IrSender code (would be found in void loop(){})
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
