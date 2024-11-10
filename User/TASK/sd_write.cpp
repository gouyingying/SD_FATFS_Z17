//
// Created by ZONE7 on 2024/11/6.
//

#include "sd_write.hpp"
#include "cmsis_os.h"
//基本只需要这三个头文件
#include "fatfs.h"
#include "file_opera.hpp"
#include "sdio.h"

#include "usart.h"

float imuyaw = 63.56;
float imupitch = 32.14;
float imuroll = 145.19;

void sd_write(void const * argument)
{
    TickType_t current;


    FRESULT res = f_mount(&SDFatFS, "0:", 1);//挂载SD卡
    if (res == FR_OK)
    {
        // usart_printf("SD card mount successful\r\n");
    }
    else {
        // usart_printf("SD card mount failed\r\n");
    }
    fatTest_Init("num.txt");

    for(;;)
    {
        current = xTaskGetTickCount();

        fatTest_WriteTXTFile(new_filename, 3,(uint32_t)(imuyaw * 100), (uint32_t)(imupitch * 100), (uint32_t)(imuroll * 100));

        vTaskDelayUntil(&current, 500 / portTICK_RATE_MS);
    }
    /* USER CODE END GimbalTask */
}

