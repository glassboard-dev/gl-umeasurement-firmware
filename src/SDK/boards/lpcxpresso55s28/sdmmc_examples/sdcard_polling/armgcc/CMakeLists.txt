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

# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR})

SET(EXECUTABLE_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${ProjDirPath}/${CMAKE_BUILD_TYPE})


project(sdcard_polling)

set(MCUX_SDK_PROJECT_NAME sdcard_polling.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../sdcard_polling.c"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../sdmmc_config.c"
"${ProjDirPath}/../sdmmc_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../devices/LPC55S28/drivers
    ${ProjDirPath}/../../../../../middleware/sdmmc
    ${ProjDirPath}/../../../../../devices/LPC55S28/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../devices/LPC55S28
    ${ProjDirPath}/../../../../../components/uart
    ${ProjDirPath}/../../../../../components/lists
    ${ProjDirPath}/../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../components/osa
    ${ProjDirPath}/../../../../../devices/LPC55S28/utilities
)

# include modules
include(driver_lpc_iocon_LPC55S28)

include(driver_sdif_LPC55S28)

include(middleware_sdmmc_sd_LPC55S28)

include(middleware_sdmmc_common_LPC55S28)

include(middleware_sdmmc_host_sdif_LPC55S28)

include(middleware_sdmmc_host_sdif_polling_LPC55S28)

include(driver_flexcomm_usart_LPC55S28)

include(utility_debug_console_lite_LPC55S28)

include(utility_assert_lite_LPC55S28)

include(driver_clock_LPC55S28)

include(driver_common_LPC55S28)

include(device_LPC55S28_CMSIS_LPC55S28)

include(device_LPC55S28_startup_LPC55S28)

include(driver_flexcomm_LPC55S28)

include(driver_lpc_gpio_LPC55S28)

include(driver_power_LPC55S28)

include(component_usart_adapter_LPC55S28)

include(component_lists_LPC55S28)

include(CMSIS_Include_core_cm33_LPC55S28)

include(CMSIS_Include_common_LPC55S28)

include(CMSIS_Include_dsp_LPC55S28)

include(driver_reset_LPC55S28)

include(middleware_sdmmc_osa_bm_LPC55S28)

include(component_osa_bm_LPC55S28)

include(utilities_misc_utilities_LPC55S28)

include(device_LPC55S28_system_LPC55S28)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../devices/LPC55S28/gcc/libpower_hardabi.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../devices/LPC55S28/gcc/libpower_hardabi.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


