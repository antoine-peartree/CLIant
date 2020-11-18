cmake_minimum_required(VERSION 3.16.3)

project(nucleo-l552ze-q)

enable_language(C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(STM32CUBEIDE_GENERATED_FILES
        Core/Src/stm32l5xx_hal_msp.c
        Core/Src/stm32l5xx_it.c
# Syscalls replaced by retarget.c (allow printf into UART)
#Core/Src/syscalls.c
        Core/Src/sysmem.c
        Core/Src/system_stm32l5xx.c
        Core/Startup/startup_stm32l552zetxq.s
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_cortex.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_dma.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_dma_ex.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_exti.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_flash.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_flash_ex.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_flash_ramfunc.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_gpio.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_i2c.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_i2c_ex.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_icache.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_pwr.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_pwr_ex.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_rcc.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_rcc_ex.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_rtc.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_rtc_ex.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_tim.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_tim_ex.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_uart.c
        Drivers/STM32L5xx_HAL_Driver/Src/stm32l5xx_hal_uart_ex.c
    )

set(USER_SRC_FILES
        main.c
        ../cliant_c/cliant.c
        User/Drivers/Src/leds.c
        User/Core/Src/retarget.c
   )

set(EXECUTABLE ${PROJECT_NAME}.out)

add_executable(${EXECUTABLE} ${STM32CUBEIDE_GENERATED_FILES} ${USER_SRC_FILES})

target_compile_definitions(${EXECUTABLE} PRIVATE
        -DUSE_HAL_DRIVER
        -DSTM32L552xx
        )

target_include_directories(${EXECUTABLE} PRIVATE
    Core/Inc
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32L5xx/Include
    Drivers/STM32L5xx_HAL_Driver/Inc
    User/Core/Inc
    User/Drivers/Inc
    ../cliant_c
    )

target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m33
        -mthumb
        -mfpu=fpv5-sp-d16
        -mfloat-abi=hard
        -ffunction-sections
        -fdata-sections
        -Wall

        $<$<CONFIG:Debug>:-Og>
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/ld_config/STM32L552ZETXQ_FLASH.ld
        -mcpu=cortex-m33
        -mthumb
        -mfpu=fpv5-sp-d16
        -mfloat-abi=hard
        -specs=nosys.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.amp,--cref
        -Wl,--gc-sections
        )

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE}
        )

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
    )
