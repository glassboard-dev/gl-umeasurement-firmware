# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED (VERSION 3.1.1)

# THE VERSION NUMBER
SET (Tutorial_VERSION_MAJOR 1)
SET (Tutorial_VERSION_MINOR 0)

# ENABLE ASM
ENABLE_LANGUAGE(ASM)

SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)

project(uMeasurement)

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${TARGET_MCU})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE}/${TARGET_MCU})

IF(${TARGET_MCU} MATCHES LPC55S28)
    set(TARGET_CPU_D CPU_LPC55S28JBD100)
ELSE()
    set(TARGET_CPU_D CPU_LPC5526JBD64)
ENDIF()

set(MCUX_SDK_PROJECT_NAME ${CMAKE_PROJECT_NAME}.elf)

include(${ProjDirPath}/flags.cmake)
include(${ProjDirPath}/config.cmake)

set(INCLUDES    ${ProjDirPath}/../src
                ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}
                ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/drivers
                ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/utilities
                ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/utilities/debug_console
                ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/utilities/debug_console_lite
                ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/utilities/str
                ${ProjDirPath}/../KSDK/components/uart
                ${ProjDirPath}/../KSDK/components/lists
                ${ProjDirPath}/../KSDK/components/serial_manager
                ${ProjDirPath}/../KSDK/CMSIS/Include
)

include_directories(${INCLUDES})

set(APP_SRC ${ProjDirPath}/../src/led_blinky.c
            ${ProjDirPath}/../src/pin_mux.c
            ${ProjDirPath}/../src/board.c
            ${ProjDirPath}/../src/clock_config.c
)

FILE(GLOB KSDK_DRIVER_SRC   ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/*.c
                            ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/gcc/*.S
                            ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/drivers/*.c
                            ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/utilities/*.c
                            ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/utilities/debug_console/*.c
                            ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/utilities/debug_console_lite/*.c
                            ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/utilities/str/*.c
                            ${ProjDirPath}/../KSDK/components/uart/*.c
                            ${ProjDirPath}/../KSDK/components/lists/*.c
                            ${ProjDirPath}/../KSDK/components/serial_manager/*.c
)

add_executable(${MCUX_SDK_PROJECT_NAME}
    ${KSDK_DRIVER_SRC}
    ${APP_SRC}
)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)
target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/gcc/libpower_hardabi.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../KSDK/devices/${TARGET_MCU}/gcc/libpower_hardabi.a)
endif()

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)

# Add a makefile "target" to erase our part
add_custom_target(erase pyocd erase -t ${TARGET_MCU} --chip)

# Add a makefile "target" to flash the micro
add_custom_target(flash pyocd flash -t ${TARGET_MCU} -f 2000khz ${EXECUTABLE_OUTPUT_PATH}/${MCUX_SDK_PROJECT_NAME})