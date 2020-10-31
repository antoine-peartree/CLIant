# CLIant Linux C example

Build the example with
> make

Then run the example
> ./build/ex_CLIant_c_linux

You can now write command into the fifo created at
> /tmp/cliant_fifo

Example :
> echo ping
should trigger "ping" output from your program.

You can either try :
> echo "add --nb1 2 --nb2 5" > /tmp/cliant_fifo
The example should then display the result of the addition.

Let's create your own functions !
