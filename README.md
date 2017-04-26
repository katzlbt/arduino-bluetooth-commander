CommandInterpreter is a shell-like command interpreter that parses one 
command + two arguments per line from char buffer of a serial connection,
which is a Bluetooth Smart (low energy) UART in this case.
One command is limited to 20 chars as the UART nRf8001 Bluefruit hardware cannot process more
than that in one UART message/packet. Return messages can be assembled by the
software on the smartphone to larger messages.

V1.1 2017-04-22 Added Controllino Mini Hardware and Adapter https://controllino.biz/product/
V1.1 2017-04-22 Added https://www.adafruit.com/product/2633
V1.0 2016       https://www.adafruit.com/product/1697

Example commands:
  * "blink" with argument "on" or "off": blink [ on | off ]
  * ---
  * "pmode a0 o"   ... pinMode(A0, OUTPUT)
  * "dwrite a0 h"  ... digitalWrite(A0, HIGH)
  * ---  
  * "pmode d1 o"   ... pinMode(5, OUTPUT)
  * "awrite d1 128" ... analogWrite(5, 128)
  * "pmode d2 o"   ... pinMode(6, OUTPUT)
  * "awrite d2 64" ... analogWrite(6, 128)

Download and install the dependent library [Adafruit_nRF8001](http://github.com/adafruit/Adafruit_nRF8001)

Use an UART Serial App like this: https://itunes.apple.com/at/app/nrf-toolbox/id820906058?mt=8

USAGE:

    #define CMD_MAX_COMMANDS 7
    #include "CatsCommandInterpreter.h"
    CatsCommandInterpreter commandInterpreter = CatsCommandInterpreter();

    boolean command_blink(char* arg1) {...}
    
    setup() { commandInterpreter.addCommand("blink", command_blink); }
    
    rx() 
    {
      if(commandInterpreter.interpretCommand(buffer, len)) // check if command can be interpreted
        return;  // do not echo if command was a success
    }

Links:
  * https://electronza.com/controllino-mini-review/
  * http://controllino.biz/wp-content/uploads/2016/05/MINI_DATASHEET-19-04-2016.pdf
  * http://www.produktinfo.conrad.com/datenblaetter/1300000-1399999/001325474-an-01-en-CONTROLLINO_MINI.pdf

![](https://raw.githubusercontent.com/katzlbt/arduino-bluetooth-commander/master/pictures/ArduinoProBluefruit.jpg)
