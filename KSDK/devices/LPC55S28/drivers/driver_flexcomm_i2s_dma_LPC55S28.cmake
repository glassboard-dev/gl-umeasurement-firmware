if(NOT DRIVER_FLEXCOMM_I2S_DMA_LPC55S28_INCLUDED)
    
    set(DRIVER_FLEXCOMM_I2S_DMA_LPC55S28_INCLUDED true CACHE BOOL "driver_flexcomm_i2s_dma component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_i2s_dma.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_flexcomm_i2s_LPC55S28)

    include(driver_lpc_dma_LPC55S28)

endif()
