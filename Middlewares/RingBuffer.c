/*!
 *\file RingBuffer.c
 * \author no1wudi
 */


#include "RingBuffer.h"

/*!
 *@ingroup ring_buffer
 * \brief RING_BUFFER_ENTER_CRITICAL Macro to enter critical mode
 */

#define RING_BUFFER_ENTER_CRITICAL()    __disable_irq()

/*!
 *@ingroup ring_buffer
 * \brief RING_BUFFER_EXIT_CRITICAL Macro to exit critical mode
 */

#define RING_BUFFER_EXIT_CRITICAL()     __enable_irq()

/*!
 * @ingroup ring_buffer
 * \brief RingBuffer_Init  Construct a new ring buffer
 * \param capacity  The buffer size allocated for the buffer pool
 * \return  Pointer to ring buffer instance
 */

RingBuffer_TypeDef * RingBuffer_Init(uint16_t capacity)
{
    RingBuffer_TypeDef * temp = NULL;
    
    temp = malloc(sizeof(RingBuffer_TypeDef));

    if(NULL != temp)
    {
        temp->buffer = malloc(capacity);

        if(NULL == temp->buffer)
        {
            free(temp);
            temp = NULL;
        }
        else
        {
            temp->write_index = 0;
            temp->read_index = 0;
            temp->used_size = 0;
            temp->buffer_capacity = capacity;
        }
    }

    return temp;
}

/*!
 * @ingroup ring_buffer
 * \brief RingBuffer_Write Write data to buffer
 * \param ring_buffer   Ring buffer instance
 * \param data  Data to be wrote
 */

void RingBuffer_Write(RingBuffer_TypeDef * ring_buffer,uint8_t data){
    RING_BUFFER_ENTER_CRITICAL();
    if(ring_buffer->used_size == ring_buffer->buffer_capacity){
        RING_BUFFER_EXIT_CRITICAL();
        return;
    }
    ring_buffer->used_size++;
    if(ring_buffer->write_index == ring_buffer->buffer_capacity - 1){
        ring_buffer->write_index = 0;
    }
    ring_buffer->buffer[ring_buffer->write_index++] = data;
    RING_BUFFER_EXIT_CRITICAL();
}

/*!
 * @ingroup ring_buffer
 * \brief RingBuffer_Read  Read a data from buffer
 * \param ring_buffer   Buffer instance
 * \return
 */

uint8_t RingBuffer_Read(RingBuffer_TypeDef * ring_buffer){
    uint8_t tmp;
    RING_BUFFER_ENTER_CRITICAL();
    if(ring_buffer->used_size == 0){
        RING_BUFFER_EXIT_CRITICAL();
        return 0;
    }
    if(ring_buffer->read_index == ring_buffer->buffer_capacity - 1) {
        ring_buffer->read_index = 0;
    }
    ring_buffer->used_size--;
    tmp = ring_buffer->buffer[ring_buffer->read_index++];
    RING_BUFFER_EXIT_CRITICAL();
    return tmp;
}

/*!
 * @ingroup ring_buffer
 * \brief RingBuffer_Used  Get the data numbers in buffer
 * \param ring_buffer   Buffer instance
 * \return
 */

uint16_t RingBuffer_Used(RingBuffer_TypeDef * ring_buffer){
    return ring_buffer->used_size;
}

/*!
 * @ingroup ring_buffer
 * \brief RingBuffer_Clear Clear buffer
 * \param ring_buffer   Ring buffer instance
 */

void RingBuffer_Clear(RingBuffer_TypeDef * ring_buffer) {
    RING_BUFFER_ENTER_CRITICAL();

    ring_buffer->read_index = 0;
    ring_buffer->write_index = 0;
    ring_buffer->used_size = 0;


    RING_BUFFER_EXIT_CRITICAL();

}
