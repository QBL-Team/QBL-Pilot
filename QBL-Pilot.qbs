import qbs
import CortexMProduct

CortexMProduct{
    useFPU: true
    type: ["application","size"]

    cpp.includePaths:[
        "Drivers/CMSIS/Device/ST/STM32F4xx/Include",
        "Drivers/CMSIS/Include",
        "Drivers/Device",
        "Drivers/STM32F4xx_HAL_Driver/Inc",
        "Inc",
        "Middlewares",
        "Middlewares/mavlink",
        "Middlewares/mavlink/common",
        "Middlewares/Third_Party/FatFs/src",
        "Middlewares/Third_Party/FatFs/src/drivers",
    ]


    files: [
        "GNU-GCC/*.s",
        "GNU-GCC/*.c",
        "Src/*.c",
        "Drivers/STM32F4xx_HAL_Driver/Src/*.c",
        "Drivers/Device/*.c",
        "Middlewares/Third_Party/FatFs/src/*.c",
        "Middlewares/Third_Party/FatFs/src/drivers/*.c",
        "Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/*.c",
        "Middlewares/*.c",
    ]

    cpp.defines :[
        "USE_HAL_DRIVER",
        "STM32F407xx"
    ]

    cpp.linkerScripts:[
        "GNU-GCC/*.ld"
    ]

}
