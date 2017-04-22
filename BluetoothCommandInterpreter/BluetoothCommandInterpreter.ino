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

// select used hardware/board with different pin settings
//#define HW_UNO 1
#define HW_CONTROLLINO 1
#include "Hardware.h"
// Given by hardware: MISO, MOSI, SCK
// defines BLE_CHIP_SELECT, BLE_INTERRUPT, BLE_RESET
// CONTROLLINO RealTimeClock is available with CONTROLLINO_RTC_CHIP_SELECT PIN 10 (LOW=active)

#include <SPI.h>

#include "Adafruit_BLE_UART.h"

// Initialize UART:
Adafruit_BLE_UART uart = Adafruit_BLE_UART(BLE_CHIP_SELECT, BLE_INTERRUPT, BLE_RESET);

// ====== CUSTOM PIN SETUP ======
#define LED_PIN PIN_D0

// define next line to do serial debugging
#define ACTIVATE_SERIAL 1

#ifdef ACTIVATE_SERIAL
  #define sbegin(x) Serial.begin(x)
  #define sprint(args...) Serial.print(args)
  #define sprintln(args...) Serial.println(args)
#else
  #define sbegin(x)
  #define sprint(args...)
  #define sprintln(args...)
#endif

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

#define AVAILABLE_PINS 8
char* pinNames[AVAILABLE_PINS] = {"d0", "d1", "d2", "d3", "d4", "d5", "a0", "a1"};
unsigned char pinIds[AVAILABLE_PINS] = 
  {  PIN_D0, PIN_D1, PIN_D2, 
     PIN_D3, PIN_D4, PIN_D5, 
     PIN_A0, PIN_A1};
boolean dreadStatusPins[AVAILABLE_PINS]  = {1,1,1,1,1,1,1,1};  // set to 1 for inclusion in status

void json_append_attr_int(String *status, char *attr, int val, boolean comma=false)
{
    if(comma) *status += ',';
    *status += '"';
    *status += attr;
    *status += "\":";
    *status += val;
}

boolean command_status(char* arg1, char* arg2)  // TX: STATUS-JSON {"blinking":1}
{
    // ==== STATUS JSON ====>>>
    String status = "{";  // using String adds 2kB!
    
    json_append_attr_int(&status, "blinking", blinkNow); // status += ',';
    
    for(int i=0;i<AVAILABLE_PINS;i++)
    {
        if(dreadStatusPins[i])
            json_append_attr_int(&status, pinNames[i], digitalRead(pinIds[i]), true);            
    }
    
    status += '}';
    // <<<==== STATUS JSON ====    
    
    // the message will be split into 20 byte packets and must be assembled at the destination
    uart.print(status);
    return true;
}

int argument_pin(char* arg)
{
    for(int i=0; i<AVAILABLE_PINS;i++)
        if(pinNames[i][0] == arg[0] && pinNames[i][1] == arg[1])
            return pinIds[i];
    
    uart.print(F("ERROR invalid pin!"));
    return 0;
}

int argument_mode(char* arg)
{
    if(*arg == 'o')
       return OUTPUT;
       
    if(*arg == 'p')
       return INPUT_PULLUP;
       
    return INPUT;
}

int argument_hilo(char* arg)
{
    if(*arg == 'h')
       return HIGH;
       
    return LOW;
}

boolean command_pmode(char* arg1, char* arg2)  // pinmode A0 [O, I]
{
    // available pins are: 4, 5, 6, 7, 8, 3, A0, A1, A2, A3, A4, A5
    int pin = argument_pin(arg1);
    if(pin == 0)
        return true;
    
    int mode = argument_mode(arg2);
    
    pinMode(pin, mode);
    uart.print(F("OK pinmode"));
    
    return true;
}

boolean command_dwrite(char* arg1, char* arg2)  // pinmode A0 [O, I]
{
    int pin = argument_pin(arg1);
    if(pin == 0)
        return true;
    
    digitalWrite(pin, argument_hilo(arg2));
    uart.print(F("OK dwrite"));
    return true;
}

boolean command_dread(char* arg1, char* arg2)  // pinmode A0 [O, I]
{
    int pin = argument_pin(arg1);
    if(pin == 0)
        return true;
        
    if(digitalRead(pin))
        uart.print("1");
    else
        uart.print("0");
       
    return true;
}

boolean command_awrite(char* arg1, char* arg2)  // pinmode A0 [O, I]
{
    int pin = argument_pin(arg1);
    
    if(pin!=3 && pin!=5 && pin!=6) // UNO SPECIFIC
    {
        uart.print(F("ERROR invalid pin!"));
        return true;
    }
    
    analogWrite(pin, atoi(arg2));
    uart.print("OK awrite");
    return true;
}

boolean command_blink(char* arg1, char* arg2)
{
    if(strcmp(arg1, "on") == 0) // command1: start blinking
    {
        // do not use delay before write, WHY? 
        uart.print(F("BLINK ON!"));
        
        // execute the command
        blinkNow = true;
        return true;
    }
    
    if(strcmp(arg1, "off") == 0) // command2: stop blinking
    {
        // do not use delay before write, WHY? 
        uart.print(F("BLINK OFF!"));
        
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
    uart.setDeviceName("Locandy"); /* 7 characters max! */
    uart.begin();
    // ====== END ==== Bluefruit UART SETUP ======
    
    // ====== BEGIN == Start Serial Console ======
    sbegin(9600); // print smart stuff to the console
    delay(1000);
    sprintln(F("nRF8001 CommandInterpreter Demo"));
    // ====== END ==== Start Serial Console ======
    
    // ======  ==============================
    pinMode(LED_PIN,OUTPUT);    

    // ======  REGISTER COMMANDS ==============================
    commandInterpreter.addCommand("blink", command_blink);
    commandInterpreter.addCommand("status", command_status);
    commandInterpreter.addCommand("pmode", command_pmode);
    commandInterpreter.addCommand("dwrite", command_dwrite);
    commandInterpreter.addCommand("dread", command_dread);
    commandInterpreter.addCommand("awrite", command_awrite);
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
        digitalWrite(LED_PIN, state);
        delay(1000);
    }
    else
        digitalWrite(LED_PIN, LOW);
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
      sprintln(F("Advertising started"));
      break;
    case ACI_EVT_CONNECTED:
      sprintln(F("Connected!"));
      break;
    case ACI_EVT_DISCONNECTED:
      sprintln(F("Disconnected or advertising timed out"));
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
  sprint(F("Received "));
  sprint(len);
  sprint(F(" bytes: "));
  
  if(commandInterpreter.interpretCommand(buffer, len)) // check if command can be interpreted
      return;  // do not echo if command was a success

  uart.print(F("Unknown Command Error: "));
  uart.write(buffer, len);
  
  // print buffer in HEX to the console:
  /*
  print(F(" ["));
  for(int i=0; i<len; i++)
  {
      print(" 0x"); print((char)buffer[i], HEX); 
  }
  println(F(" ]"));
  */
}

