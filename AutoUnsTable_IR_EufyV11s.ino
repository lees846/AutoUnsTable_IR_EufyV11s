/*
 * Shayla Lee April 2024
 * 
 * This code is the protocol which controls UnsTable. 
 * 
 *************************************************************************************
 * This code uses Arduino-IRremote Library https://github.com/Arduino-IRremote/Arduino-IRremote 
 * and this file is based on SendRawDemo.cpp, referencing ReceiveAndSendDistanceWidth.cpp, 
 * starting on February 15, 2024
 * 
 * The signals in this file address a Eufy Robovac 11s, collected using RecieveDemo.ino from 
 * the same library.
 * 
 * https://roboticsbackend.com/arduino-turn-led-on-and-off-with-button/ was referenced to
 * make an on/off button. Press button once to start, then press and hold to stop. 
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


//const String robotMoves[] = {"up", "right", "down", "left", "spinLeft", "spinRight", "nod", "shake"}; 

// Biased moves list, less likely to move forward or do speical moves
const String robotMoves[] = {
  "spinLeft", 
  "spinRight",
  "nod",
  "shake",
  /* 8 */"up", "up", "up", "up", "up", "up", "up", "up",
  /* 10 */"right", "right", "right", "right", "right", "right", "right", "right", "right", "right", 
  /* 11 */"down", "down", "down", "down", "down", "down", "down", "down", "down", "down", "down",
  /* 10 */"left", "left", "left", "left", "left", "left", "left", "left", "left", "left", 
}; 

// totalMoves is used for random and it's maximum is exclusive, 1 greater than num of robotMoves
const int totalMoves = 44;

// Infrared codes to control EUFY
uint32_t upRawData[]={0x70A8F416, 0x5D00};
uint32_t downRawData[]={0x34687E16, 0x1400};
uint32_t rightRawData[]={0xB4687616, 0x9800};
uint32_t leftRawData[]={0xD4687C16, 0xE700};

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
  // Move UnsTable
  // Select two random integers between 0 and length of robotMoves[]
  const int firstMove = int(random(totalMoves));
  const int secondMove = int(random(totalMoves));
  
  // Set a random time interval between movements between 1 & 5 seconds
  long timeBetween = (random(1000, 5000)); 
  
  Serial.println(firstMove);
  Serial.println(secondMove);
  Serial.println(timeBetween);

  // Move UnsTable depending on which index was chosen for each move
  moveUnsTable(firstMove);
  moveUnsTable(secondMove);

  // Wait between each set of moves
  delay(timeBetween);
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
  else if(robotMoves[whichMove] == "spinLeft"){
    spinLeft();
  }
  else if(robotMoves[whichMove] == "spinRight"){
    spinRight();
  }
  else if(robotMoves[whichMove] == "nod"){
    nod();
  }
  else if(robotMoves[whichMove] == "shake"){
    shake();
  }
  
  // *add logic for each new element here and in robotMoves
}

// Send UP arrow signal to Eufy from IR LED
void moveUp() {
  Serial.println(("Send Eufy UP Command"));
  Serial.flush();
  IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &upRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 300, 0);
  delay(500); // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
}

// Send RIGHT arrow signal to Eufy from IR LED
void turnRight() {
  Serial.println(("Send Eufy RIGHT Command"));
  Serial.flush();
  for (int moveCount = 0; moveCount < 3; moveCount++) {
    IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &rightRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 600, 0);
    delay(100);
  }
  delay(500);
}

// Send DOWN arrow signal to Eufy from IR LED
void moveDown() {
  Serial.println(("Send Eufy DOWN Command"));
  Serial.flush();
  for (int moveCount = 0; moveCount < 3; moveCount++) {
    IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &downRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 600, 0);
    delay(100);
  }
  delay(500);
}

// Send LEFT arrow signal to Eufy from IR LED
void turnLeft() {
  Serial.println(("Send Eufy LEFT Command"));
  Serial.flush();
  for (int moveCount = 0; moveCount < 3; moveCount++) {
    IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &leftRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 200, 0);
    delay(100);
  }
  delay(500);
}

// Spin Left aka do a 360º (Left)
void spinLeft() {
  Serial.println(("Send Eufy LEFT 360 Command"));
  Serial.flush();
  // Around 21 turns make a 360º spin
  for (int moveCount = 0; moveCount < 20; moveCount++) {
    IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &leftRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 600, 0);
    delay(100);
  }
  delay(500);
}

// Spin Right aka do a 360º (Right) 
void spinRight() {
  Serial.println(("Send Eufy RIGHT 360 Command"));
  Serial.flush();
  // Around 21 turns make a 360º spin
  for (int moveCount = 0; moveCount < 20; moveCount++) {
    IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &rightRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 600, 0);
    delay(100);
  }
  delay(500);
}

// Nod aka move forward and back twice (like nodding)
void nod() {
  Serial.println(("Send Eufy NOD Command"));
  Serial.flush();
  for (int nods = 0; nods < 2; nods++) {
    IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &upRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 600, 0);
    delay(200);
    for (int moveCount = 0; moveCount < 3; moveCount++) {
      IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &downRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 600, 0);
      delay(100);
    }
  }
  delay(500); // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
}

// Shake aka turn left to right twice (like shaking head "no")
void shake() {
  Serial.println(("Send Eufy SHAKE Command"));
  Serial.flush();
  for (int shakes = 0; shakes < 2; shakes++) {
    for (int moveCount = 0; moveCount < 3; moveCount++) {
      IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &leftRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 200, 0);
      delay(100);
    }
    for (int moveCount = 0; moveCount < 3; moveCount++) {
      IrSender.sendPulseDistanceWidthFromArray(38, 3000, 2900, 550, 1400, 550, 450, &rightRawData[0], 48, PROTOCOL_IS_LSB_FIRST, 600, 0);
      delay(100);
    }
  }
  delay(500); // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
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
