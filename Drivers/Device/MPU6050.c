/*!
 * \file MPU6050.c
 * \author no1wudi
 */

/*!
  *
  * @defgroup mpu6050_driver MPU6050 驱动
  * @{
  */


#include "MPU6050.h"

/*!
 * \brief iic_handle 内部句柄，指向挂载传感器的IIC总线
 */
static I2C_HandleTypeDef * iic_handle = NULL;

#define MPU6050_IIC_ADDRESS     0xD0        ///< 芯片的IIC地址，参见其数据手册
#define MPU6050_GYRO_SCALE      32.8f       ///< 陀螺仪的数据缩放比例
#define MPU6050_ACCE_SCALE      4096.0f     ///< 加速度计的数据缩放比例
#define MPU6050_CHIP_ID         0x68        ///< 芯片的ID
#define MPU6050_REG_ID_ADDR     0x75        ///< 芯片ID寄存器的地址
#define MPU6050_REG_DATA_ADDR   0x3B        ///< 数据寄存器的起始地址
#define MPU6050_REG_DATA_LENGTH 14          ///< 将要读取的数据的长度，单位为byte
#define MPU6050_REG_ADDR_WIDTH  1           ///< 寄存器地址的宽度，单位为byte

/*!
 *\brief 传感器的配置表，左边为寄存器地址，右边为具体的配置值
 */

static uint8_t MPU_CONFIG_TABLE[] = {
    0x6B, 0x00,  //PWM MGMT 1
    0x1B, 0x10,  //GYRO Range 0 - 1000
    0x1C, 0x10,  //Acce Range 0 - 8g
    0x1A, 0x00,  //Low pass filter
    0x6A, 0x00,
    0x37, 0x02
};

/*!
 * \brief MPU6050_Init 初始化传感器
 * \param handle    挂载传感器的IIC总线的句柄
 * \return  如果初始化成功，返回true
 */

bool MPU6050_Init(I2C_HandleTypeDef * handle)
{
    uint8_t cnts = 0;       //temp counter
    uint8_t tmp = 0;        //temp variable

    iic_handle = handle;

    HAL_I2C_Mem_Read(iic_handle, MPU6050_IIC_ADDRESS, MPU6050_REG_ID_ADDR, MPU6050_REG_ADDR_WIDTH, &tmp, 1, 1);

    if(MPU6050_CHIP_ID != tmp)
    {
        return false;
    }

    for(cnts = 0; cnts < sizeof(MPU_CONFIG_TABLE); cnts += 2)
    {
        if(HAL_OK == HAL_I2C_Mem_Write(iic_handle, MPU6050_IIC_ADDRESS, MPU_CONFIG_TABLE[cnts], MPU6050_REG_ADDR_WIDTH , (uint8_t *)(MPU_CONFIG_TABLE + 1 + cnts), 1, 1))
        {

        }
        else
        {
            return false;
        }
    }

    return true;
}

/*!
 * \brief MPU6050_Update    从传感器中读取最新的数据
 * \param axis  本数组长度为6，元素0~2对应于加速度计的X,Y,Z三个轴；元素3~5对应于陀螺仪的X,Y,Z三个轴
 * \param temp 读取传感器的温度，如果不需要，将本参数设置为NULL
 * \return  如果初始化成功，返回true
 */

bool MPU6050_Update(float axis[6], float * temp)
{
    uint8_t tmp[MPU6050_REG_DATA_LENGTH];

    if(HAL_OK != HAL_I2C_Mem_Read(iic_handle, MPU6050_IIC_ADDRESS, MPU6050_REG_DATA_ADDR, 1, tmp, MPU6050_REG_DATA_LENGTH, 1))
    {
        return false;
    }

    //Calculate temp
    if(NULL != temp)
    {
        *temp = (int16_t)((tmp[6] << 8) + tmp[7]) / 340.0f + 36.53f;
    }

    axis[0] = (int16_t)((tmp[0] << 8) | tmp[1]);
    axis[1] = (int16_t)((tmp[2] << 8) | tmp[3]);
    axis[2] = (int16_t)((tmp[4] << 8) | tmp[5]);
    axis[3] = (int16_t)((tmp[8] << 8) | tmp[9]);
    axis[4] = (int16_t)((tmp[10] << 8) | tmp[11]);
    axis[5] = (int16_t)((tmp[12] << 8) | tmp[13]);

    //Acce data normalize ~ x,y,z
    axis[0] /= MPU6050_ACCE_SCALE;
    axis[1] /= MPU6050_ACCE_SCALE;
    axis[2] /= MPU6050_ACCE_SCALE;

    //Gyro data normalize ~ x,y,z
    axis[3] /= MPU6050_GYRO_SCALE;
    axis[4] /= MPU6050_GYRO_SCALE;
    axis[5] /= MPU6050_GYRO_SCALE;

    return true;
}

/*!
 * @}
 */
