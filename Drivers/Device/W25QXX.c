/*!
 * \file W25QXX.c
 * \author no1wudi
 */

#include "W25QXX.h"

/*!
  *
  * @ingroup w25qxx_driver W25QXX 驱动
  * @{
  */

/*!
 *
 * @defgroup w25qxx_driver_private W25QXX 驱动内部数据
 * @{
 */

#define W25QXX_CMD_WRITE_ENABLE     0x06    ///< 写使能指令
#define W25QXX_CMD_WRITE_DISABLE    0x04    ///< 写失能指令
#define W25QXX_CMD_READ_STATUS_1    0x05    ///< 读取状态寄存器1
#define W25QXX_CMD_CHIP_EARSE       0xC7    ///< 擦出整个芯片
#define W25QXX_CMD_POWER_DOWN       0xB9    ///< 掉电指令
#define W25QXX_CMD_PROGRAM          0x02    ///< 写数据指令，每次最大写入256字节
#define W25QXX_CMD_SECTOR_ERASE     0x20    ///< 扇区擦除指令
#define W25QXX_CMD_BLOCK_ERASE      0xD8    ///< 块区擦除指令
#define W25QXX_CMD_READ             0x03    ///< 读数据指令
#define W25QXX_CMD_READ_ID          0x9F    ///< 读ID指令

#define W25QXX_BUSY_MASK            0x01    ///< 忙碌位掩码
#define W25QXX_WRITE_ENABLE_MASK    0x02    ///< 写使能掩码

#define W25QXX_CHIP_WINBOND_ID      0xEF    ///< 华邦芯片的ID

#define W25QXX_CS_ON()        HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port,W25QXX_CS_Pin,GPIO_PIN_RESET)  ///< 片选使能
#define W25QXX_CS_OFF()       HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port,W25QXX_CS_Pin,GPIO_PIN_SET)     ///< 片选失能

static SPI_HandleTypeDef * spi_handle = NULL;

/*!
 * \brief W25Q_GetStatus  读取状态寄存器1
 * \return 返回当前状态寄存器的值
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
 * \brief W25Q_SetWriteEnable   使能芯片的写入功能
 * \return  如果使能成功，返回true
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
 * \brief W25Q_Init 初始化W25Qx系列的闪存芯片
 * \param handle    挂载闪存芯片的SPI总线的句柄
 * \return 如果初始化成功，返回true
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
 * \brief W25Q_CheckForIdle 检查闪存芯片是否处于空闲状态，只有空闲状态可以进行操作
 * \return 如果芯片处于空闲状态，返回true
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
 * \brief W25Q_EraseChp 擦除整个芯片
 * \return 如果指令发送成功，返回true
 */

bool W25Q_EraseChip(void)
{
    uint8_t tmp = W25QXX_CMD_CHIP_EARSE;
    
    W25QXX_CS_ON();
    HAL_SPI_Transmit(spi_handle, &tmp, 1, 1);
    W25QXX_CS_OFF();
    
    return true;
}

/*!
 * \brief W25Q_EraseSector 擦除制定的扇区
 * \param addr  要擦除的扇区的地址
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
 * \brief W25Q_Write 写入数据到闪存芯片中
 * \param addr 写入数据的起始地址
 * \param buffer 写入数据的缓存
 * \param length 要写入的数据的长度，最大256字节
 * \return 如果写入成功，返回true
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
 * \brief W25Q_Read 从闪存中读取数据
 * \param addr 将要读取数据的起始地址
 * \param buffer 接收数据的缓冲
 * \param length 要读取的数据的长度
 * \return 如果读取成功，返回true
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
