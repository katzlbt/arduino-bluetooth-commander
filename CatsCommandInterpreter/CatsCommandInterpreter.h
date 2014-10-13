// ====================================
// ===== COMMAND INTERPRETER      =====
// ====================================

// CONFIGURATION define those before including this header

#define ACTIVATE_SERIAL 1

#ifndef ACTIVATE_SERIAL
  #define Serial.print(x) 
  #define Serial.println(x) 
#endif

#ifndef CMD_WORD_SEPARATOR
  #define CMD_WORD_SEPARATOR ' '
#endif

#ifndef CMD_MAX_COMMANDS
  #define CMD_WORD_MAXLEN 9
#endif

#ifndef CMD_MAX_COMMANDS
  #define CMD_MAX_COMMANDS 7
#endif

/** CommandInterpreter is a shell-like command interpreter that parses one 
 ** command + one argument per line from char buffer of a serial connection.
 ** Example: Command: "blink" with argument "on" or "off": blink [ on | off ]
 ** blink off
 USAGE:
    #define CMD_MAX_COMMANDS 7
    #include "CatsCommandInterpreter.h"
    CatsCommandInterpreter commandInterpreter = CatsCommandInterpreter();

    boolean command_blink(char* arg1) {...}
    setup() { commandInterpreter.addCommand("blink", command_blink); }
    
    rx() 
    { commandInterpreter.parseArguments(buffer, len);
      if(commandInterpreter.interpretCommand()) // check if command can be interpreted
        return;  // do not echo if command was a success
    }
 **/
typedef boolean (*CommandExecuteCallback)(char* arg1, char* arg2);  

class CatsCommandInterpreter
{
    char command[CMD_WORD_MAXLEN+1];  // currently parsed command
    char arg1[CMD_WORD_MAXLEN+1];     // currently parsed argument
    char arg2[CMD_WORD_MAXLEN+1];     // currently parsed argument
    
    const char *commands[CMD_MAX_COMMANDS];             // available commands
    CommandExecuteCallback commandf[CMD_MAX_COMMANDS];  // available execution functions
    
  public:
    CatsCommandInterpreter()
    {
        for(int i=0;i<CMD_MAX_COMMANDS;i++)
            commands[i] = 0;
    }
    
    void addCommand(const char *command, CommandExecuteCallback callback)
    {
        for(int i=0;i<CMD_MAX_COMMANDS;i++)
        {   if(commands[i] == 0)  // find free slot and fill it
            {
                commands[i] = command;
                commandf[i] = callback;
                return;
            }
        }
        
        Serial.println(F("CommandInterpreter MAX_COMMANDS exceeded."));
    }
    
    void parseArguments(uint8_t *buffer, uint8_t len)
    {
        // parse incoming command (MAX 20 BYTES?)into command plus 1 argument
        command[0] = command[CMD_WORD_MAXLEN] = 0; // if it is missing
        arg1[0] = arg1[CMD_WORD_MAXLEN] = 0;       // if it is missing
        arg2[0] = arg2[CMD_WORD_MAXLEN] = 0;       // if it is missing
        
        int argc=0, lena=0; // arguments count
        
        char *dest = command; // copy to string 1
        for(int i=0; i<len; i++)
        {
            char c = buffer[i];
            
            if(++lena < CMD_WORD_MAXLEN)
                *dest = c;
            
            if(c == CMD_WORD_SEPARATOR && argc == 0)  // switch to arg1 if space, repeat if there could be more args
            {
                *dest = 0;    // replace space (32) with 0
                dest = arg1;  // string 2
                lena = 0;
                argc++;
            }
            else
            if(c == CMD_WORD_SEPARATOR && argc == 1)  // switch to arg2 if space, repeat if there could be more args
            {
                *dest = 0;    // replace space (32) with 0
                dest = arg2;  // string 3
                lena = 0;
                argc++;
            }
            else // increment if NOT SEPARATOR
                dest++;
        }
        
        *(dest++) = 0; // terminate last arg
        
        Serial.print(command);
        Serial.print("(");
        Serial.print(arg1);
        Serial.print(",");
        Serial.print(arg2);
        Serial.println(")");
    }
    
    boolean interpretCommand(uint8_t *buffer, uint8_t len)  // returns false if unknown/failed command
    {
        this->parseArguments(buffer, len);
        return this->interpretCommand();
    }
    
    boolean interpretCommand()  // returns false if unknown/failed command
    {
        for(int i=0;i<CMD_MAX_COMMANDS;i++)
            if(strcmp(this->command, commands[i]) == 0) // command1: start blinking
                return commandf[i](this->arg1, this->arg2);
                        
        return false;
    }
};

