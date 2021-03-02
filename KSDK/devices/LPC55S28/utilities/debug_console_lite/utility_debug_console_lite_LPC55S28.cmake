if(NOT UTILITY_DEBUG_CONSOLE_LITE_LPC55S28_INCLUDED)
    
    set(UTILITY_DEBUG_CONSOLE_LITE_LPC55S28_INCLUDED true CACHE BOOL "utility_debug_console_lite component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_debug_console.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(component_usart_adapter_LPC55S28)

    include(driver_common_LPC55S28)

endif()
