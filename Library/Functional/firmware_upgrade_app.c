#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "firmware_upgrade_app.h"
#include "main.h"
#include "can_tp_app.h"
#include "threadxMain.h"
#include "canControls.h"
#include "memoryMap.h"
#include "ota.h"
#include "flashControls.h"
#include "onBoardFlash.h"
#include "main.h"
#include "stdint.h"
#include "bmsConfiguration.h"
// #define APP1 
upgrade_type uptype = COTA;
firmware_upgrade_states upgrade_state = UPGRADE_INIT;
firmware_upgrade_states previous_upgrade_state = UPGRADE_INIT;
IsoTpShims firmware_up_recv_shim;
IsoTpReceiveHandle firmware_up_recv_handle;
IsoTpMessage firmware_up_recv_message;

uint8_t firmware_up_flag = 0;
uint8_t target_id = STARK_ID;

volatile flash_write_progress_struct write_progress;

uint8_t fail_mess = 0;
uint8_t success_mess = 1;
uint32_t app_address = 0;

uint16_t curr_frame = 0;
uint16_t total_frames = 0;

uint8_t response[5] = {MARVEL_ID, STARK_ID, FUPG_STATUS, 1, MARVEL_ID};

uint32_t crc32_for_byte(uint32_t r)
{
	for (int j = 0; j < 8; ++j) r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;
	return r ^ (uint32_t)0xFF000000L;
}


uint32_t crc32(const void *data, uint32_t n_bytes)
{
	uint32_t crc = 0;
	static uint32_t table[0x100];
	if (!*table)
		for (size_t i = 0; i < 0x100; ++i) table[i] = crc32_for_byte(i);
	for (size_t i = 0; i < n_bytes; ++i)
		crc = table[(uint8_t)crc ^ ((uint8_t *)data)[i]] ^ crc >> 8;

	return crc;
}

void handle_upgrade_init()
{

    if (uptype == FOTA)
    {
#ifdef APP1
        app_address =  (uint32_t)&__app2_start__;
        uint8_t res = clearApp2();
#endif
#ifdef APP2
        app_address =  (uint32_t)&__app1_start__;
        uint8_t res = clearApp1();
#endif
        if(res == 0)
        {
            response[3] = FILE_CREATE_ERROR;
            Transmit_on_CAN1(FIRMWARE_UPGRADE_COMM, S, response, 5);
            firmware_up_flag = 0;

            // FAILED
            upgrade_state = UPGRADE_FAILED;
            return;
        }
    }

    response[3] = SUCCESS_MESS;
    Transmit_on_CAN1(FIRMWARE_UPGRADE_COMM, S, response, 5);   // sendCAN vs Transmit_on_CAN1
    upgrade_state = UPGRADE_RECEIVE_DATA;
}

void handle_rceive_data()
{
    ULONG actual_flag;

    static uint8_t mess_buff[521] = {0};
    static uint16_t mess_len = 0;
    uint64_t double_words[32] = {0};
    static unsigned int recv_crc = 0;
    static unsigned int offset = 0;
    static unsigned int computed_crc = 0;
    uint8_t message_type = 3;

    // uint8_t timeout_count = 0;
    uint8_t retry = 0;


    while (1)
    {
        if(tx_event_flags_get(&system_event_flag, STARK_CAN_TP_RX | PUASE_OTA | COMPLETE_OTA, TX_OR, &actual_flag, RECEIVE_TIMEOUT) == TX_SUCCESS)
        {
            if (tx_event_flags_get(&system_event_flag, COMPLETE_OTA, TX_OR, &actual_flag, TX_NO_WAIT) == TX_SUCCESS){
                upgrade_state = UPGRADE_COMPLETE;
                                return;
            }

            if (tx_event_flags_get(&system_event_flag, PUASE_OTA, TX_OR_CLEAR, &actual_flag, TX_NO_WAIT) == TX_SUCCESS){
                
                tx_event_flags_get(&system_event_flag, STARK_CAN_TP_RX, TX_OR_CLEAR, &actual_flag, TX_NO_WAIT);
                previous_upgrade_state = upgrade_state;
                upgrade_state = UPGRADE_PAUSE;
                tx_event_flags_get(&system_event_flag, RESUME_OTA, TX_OR_CLEAR, &actual_flag, TX_NO_WAIT);
                return;
            }            

            tx_event_flags_get(&system_event_flag, STARK_CAN_TP_RX, TX_OR_CLEAR, &actual_flag, TX_NO_WAIT);

            memcpy(&message_type, firmware_up_recv_message.payload, 1);

            switch (message_type)
            {
            case COTA_MESSAGE:
                memcpy(&recv_crc, firmware_up_recv_message.payload + 1, 4); 
                update_config((uint64_t *)&defaultFlashConfigData);
                break;
            
            case FOTA_MESSAGE:
                memcpy(mess_buff, firmware_up_recv_message.payload+9, firmware_up_recv_message.size-9);
                memcpy(&recv_crc, firmware_up_recv_message.payload + 1, 4); 
                memcpy(&offset, firmware_up_recv_message.payload + 5, 4);

                mess_len = firmware_up_recv_message.size-9;
                computed_crc = crc32(mess_buff, mess_len);
                if(computed_crc != recv_crc)
                {
                    response[3] = CRC_ERROR;
                    Transmit_on_CAN1(FIRMWARE_UPGRADE_COMM, S, response, 5);
                    firmware_up_flag = 0;

                    // FAILED
                    upgrade_state = UPGRADE_FAILED;
                    return;
                }

                for(int word_c = 0 ; word_c < mess_len/8 ; word_c++){
                    uint64_t* double_word = (uint64_t*)&mess_buff[word_c*8];
                    double_words[word_c] = *double_word;
                }

                unsigned int address = app_address + offset;
                flashWriteOTA(address, double_words, mess_len/8);
                curr_frame++;
                write_progress.offset = offset;

                // Transmit_on_CAN1(tx_flash_write_progress, S, &write_progress, sizeof(write_progress));
                response[3] = SUCCESS_MESS;
                Transmit_on_CAN1(FIRMWARE_UPGRADE_COMM, S, response, 5);
                break;
            
            default:
                break;
            }
        }
        else
        {
            retry++;
            response[3] = CAN_TP_TIMEOUT;
            Transmit_on_CAN1(FIRMWARE_UPGRADE_COMM, S, response,5);
            firmware_up_flag = 0;
            if (retry > MAX_RETRY) {upgrade_state = UPGRADE_FAILED; return ;}  
        }
    }
}

