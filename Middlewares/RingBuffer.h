/*!
 *\file RingBuffer.h
 * \author no1wudi
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdlib.h>

/*!
 * \brief Ring buffer instance type defination
 */

typedef struct
{
    uint16_t write_index;        //!<Write postion index
    uint16_t read_index;         //!<Read postion index
    uint8_t * buffer;            //!<Ring buffer pool
    uint16_t buffer_capacity;    //!<The capacity of the ring buffer
    uint16_t used_size;          //!<The buffered data's counts
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
