# gl-umeasurement-firmware

## Setup
To begin, move to the armgcc directory and execute the setup script, passing the PN you wish to generate a Makefile for. (LPC5526 or LPC55S28)
```bash
$ cd armgcc
$ ./setup.sh LPC5526
```

## Makefile Commands
To clean the working directory:
```bash
$ make clean
```

To compile run the following:
```bash
$ make -j12
```

To erase the connected target:
```bash
$ make erase
```

To flash the connected target:
```bash
$ make flash
```