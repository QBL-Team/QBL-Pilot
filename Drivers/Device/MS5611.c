/*!
 * \file MS5611.c
 * \author no1wudi
 */


/*!
  *
  * @defgroup ms5611_driver MS5611 Driver
  * @{
  */


#include "MS5611.h"

/*!
 * @defgroup ms5611_driver_internal MS5611 Driver Private
 * @{
 */

#define MS5611_CS_On()   HAL_GPIO_WritePin(MS5611_CS_GPIO_Port,MS5611_CS_Pin,GPIO_PIN_RESET)    ///< Chip select enable
#define MS5611_CS_Off()  HAL_GPIO_WritePin(MS5611_CS_GPIO_Port,MS5611_CS_Pin,GPIO_PIN_SET)      ///< Chip select disable
#define MS5611_CONVERSION_TIME  10                       ///< Chip conversion time , ms



static const uint8_t MS5611_CMD_RST                    = 0x1E; ///< Command code for reset device
static const uint8_t MS5611_CMD_CONVERT_D1             = 0x48; ///< Command code for convert pressure with OSR 4096
static const uint8_t MS5611_CMD_CONVERT_D2             = 0x58; ///< Command code for convert temperature with OSR 4096
static const uint8_t MS5611_CMD_READ_ADC       = 0x00; ///< Command code for read adc result
static const uint8_t MS5611_CMD_READ_C1        = 0xA2; ///< Command code for read PROM C1
static const uint8_t MS5611_CMD_READ_C2        = 0xA4; ///< Command code for read PROM C2
static const uint8_t MS5611_CMD_READ_C3        = 0xA6; ///< Command code for read PROM C3
static const uint8_t MS5611_CMD_READ_C4        = 0xA8; ///< Command code for read PROM C4
static const uint8_t MS5611_CMD_READ_C5        = 0xAA; ///< Command code for read PROM C5
static const uint8_t MS5611_CMD_READ_C6        = 0xAC; ///< Command code for read PROM C6


static uint32_t ms_ticks = 0;   ///<Time stamp for reading operation

static int64_t c1 = 0;      ///<PROM code C1,Pressure sensitivity | SENS T1
static int64_t c2 = 0;      ///<PROM code C2,Pressure offset | OFF T1
static int64_t c3 = 0;      ///<PROM code C3,Temperature coefficient of pressure sensitivity | TCS
static int64_t c4 = 0;      ///<PROM code C4,Temperature coefficient of pressure offset | TCO
static int64_t c5 = 0;      ///<PROM code C5,Reference temperature | T REF
static int64_t c6 = 0;      ///<PROM code C6,Temperature coefficient of the temperature | TEMPSENS

static int64_t d1 = 0;      ///< Digital pressure value
static int64_t d2 = 0;      ///< Digital temperature value

static int64_t dt = 0;      ///< Difference between actual and reference temperature
static int64_t temp = 0;    ///< Actual temperature (-40…85°C with 0.01°C resolution)

static int64_t off = 0;     ///< Offset at actual temperature
static int64_t sens = 0;    ///< Sensitivity at actual temperature
static int64_t p = 0;       ///< Temperature compensated pressure (10…1200mbar with 0.01mbar resolution)

static int64_t t2 = 0;
static int64_t off2 = 0;
static int64_t sens2 = 0;


static SPI_HandleTypeDef * spi_handle = NULL; ///< Handle for SPI bus

/*!
 *\enum MS5611_STATE
 * State flag for reading process
 */
typedef enum
{
    MS5611_STATE_IDLE = 0, ///< After powered,default in this state
    MS5611_STATE_WAIT_FOR_D1,///<Wait for pressure convertion
    MS5611_STATE_WAIT_FOR_D2,///<Wait for temperatue convertion
} MS5611_STATE;

static MS5611_STATE ms_state;       ///< State variable for reading process

/*!
 * \brief MS5611_ReadPROM   Read a PROM parameter from chip
 * \param MS5611_CMD        Command to determin whick papameter to be read
 * \return Result of reading.
 */

static uint16_t MS5611_ReadPROM(const uint8_t * MS5611_CMD)
{
    uint8_t tmp[2];

    MS5611_CS_On();
    HAL_SPI_Transmit(spi_handle, (uint8_t *) MS5611_CMD, 1, 1);
    HAL_SPI_Receive(spi_handle, tmp, 2, 1);
    MS5611_CS_Off();
    return (uint16_t)(tmp[0] << 8 | tmp[1]);
}

/*!
 * \brief MS5611_SendCMD Send a command to chip
 * \param MS5611_CMD    Which command to be sent
 */
static void MS5611_SendCMD(const uint8_t * MS5611_CMD)
{
    MS5611_CS_On();
    HAL_SPI_Transmit(spi_handle, (void *)MS5611_CMD, 1, 1);
    MS5611_CS_Off();
}
/*!
 * \brief MS5611_Compute    Calculate the real pressure and temperature.
 */
