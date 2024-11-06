//
// Created by ZONE7 on 2024/11/6.
//

#include "sd_io.hpp"

#include "file_opera.hpp"

void SDCard_ShowInfo() {
    HAL_SD_CardInfoTypeDef cardInfo;
    HAL_StatusTypeDef res = HAL_SD_GetCardInfo(&hsd, &cardInfo);
    if (res != HAL_OK) {
        return;
    }

    uint8_t temp_str[40];
    sprintf((char*)temp_str, "Card Type=%d", cardInfo.CardType);

    sprintf((char*)temp_str, "Card Version=%d", cardInfo.CardVersion);

    sprintf((char*)temp_str, "Card Class=%d", cardInfo.Class);

    sprintf((char*)temp_str, "Relative Card Address=%d", cardInfo.RelCardAdd);

    sprintf((char*)temp_str, "Block Count=%d", cardInfo.BlockNbr);

    sprintf((char*)temp_str, "Block Size(Bytes)=%d", cardInfo.BlockSize);

    sprintf((char*)temp_str, "Logic Block Count=%d", cardInfo.LogBlockNbr);

    sprintf((char*)temp_str, "Logic Block Size(Bytes)=%d", cardInfo.LogBlockSize);

    sprintf((char*)temp_str, "SD Card Capacity(MB)=%d", cardInfo.BlockNbr / 1024 * cardInfo.BlockSize / 1024);
}

void SDCard_EraseBlocks() {
    uint32_t BlockStartAdd = 0;
    uint32_t BlockEndAdd = 10;
    HAL_StatusTypeDef res = HAL_SD_Erase(&hsd, BlockStartAdd, BlockEndAdd);
    if (res == HAL_OK) {
        // Erase success handling code (removed LCD output)
    } else {
        // Erase error handling code (removed LCD output)
    }
    HAL_SD_CardStateTypeDef cardState = HAL_SD_GetCardState(&hsd);
    uint8_t temp_str[40];
    sprintf((char*)temp_str, "GetCardState() = %d", cardState);
    // Output removed
    while (cardState != HAL_SD_CARD_TRANSFER) {
        HAL_Delay(1);
        cardState = HAL_SD_GetCardState(&hsd);
    }
    sprintf((char*)temp_str, "GetCardState() = %d", cardState);
    // Output removed
}

void SDCard_TestWrite() {
    uint8_t pData[BLOCKSIZE] = "Hello, welcome to UPC\0";
    uint32_t BlockAdd = 5;
    uint32_t NumberOfBlocks = 1;
    uint32_t Timeout = 1000;
    HAL_StatusTypeDef res = HAL_SD_WriteBlocks(&hsd, pData, BlockAdd, NumberOfBlocks, Timeout);
    if (res == HAL_OK) {
        // Write success handling code (removed LCD output)
    } else {
        // Write error handling code (removed LCD output)
    }
}

void SDCard_TestRead() {
    uint8_t pData[BLOCKSIZE];
    uint32_t BlockAdd = 5;
    uint32_t NumberOfBlocks = 1;
    uint32_t Timeout = 1000;
    HAL_StatusTypeDef res = HAL_SD_ReadBlocks(&hsd, pData, BlockAdd, NumberOfBlocks, Timeout);
    if (res == HAL_OK) {
        // Read success handling code (removed LCD output)
    } else {
        // Read error handling code (removed LCD output)
    }
}

