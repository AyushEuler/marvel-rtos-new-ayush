#ifndef __CAN_TP_TEST_H__
#define __CAN_TP_TEST_H__

#include "stdint.h"
#include "isotp/isotp.h"
#include "tx_api.h"
#ifndef MARVEL_CANTP
#include "enumerators.h"
#endif

void can_tp_tx_hw(const uint32_t arbitration_id, uint8_t* data, const uint8_t size);
void set_timer(uint16_t time_ms, void (*callback));

uint8_t CAN_TP_Transmit(uint32_t arbitration_id, uint8_t* message, uint16_t message_length, unsigned long flow_control_bit, unsigned long flow_control_wait, TX_EVENT_FLAGS_GROUP* flow_control_flag);

void CAN_TP_Receive_interrupt(uint32_t arbitration_id, uint8_t* data, uint16_t dlc, IsoTpShims *shim, IsoTpMessage* recv_mess, IsoTpReceiveHandle* recv_handle, unsigned long completiton_flag_bit, TX_EVENT_FLAGS_GROUP* can_tp_receiver_flag);

#endif