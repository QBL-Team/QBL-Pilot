/*!
 * \file HMC5883.c
 * \author no1wudi
 */

#include "HMC5883.h"

/*!
  *
  * @defgroup hmc5883_driver HMC5883 Driver
  * @{
  */


#define HMC5883_IIC_ADDRESS     0x3C        ///< Chip's iic address
#define HMC5883_DATA_SENS       440.0f      ///< Data scaler
#define HMC5883_REG_DATA_ADDR   0x03        ///< Data registers' address
#define HMC5883_REG_DATA_LENGTH 6           ///< How many data registers to be read
#define HMC5883_REG_ADDR_WIDTH  1           ///< width of chip's register address


/*!
 *\brief Configure table of the chip,for more informatin please refer to its datasheet
 */

static const uint8_t HMC5883_CONFIG_TABLE[] =
{
    0x00, 0x18,
    0x01, 0x80,
    0x02, 0x00,
};


/*!
 * \brief iic_handle Internal handle for iic bus
 */
static I2C_HandleTypeDef * iic_handle = NULL;

/*!
 * \brief HMC5883_Init Initialize the HMC5883
 * \param handle    Handle pointed to the IIC bus
 * \return  Return true if init succeed
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
 * \brief HMC5883_Update Get newest data from sensor
 * \param axis Axis array contains 3 data ,0 - 2 mapped to MagnX - MagnY - MagnZ,
 * \return If read succeed,return true
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
