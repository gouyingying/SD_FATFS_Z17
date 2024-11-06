//
// Created by ZONE7 on 2024/11/6.
//

#include "file_opera.hpp"
#include <stdarg.h> // 包含可变参数相关的头文件
#include "usart.h"

void fatTest_GetDiskInfo() {
    FATFS *fs;
    DWORD free_clust;
    FRESULT res = f_getfree("0:", &free_clust, &fs);
    if (res != FR_OK) {
        return;
    }

    DWORD total_sector = (fs->n_fatent - 2) * fs->csize;
    DWORD free_sector = free_clust * fs->csize;

#if _MAX_SS == _MIN_SS
    DWORD free_space = free_sector >> 11;
    DWORD total_space = total_sector >> 11;
#else
    DWORD free_space = (free_sector * fs->ssize) >> 10;
    DWORD total_space = (total_sector * fs->ssize) >> 10;
#endif

    char temp_str[40];
    sprintf(temp_str, "FAT type = %d", (int)fs->fs_type);
    // lcd_show_str(10, 10 + 9 * 15, 12, temp_str, RED);
    // lcd_show_str(10, 10 + 10 * 15, 12, "[1=FAT12,2=FAT16,3=FAT32,4=EXFAT]", BLACK);

#if _MAX_SS == _MIN_SS
    sprintf(temp_str, "Sector size(bytes) = %d", _MIN_SS);
#else
    sprintf(temp_str, "Sector size(bytes) = %d", fs->ssize);
#endif
    // lcd_show_str(10, 10 + 11 * 15, 12, temp_str, RED);

    sprintf(temp_str, "Cluster size(sectors) = %d", fs->csize);
    // lcd_show_str(10, 10 + 12 * 15, 12, temp_str, RED);

    sprintf(temp_str, "Total cluster count = %d", fs->n_fatent - 2);
    // lcd_show_str(10, 10 + 13 * 15, 12, temp_str, RED);

    sprintf(temp_str, "Total sector count = %d", total_sector);
    // lcd_show_str(10, 10 + 14 * 15, 12, temp_str, RED);

#if _MAX_SS == _MIN_SS
    sprintf(temp_str, "Total space(MB) = %d", total_space);
#else
    sprintf(temp_str, "Total space(KB) = %d", total_space);
#endif
    // lcd_show_str(10, 10 + 15 * 15, 12, temp_str, RED);

    sprintf(temp_str, "Free cluster count = %d", free_clust);
    // lcd_show_str(10, 10 + 16 * 15, 12, temp_str, RED);

    sprintf(temp_str, "Free sector count = %d", free_sector);
    // lcd_show_str(10, 10 + 17 * 15, 12, temp_str, RED);

#if _MAX_SS == _MIN_SS
    sprintf(temp_str, "Free space(MB) = %d", free_space);
#else
    sprintf(temp_str, "Free space(KB) = %d", free_space);
#endif
    // lcd_show_str(10, 10 + 18 * 15, 12, temp_str, RED);

    // lcd_show_str(10, 10 + 19 * 15, 12, "Get FAT disk info OK", BLACK);
}

void fatTest_ScanDir(const TCHAR* PathName) {
    DIR dir;
    FILINFO dir_info;
    uint8_t show_line = 9;
    FRESULT res = f_opendir(&dir, PathName);
    if (res != FR_OK) {
        f_closedir(&dir);
        return;
    }

    uint8_t temp_str[40];
    sprintf((char*)temp_str, "All entries in dir %s", PathName);
    // lcd_show_str(10, 10 + 8 * 15, 12, temp_str, RED);
    while (1) {
        res = f_readdir(&dir, &dir_info);
        if (res != FR_OK || dir_info.fname[0] == 0) {
            break;
        }
        if (dir_info.fattrib & AM_DIR) {
            sprintf((char*)temp_str, "DIR   %s", dir_info.fname);
            // lcd_show_str(10, 10 + show_line * 15, 12, temp_str, RED);
            show_line++;
        } else {
            sprintf((char*)temp_str, "FILE  %s", dir_info.fname);
            // lcd_show_str(10, 10 + show_line * 15, 12, temp_str, RED);
            show_line++;
        }
    }
    // lcd_show_str(10, 10 + show_line * 15, 12, "Scan dir OK", RED);
    f_closedir(&dir);
}

