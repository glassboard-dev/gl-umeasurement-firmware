<p align="center">
  <h1 align="center">µMeasurement</h1>
  <p align="center">
    Open-Source Real-Time current measurement device firmware
  </p>
</p>

## Getting started
To get started with the uMeasurement device you will need a couple pieces of software and a piece of hardware.

### Software 💻
At a minium you will need the ARM GNU GCC Toolchain, Make and CMake to compile the source and generate executable artifacts.
-   [ARM GNU GCC Toolchain - 10-2020-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) - Other versions can be used but the current source is actively developed and tested using the 10-2020-q4-major release
-   Make is shipped with Linux and should already be present on the developers system. If the developer is running Windows they can either use WSL to run a Linux instance or install Make on Windows via [MinGW](https://sourceforge.net/projects/mingw/)
-   [CMake](https://cmake.org/download/)

To flash and debug the target this project uses a couple of different tools. PyOCD is used for the programming/debugging medium, and a VScode extension called Cortex-Debug gives a user interface within VScode for stepping through the source, settings breakpoints and viewing registers/variables within the target.
-   [PyOCD](https://github.com/pyocd/pyOCD#installing) - Version 0.30.5 or newer is required when programming/debugging the LPC55x microcontrollers.
-   [VScode](https://code.visualstudio.com/)
-   [Cortex-Debug VScode extenstion](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)

### Hardware ⚡
The uMeasurement hardware is based on the [LCP55xx](https://www.nxp.com/part/LPC5526JBD64#/) family of microcontrollers. There are two options for acquiring a target compatible with the uMeasurement firmware. The first and best option is a piece of the uMeasurement hardware itself or you can acquire an LCPXpresso dev kit for the LPC55S28. The downside of the dev kit is that it does not contain the necessary hardware to do differential analog measurements.
- [uMeasurement hardware](https://github.com/glassboard-dev/gl-umeasurement-hardware)
- [LCPXpresso55S28](https://www.nxp.com/design/software/development-software/lpcxpresso55s28-development-board:LPC55S28-EVK)

You will also need a debug probe for flashing and debugging. Glasslabs provides a probe compatible with PyOCD capable of flashing and debugging the target in addition to making the necessary connections for sending UART debug data out of the target when connecting to the uMeasurement hardware.
- [GlassLink Debug Probe](https://github.com/glassboard-dev/gl-glass-link-hardware)

Alternatively, the onboard debugger on the LCPXpresso dev kit can be used when developing on the dev kit hardware.

## Setup ✔️
To begin compiling and developing the firmware, move to the armgcc directory and execute the setup script, passing the PN you wish to generate a Makefile for (LPC5526 or LPC55S28). The uMeasurement board uses the LPC5526 and the LPCXpresso dev kit uses the LPC55S28 micro. You can optionally pass a path to your ARM GNU GCC toolchain. If left blank a default path will be used.
#### Linux
```bash
$ cd armgcc
# Default path of '/usr/local/gcc-arm-none-eabi-10-2020-q4-major' will be used.
$ ./setup.sh LPC5526
# OR
$ ./setup.sh LPC5526 /${USERS CUSTOM PATH TO ARM GCC TOOLCHAIN}/gcc-arm-none-eabi-10-2020-q4-major
```

#### Windows
```bash
$ cd armgcc
# Default path of 'C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2020-q4-major' will be used.
$ ./setup.ps1 LPC5526
# OR
$ ./setup.ps1 LPC5526 '\${USERS CUSTOM PATH TO ARM GCC TOOLCHAIN}\10 2020-q4-major'
```

## Compiling & Flashing ✨
The following commands should be executed from the armgcc directory.

To clean the working directory:
```bash
$ make clean
```

To compile the source:
```bash
$ make -j8
```

To erase the connected target:
```bash
$ make erase
```

To flash the connected target:
```bash
$ make flash
```