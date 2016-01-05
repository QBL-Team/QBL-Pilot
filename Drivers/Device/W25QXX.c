/*!
 * \file W25QXX.c
 * \author no1wudi
 */

#include "W25QXX.h"

/*!
  *
  * @defgroup w25qxx_driver W25QXX Driver
  * @{
  */

/*!
 *
 * @defgroup w25qxx_driver_private W25QXX Driver Private
 * @{
 */

#define W25QXX_CMD_WRITE_ENABLE     0x06    ///< Command for write enable
#define W25QXX_CMD_WRITE_DISABLE    0x04    ///< Command for write disable
#define W25QXX_CMD_READ_STATUS_1    0x05    ///< Command for read status register 1
#define W25QXX_CMD_CHIP_EARSE       0xC7    ///< Command for full chip earse
#define W25QXX_CMD_POWER_DOWN       0xB9    ///< Command for power down
#define W25QXX_CMD_PROGRAM          0x02    ///< Command for page program,max 256 byte once
#define W25QXX_CMD_SECTOR_ERASE     0x20    ///< Command for sector erase
#define W25QXX_CMD_BLOCK_ERASE      0xD8    ///< Command for block erase
#define W25QXX_CMD_READ             0x03    ///< Command for read data
#define W25QXX_CMD_READ_ID          0x9F    ///< Command for read chip id

#define W25QXX_BUSY_MASK            0x01    ///< Mask for busy bit
#define W25QXX_WRITE_ENABLE_MASK    0x02    ///< Mask for write enbale bit

#define W25QXX_CHIP_WINBOND_ID      0xEF    ///< Windbod chip's id

#define W25QXX_CS_ON()        HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port,W25QXX_CS_Pin,GPIO_PIN_RESET)  ///< Chil select on
#define W25QXX_CS_OFF()       HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port,W25QXX_CS_Pin,GPIO_PIN_SET)     ///< Chip select off

static SPI_HandleTypeDef * spi_handle = NULL;   ///< Internal handle of spi bus

/*!
 * \brief W25Q_GetStatus  Read status register 1
 * \return Current status
 */
static uint8_t W25Q_GetStatus(void)
{
    uint8_t tmp[2];
    tmp[0] = W25QXX_CMD_READ_STATUS_1;
    W25QXX_CS_ON();
    HAL_SPI_TransmitReceive(spi_handle, tmp, tmp, 2, 1);
    W25QXX_CS_OFF();
    return tmp[1];
}

/*!
 * \brief W25Q_SetWriteEnable   Enable write function of flash
 * \return  If set succeed ,return true
 */

static bool W25Q_SetWriteEnable(void)
{
    uint8_t retry = 15;
    uint8_t tmp = W25QXX_CMD_WRITE_ENABLE;

    while(retry--)
    {
        if(W25Q_GetStatus() & W25QXX_WRITE_ENABLE_MASK)
        {
            return true;
        }
        else
        {
            W25QXX_CS_ON();

            HAL_SPI_Transmit(spi_handle, &tmp, 1, 1);

            W25QXX_CS_OFF();
        }
    }
    return false;
}



/*!
 * @}
 */
/*!
 * \brief W25Q_Init Init the W25Qxx flash chips
 * \param handle    Handle of spi bus
 * \return If init succeed,return true
 */
bool W25Q_Init(SPI_HandleTypeDef * handle)
{
    uint8_t tmp[4];

    spi_handle = handle;
    tmp[0] = W25QXX_CMD_READ_ID;

    W25QXX_CS_ON();
    HAL_SPI_TransmitReceive(spi_handle, tmp, tmp, 4, 1);
    W25QXX_CS_OFF();

    if(tmp[1] != W25QXX_CHIP_WINBOND_ID)
    {
        return false;
    }

    return true;
}

/*!
 * \brief W25Q_CheckForIdle Check the chip is idle status or not
 * \return If chip in idle status,return true
 */

bool W25Q_CheckForIdle(void)
{
    if(W25Q_GetStatus() & W25QXX_BUSY_MASK)
    {
        return false;
    }

    return true;
}

/*!
 * \brief W25Q_EraseChp Erase full chip
 * \return
 */

bool W25Q_EraseChp(void)
{
    uint8_t tmp = W25QXX_CMD_CHIP_EARSE;
    
    W25QXX_CS_ON();
    HAL_SPI_Transmit(spi_handle, &tmp, 1, 1);
    W25QXX_CS_OFF();
    
    return true;
}

/*!
 * \brief W25Q_EraseSector Erase specific sector by address
 * \param addr  Address of sector
 * \return
 */
bool W25Q_EraseSector(uint32_t addr)
{
    uint8_t tmp[4];
    tmp[0] = W25QXX_CMD_SECTOR_ERASE;
    tmp[1] = addr >> 16;
    tmp[2] = (addr >> 8) & 0xff;
    tmp[3] = addr & 0xff;

    W25QXX_CS_ON();
    HAL_SPI_Transmit(spi_handle, tmp, 4, 1);
    W25QXX_CS_OFF();

    return true;
}
/*!
 * \brief W25Q_Write Write data to flash
 * \param addr Address of start point in flash
 * \param buffer Data buffer point
 * \param length Data length,max 256 in byte
 * \return If write succeed,return true
 */
bool W25Q_Write(uint32_t addr, uint8_t * buffer, uint16_t length)
{
    uint8_t tmp[4];

    if(false == W25Q_SetWriteEnable())
    {
        return false;
    }

    tmp[0] = W25QXX_CMD_PROGRAM;
    tmp[1] = addr >> 16;
    tmp[2] = (addr >> 8) & 0xff;
    tmp[3] = addr & 0xff;

    W25QXX_CS_ON();
    HAL_SPI_Transmit(spi_handle, tmp, 4, 1);
    HAL_SPI_Transmit(spi_handle, buffer, length, 5);
    W25QXX_CS_OFF();
    return true;
}

/*!
 * \brief W25Q_Read Read data from flash
 * \param addr Address of position in flash
 * \param buffer Buffer to receive data
 * \param length Data length to read
 * \return If read succeed,return true
 */

bool W25Q_Read(uint32_t addr, uint8_t * buffer, uint16_t length)
{
    uint8_t tmp[4];
    tmp[0] = W25QXX_CMD_READ;
    tmp[1] = addr >> 16;
    tmp[2] = (addr >> 8) & 0xff;
    tmp[3] = addr & 0xff;

    W25QXX_CS_ON();
    HAL_SPI_Transmit(spi_handle, tmp, 4, 1);
    HAL_SPI_Receive(spi_handle, buffer, length, 5);
    W25QXX_CS_OFF();
    return true;
}

/*!
 * @}
 */
