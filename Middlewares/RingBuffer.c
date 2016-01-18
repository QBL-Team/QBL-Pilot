/*!
 *\file RingBuffer.c
 * \author no1wudi
 */


#include "RingBuffer.h"

/*!
 *@ingroup ring_buffer
 * \brief RING_BUFFER_ENTER_CRITICAL 进入临界模式
 */

#define RING_BUFFER_ENTER_CRITICAL()    __disable_irq()

/*!
 *@ingroup ring_buffer
 * \brief RING_BUFFER_EXIT_CRITICAL 退出临界模式
 */

#define RING_BUFFER_EXIT_CRITICAL()     __enable_irq()

/*!
 * @ingroup ring_buffer
 * \brief RingBuffer_Init 构造一个新的环形缓冲实例
 * \param capacity  要给缓冲池分配的容量大小，单位为字节
 * \return  环形缓冲实例的指针
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
 * \brief RingBuffer_Write 向环形缓冲中写入数据
 * \param ring_buffer   环形缓冲的指针
 * \param data  将要写入的数据
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
 * \brief RingBuffer_Read  从环形缓冲中读取一个数据
 * \param ring_buffer   环形缓冲的指针
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
 * \brief RingBuffer_Used  获取环形缓冲已经使用的容量
 * \param ring_buffer   环形缓冲的实例指针
 * \return
 */

uint16_t RingBuffer_Used(RingBuffer_TypeDef * ring_buffer){
    return ring_buffer->used_size;
}

/*!
 * @ingroup ring_buffer
 * \brief RingBuffer_Clear 清除缓冲区
 * \param ring_buffer   要清除的缓冲的实例
 */

void RingBuffer_Clear(RingBuffer_TypeDef * ring_buffer) {
    RING_BUFFER_ENTER_CRITICAL();

    ring_buffer->read_index = 0;
    ring_buffer->write_index = 0;
    ring_buffer->used_size = 0;


    RING_BUFFER_EXIT_CRITICAL();

}