void fatTest_Write_Init(TCHAR* filename) {
    FIL file;
    FRESULT res = f_open(&file, filename,  FA_WRITE);
    if (res == FR_OK) {
        usart_printf("File opened Ok\n");
    }
    else {

        }
    FRESULT resl = f_lseek(&file, f_size(&file));
    if (res == FR_OK && resl == FR_OK) {
        f_puts("\n", &file);
        f_puts("\n", &file);
        f_puts("\n", &file);
        f_puts("\n", &file);
        f_puts("\n", &file);
        f_puts("NEW\n", &file);
    } else {

    }
    f_close(&file);
}




void fatTest_WriteTXTFile(TCHAR* filename, int num, ...) {
    FIL file;
    FRESULT res = f_open(&file, filename, FA_WRITE | FA_OPEN_APPEND); // 打开文件并追加
    if (res != FR_OK) {
        // 文件打开失败时的处理
        return;
    }

    // 定义可变参数列表
    va_list args;
    va_start(args, num); // 初始化 args 为第一个可变参数 num 的地址

    // 写入数据
    for (int i = 0; i < num; i++) {
        uint32_t value = va_arg(args, uint32_t); // 按照顺序获取参数
        f_printf(&file, "%d  ",  value); // 写入到文件
    }
    f_printf(&file, "\n"); // 写入到文件
    va_end(args); // 清理可变参数列表
    f_close(&file); // 关闭并保存文件
}


void fatTest_WriteBinFile(TCHAR* filename, uint32_t pointCount, uint32_t sampFreq) {
    FIL file;
    FRESULT res = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
    if (res == FR_OK) {
        f_puts("ADC1-IN5\n", &file);

        UINT bw = 0;
        f_write(&file, &pointCount, sizeof (uint32_t), &bw);
        f_write(&file, &sampFreq, sizeof (uint32_t), &bw);

        uint32_t value = 1000;
        for (uint16_t i = 0; i < pointCount; i++, value++) {
            f_write(&file, &value, sizeof (uint32_t), &bw);
        }
    }
    f_close(&file);
}

void fatTest_ReadTXTFile(TCHAR* filename) {
    uint8_t temp_str[40];
    sprintf((char*)temp_str, "Reading TXT file: %s", filename);
    // lcd_show_str(10, 10 + 8 * 15, 12, temp_str, RED);

    FIL file;
    FRESULT res = f_open(&file, filename, FA_READ);
    uint8_t show_line = 9;
    if (res == FR_OK) {
        uint8_t show_str[40];
        while (!f_eof(&file)) {
            f_gets((char*)show_str, 40, &file);
            // lcd_show_str(10, 10 + show_line * 15, 12, show_str, RED);
            show_line++;
        }
    } else if (res == FR_NO_FILE) {
        sprintf((char*)temp_str, "%s does not exist", filename);
        // lcd_show_str(10, 10 + 8 * 15, 12, temp_str, RED);
    } else {
        // lcd_show_str(10, 10 + 8 * 15, 12, "f_open() error", RED);
    }
    f_close(&file);
}

