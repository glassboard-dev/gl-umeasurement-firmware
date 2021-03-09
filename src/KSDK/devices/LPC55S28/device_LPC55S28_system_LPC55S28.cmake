if(NOT DEVICE_LPC55S28_SYSTEM_LPC55S28_INCLUDED)
    
    set(DEVICE_LPC55S28_SYSTEM_LPC55S28_INCLUDED true CACHE BOOL "device_LPC55S28_system component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_LPC55S28.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(device_LPC55S28_CMSIS_LPC55S28)

endif()
