if(NOT UTILITY_SHELL_LPC55S28_INCLUDED)
    
    set(UTILITY_SHELL_LPC55S28_INCLUDED true CACHE BOOL "utility_shell component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_shell.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(utility_debug_console_LPC55S28)

    include(component_lists_LPC55S28)

    include(driver_common_LPC55S28)

endif()