void fatTest_ReadBinFile(TCHAR* filename) {
    uint8_t temp_str[40];
    sprintf((char*)temp_str, "Reading BIN file: %s", filename);
    // lcd_show_str(10, 10 + 8 * 15, 12, temp_str, RED);
    FIL file;
    FRESULT res = f_open(&file, filename, FA_READ);
    if (res == FR_OK) {
        uint8_t show_str[40];
        f_gets((char*)show_str, 40, &file);

        UINT bw = 0;
        uint32_t pointCount;
        uint32_t sampFreq;
        f_read(&file, &pointCount, sizeof (uint32_t), &bw);
        f_read(&file, &sampFreq, sizeof (uint32_t), &bw);
        uint32_t value[pointCount];
        for (uint16_t i = 0; i < pointCount; ++i) {
            f_read(&file, &value[i], sizeof (uint32_t), &bw);
        }

        // lcd_show_str(10, 10 + 9 * 15, 12, show_str, RED);

        sprintf((char*)temp_str, "Point count: %d", pointCount);
        // lcd_show_str(10, 10 + 10 * 15, 12, temp_str, RED);

        sprintf((char*)temp_str, "Sampling frequency: %d", sampFreq);
        // lcd_show_str(10, 10 + 11 * 15, 12, temp_str, RED);

        sprintf((char*)temp_str, "value[5] = %d", value[5]);
        // lcd_show_str(10, 10 + 12 * 15, 12, temp_str, RED);

        sprintf((char*)temp_str, "value[16] = %d", value[16]);
        // lcd_show_str(10, 10 + 13 * 15, 12, temp_str, RED);
    } else if (res == FR_NO_FILE) {
        sprintf((char*)temp_str, "%s does not exist", filename);
        // lcd_show_str(10, 10 + 8 * 15, 12, temp_str, RED);
    } else {
        // lcd_show_str(10, 10 + 8 * 15, 12, "f_open() error", RED);
    }
    f_close(&file);
}

void fatTest_GetFileInfo(TCHAR* filename) {
    uint8_t temp_str[40];
    sprintf((char*)temp_str, "File info of: %s", filename);
    // lcd_show_str(10, 10 + 8 * 15, 12, temp_str, RED);

    FILINFO file_info;
    FRESULT res = f_stat(filename, &file_info);
    if (res == FR_OK) {
        sprintf((char*)temp_str, "File size(bytes)= %d", file_info.fsize);
        // lcd_show_str(10, 10 + 9 * 15, 12, temp_str, RED);

        sprintf((char*)temp_str, "File attribute = 0x%X", file_info.fattrib);
        // lcd_show_str(10, 10 + 10 * 15, 12, temp_str, RED);

        sprintf((char*)temp_str, "File name = %s", file_info.fname);
        // lcd_show_str(10, 10 + 11 * 15, 12, temp_str, RED);

        RTC_DateTypeDef sDate;
        RTC_TimeTypeDef sTime;

        sDate.Date = file_info.fdate & 0x001F;
        sDate.Month = (file_info.fdate & 0x01E0) >> 5;
        sDate.Year = 1980 + ((file_info.fdate & 0xFE00) >> 9) - 2000;

        sTime.Hours = (file_info.ftime & 0xF800) >> 11;
        sTime.Minutes = (file_info.ftime & 0x07E0) >> 5;
        sTime.Seconds = (file_info.ftime & 0x001F) << 1;

        sprintf((char*)temp_str, "File Date = 20%02d-%02d-%02d", sDate.Year, sDate.Month, sDate.Date);
        // lcd_show_str(10, 10 + 12 * 15, 12, temp_str, RED);

        sprintf((char*)temp_str, "File Time = %02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
        // lcd_show_str(10, 10 + 13 * 15, 12, temp_str, RED);
    } else if (res == FR_NO_FILE) {
        sprintf((char*)temp_str, "%s does not exist", filename);
        // lcd_show_str(10, 10 + 8 * 15, 12, temp_str, RED);
    } else {
        // lcd_show_str(10, 10 + 8 * 15, 12, "f_stat() error", RED);
    }
}

DWORD fat_GetFatTimeFromRTC() {
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    if (HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) == HAL_OK) {
        HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
        WORD date = ((2000 + sDate.Year - 1980) << 9) | (sDate.Month << 5) | sDate.Date;
        WORD time = (sTime.Hours << 11) | (sTime.Minutes << 5) | (sTime.Seconds >> 1);

        DWORD fatTime = ((DWORD)date << 16) | time;
        return fatTime;
    } else {
        return 0;
    }
}
