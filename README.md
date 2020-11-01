# CLIant

CLIant is a lib allowing user to call functions with Shell-like command lines.
For an embedded bare-metal system target, this can be used to debug/test functions with receiving Shell-like command lines from serial.

![CLIant Schema](https://raw.githubusercontent.com/antoine-peartree/CLIant/main/example_c_stm32L5/CLIantSchema.png)

However some Linux examples are provided in order to quickly try CLIant and better understand the way it works, with named FIFO as input entry for command lines.

## Get started

1. Create your own function on the following model
```C
int myFunction(int argc, char **argv)
```

2. Declare a command with 4 parameters : label (the first word of your command line. = argv[0]), a reference to your myFunction, an option description and a global description of the command.

3. Get input command lines from a serial, a named pipe, ...

4. Call cmd parse function to parse the command and call the dedicated function.

## Examples
Each example directory contains its own README.
