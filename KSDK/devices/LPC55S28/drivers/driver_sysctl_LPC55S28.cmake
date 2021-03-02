if(NOT DRIVER_SYSCTL_LPC55S28_INCLUDED)
    
    set(DRIVER_SYSCTL_LPC55S28_INCLUDED true CACHE BOOL "driver_sysctl component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_sysctl.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_LPC55S28)

endif()
