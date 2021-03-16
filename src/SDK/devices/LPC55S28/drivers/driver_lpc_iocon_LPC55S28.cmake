if(NOT DRIVER_LPC_IOCON_LPC55S28_INCLUDED)
    
    set(DRIVER_LPC_IOCON_LPC55S28_INCLUDED true CACHE BOOL "driver_lpc_iocon component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_common_LPC55S28)

endif()
