if(NOT DRIVER_WWDT_LPC55S28_INCLUDED)
    
    set(DRIVER_WWDT_LPC55S28_INCLUDED true CACHE BOOL "driver_wwdt component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_wwdt.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_LPC55S28)

endif()
