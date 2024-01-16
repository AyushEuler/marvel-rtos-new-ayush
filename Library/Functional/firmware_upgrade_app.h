#ifndef __FIRMWARE_UPGRADE_APP_H__
#define __FIRMWARE_UPGRADE_APP_H__

#include <stdint.h>
#include "stdbool.h"
#include <string.h>
#include <stdlib.h>

// #include "bootloaderFunctions.h"
// #include "threadxMain.h"
#define BIT0  (1 << 0)
#define BIT1  (1 << 1)
#define BIT2  (1 << 2)
#define BIT3  (1 << 3)
#define BIT4  (1 << 4)
#define BIT5  (1 << 5)
#define BIT6  (1 << 6)
#define BIT7  (1 << 7)
#define BIT8  (1 << 8)
#define BIT9  (1 << 9)
#define BIT10 (1 << 10)
#define BIT11 (1 << 11)
#define BIT12 (1 << 12)
#define BIT13 (1 << 13)
#define BIT14 (1 << 14)
#define BIT15 (1 << 15)
#define BIT16 (1 << 16)
#define BIT17 (1 << 17)
#define BIT18 (1 << 18)
#define BIT19 (1 << 19)
#define BIT20 (1 << 20)
#define BIT21 (1 << 21)
#define BIT22 (1 << 22)
#define BIT23 (1 << 23)
#define BIT24 (1 << 24)
#define BIT25 (1 << 25)
#define BIT26 (1 << 26)
#define BIT27 (1 << 27)
#define BIT28 (1 << 28)
#define BIT29 (1 << 29)
#define BIT30 (1 << 30)
#define BIT31 (1 << 31)


#define STARK_ID    0
#define MARK_ID     1 
#define MARVEL_ID   2
#define LAPTOP_ID   9
#define MAX_RETRY           10


#define tx_flash_write_progress 0x6FB
#define FIRMWARE_UPGRADE_COMM   0x6FA
#define rx_CAN_TP_Stark         0x1FE
#define FUPFG_REQUEST           1
#define FUPG_STATUS             0  

#define RECEIVE_TIMEOUT         0x750

#define DATA_PACKET_SIZE        512
#define NUM_TIMEOUTS            15


#define STARK_CAN_TP_RX     BIT0
#define START_OTA 			BIT1
#define COMPLETE_OTA        BIT2
#define PUASE_OTA           BIT3
#define RESUME_OTA          BIT4




typedef enum {
    SUCCESS_MESS = (uint8_t)1,
    CAN_TP_TIMEOUT = (uint8_t)2,
    FILE_CREATE_ERROR = (uint8_t)3,
    CRC_ERROR = (uint8_t)4,
    FILE_WRITE_ERROR = (uint8_t)5,
    CHECKSUM_ERROR = (uint8_t)6
} firmware_upgrade_error_codes_e;

typedef enum {
    DOWNLOAD_BIN = 1,
    RECEIVE_BIN = 2,
    DOWNLOAD_CONF = 3,
    RECEIVE_CONF = 4,
    PERFORM_UPGRADE = 5,
    PAUSE_UPGRADE = 6,
    RESUME_UPGRADE = 7,
} request_codes;


typedef enum {
    UPGRADE_INIT = (uint8_t)0,
    UPGRADE_RECEIVE_FILE_SIZE = (uint8_t)1, 
    UPGRADE_RECEIVE_DATA = (uint8_t)2,
    UPGRADE_PAUSE = (uint8_t)3,
    UPGRADE_RESUME = (uint8_t)4,
    UPGRADE_COMPLETE =  (uint8_t)5,
    UPGRADE_FAILED = (uint8_t)6,
} firmware_upgrade_states;

typedef enum {
    COTA_MESSAGE = (uint8_t) 1,
    FOTA_MESSAGE     = (uint8_t) 0,
} FILE_TYPES;

typedef enum {
    COTA = (uint8_t) 1,
    FOTA     = (uint8_t) 0,
} upgrade_type;



typedef struct __attribute__((packed)){
    uint8_t device_id;
    uint32_t offset;
} flash_write_progress_struct;




bool verifyStringFromServer(char *rest, uint64_t * data, uint32_t len);
int update_config(uint64_t *configData);
void handle_upgrade_complete();
void handle_upgrade_pause();
void handle_rceive_data();
void handle_upgrade_init();
uint8_t dataParserCOTA(uint64_t *flashData, char * server_cmd, char * delimiter);
void upgrade_firmware();
extern upgrade_type uptype;

char *strtok_r(char * __restrict__ _Str, const char * __restrict__ _Delim, char ** __restrict__ __last);

#endif