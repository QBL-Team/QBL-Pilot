/*!
 * \file HMC5883.c
 * \author no1wudi
 */

#include "HMC5883.h"

/*!
  *
  * @defgroup hmc5883_driver HMC5883 驱动
  * @{
  */


#define HMC5883_IIC_ADDRESS     0x3C        ///< 芯片的IIC地址，参见其数据手册
#define HMC5883_DATA_SENS       440.0f      ///< 原始数据到真实数据的缩放比例，参见其数据手册
#define HMC5883_REG_DATA_ADDR   0x03        ///< 数据寄存器起始地址
#define HMC5883_REG_DATA_LENGTH 6           ///< 读取的数据量，单位为byte
#define HMC5883_REG_ADDR_WIDTH  1           ///< 寄存器地址的宽度，单位为byte

/*!
 *\brief 数据配置表，左侧为寄存器，右侧为参数值，具体参见其数据手册
 */

static const uint8_t HMC5883_CONFIG_TABLE[] =
{
    0x00, 0x18,
    0x01, 0x80,
    0x02, 0x00,
};


/*!
 * \brief iic_handle IIC总线的句柄，用于读写传感器操作
 */
static I2C_HandleTypeDef * iic_handle = NULL;

/*!
 * \brief HMC5883_Init  初始化HMC5883L
 * \param handle    IIC总线的句柄，用于操作总线读写寄存器
 * \return  如果初始化成功，返回true
 */

bool HMC5883_Init(I2C_HandleTypeDef * handle)
{

    uint8_t cnts = 0;

    iic_handle = handle;
    for(; cnts < sizeof(HMC5883_CONFIG_TABLE); cnts += 2)
    {
        if(HAL_OK == HAL_I2C_Mem_Write(iic_handle, HMC5883_IIC_ADDRESS, HMC5883_CONFIG_TABLE[cnts], HMC5883_REG_ADDR_WIDTH , (uint8_t *)(HMC5883_CONFIG_TABLE + 1 + cnts), 1, 1))
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
 * \brief HMC5883_Update 读取最新的传感器数据
 * \param axis 数组的长度为3，数组的元素0~2分别对应X,Y,Z三个轴
 * \return 如果读取成功，返回true
 */

bool HMC5883_Update(float axis[3])
{
    uint8_t tmp[6];
    if(HAL_OK == HAL_I2C_Mem_Read(iic_handle, HMC5883_IIC_ADDRESS, HMC5883_REG_DATA_ADDR, HMC5883_REG_ADDR_WIDTH, tmp, HMC5883_REG_DATA_LENGTH, 5))
    {

    }
    else
    {
        return false;
    }

    axis[0] = ((int16_t)((tmp[0] << 8) | tmp[1])) / HMC5883_DATA_SENS;
    axis[1] = ((int16_t)((tmp[4] << 8) | tmp[5])) / HMC5883_DATA_SENS;
    axis[2] = ((int16_t)((tmp[2] << 8) | tmp[3])) / HMC5883_DATA_SENS;

    return true;
}

/*!
 * @}
 */
