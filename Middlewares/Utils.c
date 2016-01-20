#include "Utils.h"
#include "stm32f4xx_hal.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "HMC5883.h"
#include "LED.h"
#include "MPU6050.h"
#include "MS5611.h"
#include "W25QXX.h"
#include "PWMInput.h"
#include "PWMOutput.h"
#include "TimeMeter.h"

static uint16_t utils_system_init_state_flag = 0;

#define UTILS_SYSTEM_DEVICE_ALL_FAIL        (0 << 0)
#define UTILS_SYSTEM_DEVICE_ACCELEROMETER   (1 << 0)
#define UTILS_SYSTEM_DEVICE_GYROSCOPE       (1 << 1)
#define UTILS_SYSTEM_DEVICE_BAROMETER       (1 << 2)
#define UTILS_SYSTEM_DEVICE_MAGNETICMETER   (1 << 3)
#define UTILS_SYSTEM_DEVICE_FLASH           (1 << 4)
#define UTILS_SYSTEM_DEVICE_SDCARD          (1 << 5)

static void Utils_SystemInitCheck(uint16_t state_flag, const char * str_ok, const char * str_fail)
{
    if(utils_system_init_state_flag & state_flag)
    {
        printf(str_ok);
    }
    else
    {
        printf(str_fail);
    }
}

void Utils_RunSelfTest(void)
{

    printf("Now testing LED display,LONG press KEY to continue...\r\n");

    
    for(uint8_t i = 0; i <= LED_COLOR_CYAN; i++)
    {
        LED_Show(i);
        HAL_Delay(500);
        
        if(i == LED_COLOR_CYAN)
        {
            i = 0;
        }
        
        if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key_GPIO_Port, Key_Pin))
        {
            break;
        }
    }

    LED_Show(LED_COLOR_WHITE);
    
    printf("\r\nNow sensor initialize state check...\r\n");

    Utils_SystemInitCheck(UTILS_SYSTEM_DEVICE_ACCELEROMETER,
                          "Accelerometer initialize succeed !\r\n",
                          "Accelerometer initialize failed !\r\n");


    Utils_SystemInitCheck(UTILS_SYSTEM_DEVICE_GYROSCOPE,
                          "Gyroscope initialize succeed !\r\n",
                          "Gyroscope initialize failed !\r\n");


    Utils_SystemInitCheck(UTILS_SYSTEM_DEVICE_BAROMETER,
                          "Barometer initialize succeed !\r\n",
                          "Barometer initialize failed !\r\n");


    Utils_SystemInitCheck(UTILS_SYSTEM_DEVICE_MAGNETICMETER,
                          "Magneticmeter initialize succeed !\r\n",
                          "Magneticmeter initialize failed !\r\n");


    Utils_SystemInitCheck(UTILS_SYSTEM_DEVICE_FLASH,
                          "Flash initialize succeed !\r\n",
                          "Flash initialize failed !\r\n");


    Utils_SystemInitCheck(UTILS_SYSTEM_DEVICE_SDCARD,
                          "SDCard initialize succeed !\r\n",
                          "SDCard initialize failed !\r\n");

    printf("\r\nNow sensor reading check...\r\n");


    float tmp[7];

    if(MPU6050_Update(tmp, &tmp[6]))
    {
        printf("Accelerometer reading ok...\r\n");
        printf("ax = %f,ay = %f,az = %f\r\n", tmp[0], tmp[1], tmp[2]);
        printf("Gyroscope reading ok...\r\n");
        printf("gx = %f,gy = %f,gz = %f\r\n", tmp[3], tmp[4], tmp[5]);
    } else {
        printf("Accelerometer reading failed...\r\n");
        printf("Gyroscope reading failed...\r\n");
    }

    if(HMC5883_Update(tmp))
    {
        printf("Magneticmeter reading ok...\r\n");
        printf("mx = %f,my = %f,mz = %f\r\n", tmp[0], tmp[1], tmp[2]);
    } else {
        printf("Magneticmeter reading failed...\r\n");
    }

    uint32_t tick = HAL_GetTick();
    
    for(;;)
    {
        if(MS5611_Update(&tmp[0], &tmp[1]))
        {
            printf("Barometer reading ok...\r\n");
            printf("Pressure = %f,Temperature = %f\r\n", tmp[0], tmp[1]);
            break;
        }

        if(HAL_GetTick() - tick > 1000)
        {
            printf("Barometer reading failed...\r\n");
            break;
        }
    }
    
    printf("\r\nQBL-Pilot sel test completed !\r\n");
}

void Utils_SystemInit(void)
{
    utils_system_init_state_flag = UTILS_SYSTEM_DEVICE_ALL_FAIL;
    LED_Show(LED_COLOR_OFF);
    PWMOutput_Init(&htim1, &htim3);
    PWMInput_Init(&htim2, &htim4);
    TimeMeter_Init(&htim6);

    if(MPU6050_Init(&hi2c1))
    {
        utils_system_init_state_flag |= UTILS_SYSTEM_DEVICE_ACCELEROMETER;
        utils_system_init_state_flag |= UTILS_SYSTEM_DEVICE_GYROSCOPE;
    }

    if(HMC5883_Init(&hi2c1))
    {
        utils_system_init_state_flag |= UTILS_SYSTEM_DEVICE_MAGNETICMETER;
    }

    if(MS5611_Init(&hspi2))
    {
        utils_system_init_state_flag |= UTILS_SYSTEM_DEVICE_BAROMETER;
    }
    if(W25Q_Init(&hspi1))
    {
        utils_system_init_state_flag |= UTILS_SYSTEM_DEVICE_FLASH;
    }

    if(HAL_OK == HAL_SD_Init(&hsd, &SDCardInfo))
    {
        utils_system_init_state_flag |= UTILS_SYSTEM_DEVICE_SDCARD;
    }
}
