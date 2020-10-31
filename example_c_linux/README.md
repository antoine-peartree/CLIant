# CLIant Linux C example

Build the example with
```shell
make
```

Then run the example
```shell
./build/ex_CLIant_c_linux
```

You can now write command into the fifo created at
```shell
/tmp/cliant_fifo
```

Example :
```shell
echo "ping" > /tmp/cliant_fifo
```
Should trigger "ping" output from your program.

You can either try :
```shell
echo "add --nb1 2 --nb2 5" > /tmp/cliant_fifo
```
The example should then display the result of the addition.

Let's create your own functions !
