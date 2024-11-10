//
// Created by ZONE7 on 2024/11/6.
//

#ifndef FILE_OPERA_HPP
#define FILE_OPERA_HPP

#include "ff.h"
#include <stdio.h>
#include "rtc.h"

void fatTest_GetDiskInfo();
void fatTest_ScanDir(const TCHAR* PathName);
void fatTest_Write_Init(TCHAR* filename);
void fatTest_WriteTXTFile(TCHAR* filename, int num, ...);
void fatTest_WriteBinFile(TCHAR* filename, uint32_t pointCount, uint32_t sampFreq);
void fatTest_ReadTXTFile(TCHAR* filename);
void fatTest_ReadBinFile(TCHAR* filename);
void fatTest_GetFileInfo(TCHAR* filename);
void fatTest_Init(TCHAR* filename);
DWORD fat_GetFatTimeFromRTC();

class file_opera {

};

extern char new_filename[20];

#endif //FILE_OPERA_HPP