static void MS5611_Compute(void)
{
    //refered to datasheet
    dt = d2 - c5 * (1 << 8);
    temp = 2000 + dt * c6 / (1 << 23);

    off = c2 * (1 << 16) + (c4 * dt) / (1 << 7);
    sens = c1 * ( 1 << 15) + (c3 * dt) / (1 << 8);
    p = (d1 * sens / (1 << 21) - off) / (1 << 15);

    if(temp < 2000)
    {
        t2 = dt * dt / ((uint64_t)1 << 31);
        off2 = 5 * (temp - 2000) * (temp - 2000) / 2;
        sens2 = 5 * (temp - 2000) * (temp - 2000) / 4;

        if(temp < -1500)
        {
            off2 = off2 + 7 * (temp + 1500) * (temp + 1500);
            sens2 = sens2 + 11 * (temp + 1500) * (temp + 1500) / 2;
        }
    }
    else
    {
        t2 = 0;
        sens2 = 0;
        off2 = 0;
    }

    temp = temp - t2;
    off = off - off2;
    sens = sens - sens2;
}

/*!
 * @}
 */

/*!
 * \brief MS5611_Init Init the sensor,and read PROM values
 * \param handle    Handle of the SPI bus.
 * \return  If init succeed ,return true
 */

bool MS5611_Init(SPI_HandleTypeDef * handle)
{
    spi_handle = handle;

    //reset the state machine
    ms_state = MS5611_STATE_IDLE;

    //Reset the chip

    MS5611_SendCMD(&MS5611_CMD_RST);

    //Wait for chip reset
    HAL_Delay(10);


    //Read PROM

    c1 = MS5611_ReadPROM(&MS5611_CMD_READ_C1); //Dummy read to activate sensor

    c1 = MS5611_ReadPROM(&MS5611_CMD_READ_C1);
    c2 = MS5611_ReadPROM(&MS5611_CMD_READ_C2);
    c3 = MS5611_ReadPROM(&MS5611_CMD_READ_C3);
    c4 = MS5611_ReadPROM(&MS5611_CMD_READ_C4);
    c5 = MS5611_ReadPROM(&MS5611_CMD_READ_C5);
    c6 = MS5611_ReadPROM(&MS5611_CMD_READ_C6);

    return true;
}

/*!
 * \brief MS5611_Update Try to get newest data from sensor,notice that the conversion need some time,only if this function return true,the data is valid.
 * \param Pressure  Pointer pointed to pressure
 * \param Temperature   Pointer pointed to temperature
 * \return  If read succeed,return true
 */

bool MS5611_Update(float * Pressure, float * Temperature)
{
    uint8_t tmp[3];
    switch(ms_state)
    {
    case MS5611_STATE_IDLE:
        MS5611_SendCMD(&MS5611_CMD_CONVERT_D1);
        ms_ticks = HAL_GetTick();
        ms_state = MS5611_STATE_WAIT_FOR_D1;
        return false;

    case MS5611_STATE_WAIT_FOR_D1:

        if(HAL_GetTick() - ms_ticks >= MS5611_CONVERSION_TIME)
        {
            //Read adc result
            MS5611_CS_On();
            HAL_SPI_Transmit(spi_handle, (void *)&MS5611_CMD_READ_ADC, 1, 1);
            HAL_SPI_Receive(spi_handle, tmp, 3, 1);
            MS5611_CS_Off();

            //Conver to normal order
            d1 = (uint32_t)((tmp[0] << 16) | (tmp[1] << 8) | tmp[2]);

            MS5611_SendCMD(&MS5611_CMD_CONVERT_D2);
            ms_ticks = HAL_GetTick();
            ms_state = MS5611_STATE_WAIT_FOR_D2;
        }

        return false;

    case MS5611_STATE_WAIT_FOR_D2:

        if(HAL_GetTick() - ms_ticks >= MS5611_CONVERSION_TIME)
        {
            //Read adc result
            MS5611_CS_On();
            HAL_SPI_Transmit(spi_handle, (void *)&MS5611_CMD_READ_ADC, 1, 1);
            HAL_SPI_Receive(spi_handle, tmp, 3, 1);
            MS5611_CS_Off();

            //Conver to normal order
            d2 = (uint32_t)((tmp[0] << 16) | (tmp[1] << 8) | tmp[2]);

            MS5611_SendCMD(&MS5611_CMD_CONVERT_D1);
            ms_ticks = HAL_GetTick();
            ms_state = MS5611_STATE_WAIT_FOR_D1;

            MS5611_Compute();

            *Pressure = p / 100.0f;
            *Temperature = temp / 100.0f;
            
            return true;
        }

        return false;

    default:
        return false;
    }
}

/*!
 * @}
 */
