#include "can_tp_app.h"
#include "isotp/isotp.h"

#define MARVEL_CANTP

#ifdef STARK_CANTP
#include "can_AL.h"
#endif
#ifdef MARK_CANTP
#include "can_lib.h"
#endif
#ifdef MARVEL_CANTP
#include "main.h"
#include "canControls.h"
#include "onBoardCANv2b.h"
#endif

void can_tp_tx_hw(const uint32_t arbitration_id, uint8_t* data, const uint8_t size){
    tx_thread_sleep(2); 
#ifndef MARVEL_CANTP
    Transmit_on_CAN1(arbitration_id, S, data, size);
#endif
#ifdef MARVEL_CANTP
    sendCAN(communicationCAN, STANDARD_CAN_ID, arbitration_id, data, size);
#endif
}

void set_timer(uint16_t time_ms, void (*callback)){}

/**
 * This function transmits a CAN message using ISO-TP protocol with flow control and returns a success
 * or failure status.
 * 
 * @param arbitration_id The ID used to identify the message being transmitted on the CAN bus.
 * @param message A pointer to the message data to be transmitted.
 * @param message_length The length of the message to be transmitted in bytes.
 * @param flow_control_bit This parameter is used to specify which bit in the event flags group should
 * be set to indicate that flow control has been received.
 * @param flow_control_wait The maximum amount of time (in ticks) to wait for the flow control flag to
 * be set before giving up and returning an error.
 * @param flow_control_flag A pointer to a TX_EVENT_FLAGS_GROUP object that is used for flow control.
 * This is a synchronization object used to signal when the receiver is ready to receive more data.
 * 
 * @return a uint8_t value, which could be either 0 or 1 depending on whether the message transmission
 * was successful or not.
 */
uint8_t CAN_TP_Transmit(uint32_t arbitration_id, uint8_t* message, uint16_t message_length, ULONG flow_control_bit, ULONG flow_control_wait, TX_EVENT_FLAGS_GROUP* flow_control_flag)
{
    // tx_event_flags_set(flow_control_flag, flow_control_bit, TX_AND);
    IsoTpShims shim = isotp_init_shims((void*)printf, (void*)can_tp_tx_hw, (void*)set_timer);
    IsoTpSendHandle send_handle = isotp_send(&shim, arbitration_id, message, message_length, NULL);
    if(send_handle.completed){
        if(!send_handle.success){
                printf("Message transmission failed early!!\n");
                return 0;
            }
            else{
                printf("Single frame message sent!!\n");
                return 1;
            }
    }
    else{
        ULONG actual_flag;
        if(tx_event_flags_get(flow_control_flag, flow_control_bit, TX_OR_CLEAR, &actual_flag, flow_control_wait) == TX_SUCCESS){
            while(true){
                bool completed = isotp_continue_send(&shim, &send_handle, arbitration_id, message, message_length);
                if(completed && send_handle.completed){
                    if(send_handle.success){
                        return 1;
                    }
                    else{
                        return 0;
                    }
                }
            }
        }
        else{
            return 0;
        }
    }
    return 0;
}

/**
 * The function receives CAN messages and sets a flag when the message is completed.
 * 
 * @param arbitration_id The ID of the CAN message being received.
 * @param data A pointer to the data received over CAN bus.
 * @param dlc The length of the received CAN message data.
 * @param shim A pointer to an IsoTpShims struct, which contains function pointers for printf, CAN
 * transmit, and timer functions used by the ISO-TP protocol.
 * @param recv_mess A pointer to an IsoTpMessage struct that contains information about the received
 * message.
 * @param recv_handle A pointer to an IsoTpReceiveHandle struct, which is used to keep track of the
 * state of the ongoing receive operation. It is updated by the isotp_receive and
 * isotp_continue_receive functions.
 * @param completiton_flag_bit This parameter is a bit position in a flag variable that is used to
 * indicate the completion of the CAN TP receive operation. The bit at this position will be set when
 * the receive operation is completed.
 * @param can_tp_receiver_flag A pointer to a TX_EVENT_FLAGS_GROUP object that is used to signal the
 * completion of the receive operation.
 */
void CAN_TP_Receive_interrupt(uint32_t arbitration_id, uint8_t* data, uint16_t dlc, IsoTpShims *shim, IsoTpMessage* recv_mess, IsoTpReceiveHandle* recv_handle, ULONG completiton_flag_bit, TX_EVENT_FLAGS_GROUP* can_tp_receiver_flag)
{
    uint8_t frame_type = data[0] & 0xF0;
    if(frame_type == (uint8_t)0x10 || frame_type == (uint8_t)0x00)
    {
        *shim = isotp_init_shims((void*)printf, (void*)can_tp_tx_hw, (void*)set_timer);
        *recv_handle = isotp_receive(shim, arbitration_id, NULL);
    }
    isotp_continue_receive(shim, recv_mess, recv_handle, arbitration_id, data, dlc);
    if(recv_mess->completed && recv_handle->completed)
    {
        tx_event_flags_set(can_tp_receiver_flag, completiton_flag_bit, TX_OR);
    }
}