void handle_upgrade_pause(){

    uint8_t debug[5] = {5};
    Transmit_on_CAN1(0x340, S, debug, 5);

    ULONG actual_flag;
    if (tx_event_flags_get(&system_event_flag, RESUME_OTA, TX_OR_CLEAR, &actual_flag, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
        // reset flags 
        tx_event_flags_get(&system_event_flag, PUASE_OTA, TX_OR_CLEAR, &actual_flag, TX_NO_WAIT);
        response[3] = RESUME_UPGRADE;
        Transmit_on_CAN1(FIRMWARE_UPGRADE_COMM, S, response, 5);
        upgrade_state = previous_upgrade_state;
        return;

    }
    
}

void handle_upgrade_complete()
{
    ULONG actual_flag;
    if(tx_event_flags_get(&system_event_flag, COMPLETE_OTA, TX_OR_CLEAR, &actual_flag, TX_WAIT_FOREVER) == TX_SUCCESS){
                switch_partition_and_reset();
    }

}


void upgrade_firmware()
{
    ULONG actual_flag;
    if(tx_event_flags_get(&system_event_flag, START_OTA, TX_OR_CLEAR, &actual_flag, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
        upgrade_state = UPGRADE_INIT;
        while (1)
        {
            switch (upgrade_state)
            {
            case UPGRADE_INIT:
                handle_upgrade_init();
                break;
            case UPGRADE_RECEIVE_DATA:
                handle_rceive_data();
                break;
            case UPGRADE_PAUSE:
                handle_upgrade_pause();
                break;
            case UPGRADE_COMPLETE:
                handle_upgrade_complete();
                break;
            case UPGRADE_FAILED:
                return;
                break;
            default:
                return;
                break;
            }
            tx_thread_sleep(0x20);
        }   
    }
}
uint32_t value;

bool verifyStringFromServer(char *rest, uint64_t * data, uint32_t len)
{
    char delimiter[2] = ",";
    if (rest == NULL || *rest == '\0')
    {
        return false;
    }
    int i = 0;
    while (*rest != '\0')
    {
        if (i == len)
        {
            break;
        }
        value = (uint32_t)atoll((const char *)strtok_r(NULL, delimiter, &rest));
        
        data[i] = value;
        i++;
    }
    if (i < len)
    {
        return false;
    }
    return true;
}

uint8_t dataParserCOTA(uint64_t *configData, char * server_cmd, char * delimiter)
{
    bmsStatus_t status;
    unsigned int recv_crc = 0;
    memcpy(&recv_crc, firmware_up_recv_message.payload + 1, 4); 

    if (verifyStringFromServer(server_cmd + 5, configData, FLASH_CONFIG_TOTAL_LENGTH - 1))
    {
        
        status = writeDataToFlash(FLASH_COTA_UPDATE, FLASH_CONFIG_ADDRESS, configData, FLASH_CONFIG_TOTAL_LENGTH, (uint8_t *)&recv_crc);
        return status;
    }
    else
    {
        status = BMS_COTA_UPDATE_ERROR;
        return status;
    }
}

int update_config(uint64_t *configData)
{
    bmsStatus_t status;
    char delimiter[2] = ",";
    status = dataParserCOTA(configData, ((char *)firmware_up_recv_message.payload), delimiter);  

    return status;

    //set flag and ackowledge to Stark here
}
