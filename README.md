CommandInterpreter is a shell-like command interpreter that parses one 
command + one argument per line from char buffer of a serial connection.
Example: Command: "blink" with argument "on" or "off": blink [ on | off ]

Copy CatsCommandInterpreter/* to your libraries folder and restart the 
Arduino SDK. Open the example as sketch and study it.

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
