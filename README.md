# chip8_emu
## What's this?
>CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s. CHIP-8 programs are run on a CHIP-8 virtual machine. It was made to allow video games to be more easily programmed for these computers, but CHIP 8 is still used today, due to its simplicity, and consequently on any platform and its teaching of programming Binary numbers. -Wikipedia  

The emulator was written in C++ (not purely) and was designed with efficiency, simplicity and cross-compability in mind. Currently the clock defaults at 600Hz however you are free to change it from the arguments.  
*Quick note: All of the included roms are NOT mine*

## Controls
CHIP-8's original layout:
```
| 1 | 2 | 3 | C |
| 4 | 5 | 6 | D |
| 7 | 8 | 9 | E |
| A | 0 | B | F |
```  
Emulator's layout:  
```
| 1 | 2 | 3 | 4 |
| Q | W | E | R |
| A | S | D | F |
| Z | X | C | V |
```

## Building
To build the emulator just type:
```
$ make
```

## Executing
To run the emulator use the following command:
```
$ ./chip8 {INSERT_ROM_PATH} {CUSTOM_CLOCK: -c} {DEBUG: -dbg}
```
{**INSERT_ROM_PATH}** : The first argument must always be the rom path that you are trying to emulate. Invalid paths will result to an error.  
***Optional* {CUSTOM_CLOCK: -c}** : This optional argument will define a custom clock speed (in hz) just in case you need it. *Default is 600Hz.*  
***Optional* {DEBUG: -dbg}** :  This optional argument will enable all of the debug features currently available. *Disabled by default.*  

## Debugging
This implementation has the following debugging features:  
* Instruction debugging
* Memory dumping
* Graphics memory dumping