/*********************************************************************
*** Based on BluefruitBLE nRF8001 Bluetooth Low Energy Breakout
*** ttp://github.com/adafruit/Adafruit_nRF8001
*** ttp://www.adafruit.com/products/1697
***
*** Download any Bluetooth Low Energy UART App like:
*** https://itunes.apple.com/us/app/nrf-uart/id614594903?mt=8
*** ... connect ... type "blink on" (if you got the hardware set up)
*********************************************************************/

// This version uses call-backs on the event and RX so there's no data handling in the main loop!

#include <SPI.h>
#include "Adafruit_BLE_UART.h"

// ====== Bluefruit UART PIN SETUP ======
#define ADAFRUITBLE_REQ 10
#define ADAFRUITBLE_RDY 2
#define ADAFRUITBLE_RST 9
// Initialize UART:
Adafruit_BLE_UART uart = Adafruit_BLE_UART(ADAFRUITBLE_REQ, ADAFRUITBLE_RDY, ADAFRUITBLE_RST);

// ====== CUSTOM PIN SETUP ======
#define ledPin A3

// ====================================
// ===== COMMAND INTERPRETER      =====
// ====================================

// #define CMD_MAX_COMMANDS 7
#include "CatsCommandInterpreter.h"
CatsCommandInterpreter commandInterpreter = CatsCommandInterpreter();

// ====================================
// ====== COMMANDS               ======
// ====================================

boolean blinkNow = false;

void json_append_attr_int(String *status, char *attr, int val)
{
    *status += '"';
    *status += attr;
    *status += "\":";
    *status += val;
}

boolean command_status(char* arg1)  // TX: STATUS-JSON {"blinking":1}
{
    // ==== STATUS JSON ====>>>
    String status = "{";  // using String adds 2kB!
    
    json_append_attr_int(&status, "blinking", blinkNow); // status += ',';
    
    status += '}';
    // <<<==== STATUS JSON ====    
    
    uart.print(status);
    return true;
}

boolean command_blink(char* arg1)
{
    if(strcmp(arg1, "on") == 0) // command1: start blinking
    {
        // do not use delay before write, WHY? 
        uart.print("BLINK ON!");
        
        // execute the command
        blinkNow = true;
        return true;
    }
    
    if(strcmp(arg1, "off") == 0) // command2: stop blinking
    {
        // do not use delay before write, WHY? 
        uart.print("BLINK OFF!");
        
        // execute the command
        blinkNow = false;
        return true;
    }
    
    return false;
}

// ====================================
// ====== SETUP                  ======
// ====================================

void setup(void)
{
    // ====== BEGIN == Bluefruit UART SETUP ======
    uart.setRXcallback(rxCallback);
    uart.setACIcallback(aciCallback);
    uart.setDeviceName("BlueBli"); /* 7 characters max! */
    uart.begin();
    // ====== END ==== Bluefruit UART SETUP ======
    
    // ====== BEGIN == Start Serial Console ======
    Serial.begin(9600); // print smart stuff to the console
    delay(1000);
    Serial.println(F("nRF8001 CommandInterpreter Demo"));
    // ====== END ==== Start Serial Console ======
    
    // ======  ==============================
    pinMode(ledPin,OUTPUT);    

    // ======  REGISTER COMMANDS ==============================
    commandInterpreter.addCommand("blink", command_blink);
    commandInterpreter.addCommand("status", command_status);
}

// ====================================
// ====== MAIN LOOP              ======
// ====================================

void loop()
{
    uart.pollACI();

    if(blinkNow)
    {
        static int state = LOW;
        state = ~state;
        digitalWrite(ledPin, state);
        delay(500);
    }
    else
        digitalWrite(ledPin, LOW);
}

/**************************************************************************/
/*!
    This function is called whenever select ACI events happen
*/
/**************************************************************************/
void aciCallback(aci_evt_opcode_t event)
{
  switch(event)
  {
    case ACI_EVT_DEVICE_STARTED:
      Serial.println(F("Advertising started"));
      break;
    case ACI_EVT_CONNECTED:
      Serial.println(F("Connected!"));
      break;
    case ACI_EVT_DISCONNECTED:
      Serial.println(F("Disconnected or advertising timed out"));
      break;
    default:
      break;
  }
}

/**************************************************************************/
/*!
    This function is called whenever data arrives on the RX channel
*/
/**************************************************************************/
void rxCallback(uint8_t *buffer, uint8_t len)
{
  Serial.print(F("Received "));
  Serial.print(len);
  Serial.print(F(" bytes: "));
  
  if(commandInterpreter.interpretCommand(buffer, len)) // check if command can be interpreted
      return;  // do not echo if command was a success

  uart.print("Huh?: ");
  uart.write(buffer, len);
  
  // print buffer in HEX to the console:
  /*
  Serial.print(F(" ["));
  for(int i=0; i<len; i++)
  {
      Serial.print(" 0x"); Serial.print((char)buffer[i], HEX); 
  }
  Serial.println(F(" ]"));
  */
}

