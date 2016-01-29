cmake_minimum_required(VERSION 3.3)
project(QBL-Pilot)

enable_language(C)
enable_language(ASM)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m4)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Build)

include(CortexM.CMake)

cortex_set_tool_chain(
    "D:\\Develop\\GCC_ARM\\bin\\"
    "arm-none-eabi"
    "${CMAKE_SOURCE_DIR}/GNU-GCC/STM32F407VG_FLASH.ld"
    )

cortex_enable_fpu()

include_directories(Inc)
include_directories(Src)
include_directories(Drivers/CMSIS/Include)
include_directories(Drivers/CMSIS/Device/ST/STM32F4xx/Include)
include_directories(Drivers/STM32F4xx_HAL_Driver/Inc)
include_directories(Drivers/STM32F4xx_HAL_Driver/Inc/Legacy)
include_directories(Middlewares/mavlink)
include_directories(Middlewares/mavlink/common)
include_directories(Middlewares/Third_Party/FatFs/src)
include_directories(Middlewares/Third_Party/FatFs/src/drivers)
include_directories(Drivers/Device)
include_directories(Middlewares)

add_definitions("-DUSE_HAL_DRIVER" "-DSTM32F407xx")

aux_source_directory(Src SOURCE_FILES)
aux_source_directory(Inc SOURCE_FILES)
aux_source_directory(Middlewares SOURCE_FILES)
aux_source_directory(Drivers/Device SOURCE_FILES)
aux_source_directory(Drivers/STM32F4xx_HAL_Driver/Src SOURCE_FILES)
aux_source_directory(Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates SOURCE_FILES)
aux_source_directory(Middlewares/Third_Party/FatFs/Src SOURCE_FILES)
aux_source_directory(Middlewares/Third_Party/FatFs/Src/drivers SOURCE_FILES)


set(SOURCE_FILES
    ${SOURCE_FILES}
    GNU-GCC/startup_stm32f407xx.s
    GNU-GCC/syscalls.c
    )

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})
cortex_build_info()
cortex_build_bin()
