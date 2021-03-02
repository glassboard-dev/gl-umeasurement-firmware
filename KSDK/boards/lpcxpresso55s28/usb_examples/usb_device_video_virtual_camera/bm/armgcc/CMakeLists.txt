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


project(dev_video_virtual_camera_bm)

set(MCUX_SDK_PROJECT_NAME dev_video_virtual_camera_bm.elf)

include(${ProjDirPath}/flags.cmake)

include(${ProjDirPath}/config.cmake)

add_executable(${MCUX_SDK_PROJECT_NAME} 
"${ProjDirPath}/../usb_device_descriptor.c"
"${ProjDirPath}/../usb_device_descriptor.h"
"${ProjDirPath}/../video_data.c"
"${ProjDirPath}/../video_data.h"
"${ProjDirPath}/../virtual_camera.c"
"${ProjDirPath}/../virtual_camera.h"
"${ProjDirPath}/../usb_device_config.h"
"${ProjDirPath}/../board.c"
"${ProjDirPath}/../board.h"
"${ProjDirPath}/../clock_config.c"
"${ProjDirPath}/../clock_config.h"
"${ProjDirPath}/../pin_mux.c"
"${ProjDirPath}/../pin_mux.h"
"${ProjDirPath}/../usb_device_ch9.c"
"${ProjDirPath}/../usb_device_ch9.h"
"${ProjDirPath}/../usb_device_class.c"
"${ProjDirPath}/../usb_device_class.h"
"${ProjDirPath}/../usb_device_video.c"
"${ProjDirPath}/../usb_device_video.h"
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${ProjDirPath}/..
)

set(CMAKE_MODULE_PATH
    ${ProjDirPath}/../../../../../../middleware/usb
    ${ProjDirPath}/../../../../../../devices/LPC55S28/utilities/debug_console_lite
    ${ProjDirPath}/../../../../../../middleware
    ${ProjDirPath}/../../../../../../devices/LPC55S28/drivers
    ${ProjDirPath}/../../../../../../devices/LPC55S28
    ${ProjDirPath}/../../../../../../components/uart
    ${ProjDirPath}/../../../../../../components/lists
    ${ProjDirPath}/../../../../../../components/osa
    ${ProjDirPath}/../../../../../../CMSIS/Include
    ${ProjDirPath}/../../../../../../devices/LPC55S28/utilities
)

# include modules
include(middleware_usb_device_ip3511fs_LPC55S28)

include(utility_debug_console_lite_LPC55S28)

include(utility_assert_lite_LPC55S28)

include(middleware_baremetal_LPC55S28)

include(driver_clock_LPC55S28)

include(driver_common_LPC55S28)

include(device_LPC55S28_CMSIS_LPC55S28)

include(device_LPC55S28_startup_LPC55S28)

include(driver_flexcomm_usart_LPC55S28)

include(driver_flexcomm_LPC55S28)

include(driver_lpc_gpio_LPC55S28)

include(driver_lpc_iocon_LPC55S28)

include(driver_power_LPC55S28)

include(driver_reset_LPC55S28)

include(middleware_usb_phy_LPC55S28)

include(component_usart_adapter_LPC55S28)

include(component_lists_LPC55S28)

include(middleware_usb_device_controller_driver_LPC55S28)

include(component_osa_LPC55S28)

include(middleware_usb_device_ip3511hs_LPC55S28)

include(middleware_usb_device_common_header_LPC55S28)

include(CMSIS_Include_core_cm33_LPC55S28)

include(CMSIS_Include_common_LPC55S28)

include(CMSIS_Include_dsp_LPC55S28)

include(middleware_usb_common_header_LPC55S28)

include(utilities_misc_utilities_LPC55S28)

include(device_LPC55S28_system_LPC55S28)


TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--start-group)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE m)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE c)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE gcc)

target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE nosys)

if(CMAKE_BUILD_TYPE STREQUAL release)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../devices/LPC55S28/gcc/libpower_hardabi.a)
endif()

if(CMAKE_BUILD_TYPE STREQUAL debug)
    target_link_libraries(${MCUX_SDK_PROJECT_NAME} PRIVATE ${ProjDirPath}/../../../../../../devices/LPC55S28/gcc/libpower_hardabi.a)
endif()

TARGET_LINK_LIBRARIES(${MCUX_SDK_PROJECT_NAME} PRIVATE -Wl,--end-group)


