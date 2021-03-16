<p align="center">
  <h1 align="center">µMeasurement</h1>
  <p align="center">
    Open-Source Real-Time current measurement device firmware
  </p>
</p>

## Getting started
To get started with the uMeasurement device you will need a couple pieces of software and a piece of hardware.

### Software 💻
At a minium you will need the ARM GNU GCC Toolchain to compile the source and generate executable artifacts.
-   [ARM GNU GCC Toolchain - 10-2020-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)

To flash and debug the target this project uses a couple of different tools. PyOCD is used for the programming/debugging medium, and a VScode extension called Cortex-Debug gives a user interface within VScode for stepping through the source, settings breakpoints and viewing registers/variables within the target.
-   [PyOCD](https://github.com/pyocd/pyOCD#installing)
-   [VScode](https://code.visualstudio.com/)
-   [Cortex-Debug VScode extenstion](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)

### Hardware ⚡
The uMeasurement hardware is based on the [LCP55xx](https://www.nxp.com/part/LPC5526JBD64#/) family of microcontrollers. There are two options for acquiring a target compatible with the uMeasurement firmware. The first and best option is a piece of the uMeasurement hardware itself or you can acquire and LCPXpresso dev kit for the LPC55S28. The downside is that the dev kit does not contain the necessary hardware to do differential analog measurements.
- [uMeasurement hardware](https://github.com/glassboard-dev/gl-umeasurement-hardware)
- [LPCXpresso55S28](https://www.nxp.com/design/software/development-software/lpcxpresso55s28-development-board:LPC55S28-EVK)

You will also need a debug probe for flashing and debugging. There are several probes compatible with PyOCD and Glasslabs provides one of these options that also makes the necessary connections for sending UART debug data out of the target when connecting to the uMeasurement hardware.
- [GlassLink Debug Probe](https://github.com/glassboard-dev/gl-glass-link-hardware)

## Setup ✔️
After a first time install of PyOCD you will need to install the necessary CMSIS Pack for your target microcontroller.
```bash
# Install the pack for our LPC5526 micro
$ pyocd pack -i lpc5526
# optionally install the pack for the LPC55S28 micro
$ pyocd pack -i lpc55s28
```

To begin compiling and developing the firmware, move to the armgcc directory and execute the setup script, passing the PN you wish to generate a Makefile for (LPC5526 or LPC55S28) and the PATH to your desired ARM GCC toolchain. The uMeasurement board uses the LPC5526 and the LPCXpresso dev kit uses the LPC55S28 micro.
```bash
$ cd armgcc
$ ./setup.sh LPC5526 /usr/local/gcc-arm-none-eabi-10-2020-q4-major
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