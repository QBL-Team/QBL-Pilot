/*!
 *\file RingBuffer.h
 * \author no1wudi
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdlib.h>

/*!
 * \brief 环形缓冲实例的数据定义
 */

typedef struct
{
    uint16_t write_index;        //!<写地址索引
    uint16_t read_index;         //!<读地址索引
    uint8_t * buffer;            //!<缓冲池
    uint16_t buffer_capacity;    //!<容量大小，单位为字节
    uint16_t used_size;          //!<已经使用的缓冲区的大小
}RingBuffer_TypeDef;

/*!
 * @defgroup ring_buffer Ring Buffer
 * @{
 */


extern RingBuffer_TypeDef * RingBuffer_Init(uint16_t capacity);
extern void RingBuffer_Write(RingBuffer_TypeDef * ring_buffer,uint8_t data);
extern uint8_t RingBuffer_Read(RingBuffer_TypeDef * ring_buffer);
extern uint16_t RingBuffer_Used(RingBuffer_TypeDef * ring_buffer);
extern void RingBuffer_Clear(RingBuffer_TypeDef * ring_buffer);

/*!
 * @}
 */


#endif // RING_BUFFER_H
