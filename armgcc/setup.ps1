# Cleanup any old Cmake artifacts
if( Test-Path -Path './CMakeFiles' ) {Remove-Item './CMakeFiles' -Recurse}
if( Test-Path -Path './debug' ) {Remove-Item './debug' -Recurse}
if( Test-Path -Path './release' ) {Remove-Item './release' -Recurse}
if( Test-Path './Makefile' ) {Remove-Item './Makefile'}
if( Test-Path './cmake_install.cmake' ) {Remove-Item './cmake_install.cmake'}
if( Test-Path './CmakeCache.txt' ) {Remove-Item './CmakeCache.txt'}
if( Test-Path './output.map' ) {Remove-Item './output.map'}

# Store our command line arguments
$mcu_pn=$args[0];
$toolchain=$args[1];

# Determine the MCU PN
if( $mcu_pn -eq $null) {
    Write-Output "Please provide the PN you want to generate a Makefile for. Available options: LPC5526 or LPC55S28"
}
elseif( ($mcu_pn -eq "LPC55S28") -or ($mcu_pn -eq "LPC5526") ) {
    # Export our ARMGCC path
    if( $toolchain -eq $null ) {
        Write-Output "Using default ARM GCC toolchain path: 'C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2020-q4-major'"
        $env:ARMGCC_DIR = 'C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2020-q4-major'
    }
    else {
        Write-Output "Argument given: $($toolchain)"
        $env:ARMGCC_DIR = $toolchain
    }

    if( $mcu_pn -eq "LPC55S28" ) {
        cmake -DCMAKE_TOOLCHAIN_FILE="./armgcc.cmake" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=debug  -DTARGET_MCU=LPC55S28 .
    }
    else {
        cmake -DCMAKE_TOOLCHAIN_FILE="./armgcc.cmake" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=debug  -DTARGET_MCU=LPC5526 .
    }
}
else {
    Write-Output "Invalid PN provided. Available options: LPC5526 or LPC55S28"
}

