/*!
 * \file DataRecorder.h
 * \author no1wudi
 */

#ifndef __DATARECORDER_H
#define __DATARECORDER_H

#include <stdbool.h>


/*!
 * @defgroup datarecorder Data Recorder
 * @{
 */

/*!
 * \brief DataRecorder_Init 初始化数据记录器
 * \return 初始化成功并成功创建文件，返回true，否则请检查SD卡连接及其文件系统是否为FAT32
 */

extern bool DataRecorder_Init(void);

extern void DataRecorder_Record(void);

extern void DataRecorder_End(void);

/*!
 * @}
 */
#endif
