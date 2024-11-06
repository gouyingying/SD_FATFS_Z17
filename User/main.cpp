//
// Created by ZONE7 on 2024/11/6.
//
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

#include "sd_io.hpp"
#include "file_opera.hpp"
#include "sdio.h"
#include <cstring>


void BSP_Init(void);



int main()
{
    BSP_Init();
    User_Init();


    while(1)
    {


        HAL_Delay(50);
    }
}


void BSP_Init(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_DMA_Init();
    MX_RTC_Init();
    MX_USART1_UART_Init();
    MX_SDIO_SD_Init_Fix();
    MX_FATFS_Init();

}