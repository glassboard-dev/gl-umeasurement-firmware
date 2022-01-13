<p align="center">
  <h1 align="center">µMeasurement</h1>
  <p align="center">
    Open-Source Real-Time current measurement device firmware
  </p>
</p>

<!-- REPLACE YOUR BADGE AND CODECOV URL HERE FOR CODE COVERAGE IN THE README -->
<!-- [![codecov](https://codecov.io/gh/glassboard-dev/thermaquil-quantum-controller-carrier-firmware/branch/develop/graph/badge.svg?token=4HFQ910HXS)](https://codecov.io/gh/glassboard-dev/thermaquil-quantum-controller-carrier-firmware) -->

### Tools Setup ✔️
At a minium you will need the ARM GNU GCC Toolchain, Make and CMake to compile the source and generate executable artifacts.
-   [ARM GNU GCC Toolchain - 10-2020-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) - Other versions can be used but the current source is actively developed and tested using the 10-2020-q4-major release
-   Make -  Make is shipped with Linux and should already be present on the developers system. If the developer is running Windows they can either use WSL to run a Linux instance or install Make on Windows via [MinGW](https://sourceforge.net/projects/mingw/)
-   [CMake](https://cmake.org/download/)

To flash and debug the target this project uses a couple of different tools. PyOCD is used for the programming/debugging medium, and a VScode extension called Cortex-Debug gives a user interface within VScode for stepping through the source, settings breakpoints and viewing registers/variables within the target.
-   [PyOCD](https://github.com/pyocd/pyOCD#installing) - Version 0.30.5 or newer is required when programming/debugging the LPC55x microcontrollers.
-   [VScode](https://code.visualstudio.com/)
-   [Cortex-Debug VScode extenstion](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)

To write and execute unit tests on the firmware source, you will need to install the Ceedling unit test framework and the Python package gcovr for generate Cobertura html reports. Ceedling is a Ruby Gem, so Ruby must first be installed on your system. GCC and Make are also needed. If the developer is on Windows, you can find install instructions above.
-   [Ruby](https://www.ruby-lang.org)
    - Windows - [RubyInstaller 2.7.3-1](https://github.com/oneclick/rubyinstaller2/releases/download/RubyInstaller-2.7.3-1/rubyinstaller-2.7.3-1-x64.exe)
    - Linux - ```$ sudo apt-get install ruby```
-   [Ceedling](http://www.throwtheswitch.org/ceedling)
-   [Python](https://www.python.org/downloads/)
-   [gcovr](https://gcovr.com/en/stable/)
    - ```python -m pip install gcovr```

### Adding new source files 📁
To add a new source file to the ```src/``` folder: From the root directory, use the Ceedling command below to create a new source ```.c``` and ```.h``` file. This will also create a unit test file in the ```test/``` folder for you.
```c
// Don't include a .c or .h in the filename
$ ceedling module:create[FILENAME]
```

### Compiling, Flashing & Testing ✨
After a fresh clone, the ```setup``` scripts can be used to initialize the project. Passing ```debug``` as an argument will generate a debug build. Otherwise a ```release``` build is generated.
##### Windows
```console
setup.ps1 debug
```
##### Linux
```console
chmod +x ./setup.sh
./setup.sh debug
```

Once a project has been initialized, the following commands should be ran from the ```build/``` folder

To clean the working directory:
```console
make clean
```

To compile the source:
```console
make -j8
```

To erase the connected target:
```console
make erase
```

To flash the connected target:
```console
make flash
```

To execute unit tests:
```console
make test
```

### Source Documentation Style 📃
Read more about the Source documentation style [here](./doc/DOCUMENTATION_STYLE.md)

### Develop Strategy 📖
Read more about the Git develop strategy used [here](./doc/DEVELOP_STRATEGY.md)
