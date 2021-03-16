if(NOT DEVICE_LPC55S28_CMSIS_LPC55S28_INCLUDED)
    
    set(DEVICE_LPC55S28_CMSIS_LPC55S28_INCLUDED true CACHE BOOL "device_LPC55S28_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm33_LPC55S28)

endif()
