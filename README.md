CommandInterpreter is a shell-like command interpreter that parses one 
command + one argument per line from char buffer of a serial connection.
Example: Command: "blink" with argument "on" or "off": blink [ on | off ]

Download and install the dependent library [Adafruit_nRF8001](http://github.com/adafruit/Adafruit_nRF8001)
then copy CatsCommandInterpreter/* to your libraries folder and restart the 
Arduino SDK. Open the example as sketch and study it.

Use an UART Serial App like this: https://itunes.apple.com/us/app/nrf-uart/id614594903?mt=8

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

![](https://raw.githubusercontent.com/katzlbt/arduino-bluetooth-commander/master/pictures/ArduinoProBluefruit.jpg)
