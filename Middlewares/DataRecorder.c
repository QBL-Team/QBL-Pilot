/*!
 * \file DataRecorder.h
 * \author no1wudi
 */

#include "DataRecorder.h"
#include "fatfs.h"
#include "sdio.h"
#include <stdio.h>
#include <string.h>
#include "HMC5883.h"

static FATFS data_recorder_disk;
static FIL data_file;
static int index_id = 0;

bool DataRecorder_Init()
{
    f_mount(&data_recorder_disk, "Data Recorder", 1);
    if (FR_OK == f_open(&data_file, "Mag.csv", FA_CREATE_ALWAYS | FA_WRITE)) {
        f_puts("id,point_X,point_Y,point_Z\n",&data_file);
        return true;
    }
    else {
        return false;
    }
}

void DataRecorder_Record(void)
{
    char buffer[50];
    unsigned int i = 0;
    float axis[3];
    HMC5883_Update(axis);
    sprintf(buffer,"%d,%d,%d,%d\n", index_id++, (volatile int)(440.0f * axis[0]),(volatile int)(440.0f *  axis[1]), (volatile int)(440.0f * axis[2]));
    f_write(&data_file,buffer,strlen(buffer),&i);
}

void DataRecorder_End(void)
{
    f_close(&data_file);
}
