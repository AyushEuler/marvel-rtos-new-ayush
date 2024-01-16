# can-tp-module
Can tp module that can be integrated with threadx rtos system

Add following paths to Cmakelists.txt 

Includes:
```cmake
/
/isotp-c/src
/isotp-c/deps/bitfield-c/src
```

Sources:
```cmake
/isotp-c/deps/bitfield-c/src/bitfield/8byte.c
/isotp-c/deps/bitfield-c/src/bitfield/bitarray.c
/isotp-c/deps/bitfield-c/src/bitfield/bitfield.c
/isotp-c/src/isotp/isotp.c
/isotp-c/src/isotp/receive.c
/isotp-c/src/isotp/send.c
/can_tp_app.c
```

## Usage
- Setup:
    - Include header with Transmit_on_CAN function in [can_tp_app.c](can_tp_app.c) (can_AL.h in stark, can_lib.h in cluster)
    - set the maximum message size in [isotp-c/src/isotp/isotp_types.h](isotp-c/src/isotp/isotp_types.h) on line 9 and 13 (maximum tested with 1024 size)
- Transmit:
    - Transmit function can be used with any thread. 
    - flow control flag needs to be set in can-interrupt when flow control is received.
    - thread will be blocked until tranmission is done.
- Receive:
    - Receive function is to be placed inside can interrupt.
    - message receiving is completed when the flag bit is set.

## Transmit Function
```c
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

uint8_t CAN_TP_Transmit(uint32_t arbitration_id, uint8_t* message, uint16_t message_length, unsigned long flow_control_bit, unsigned long flow_control_wait, TX_EVENT_FLAGS_GROUP* flow_control_flag);
```

## Receive Function
```c
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
void CAN_TP_Receive_interrupt(uint32_t arbitration_id, uint8_t* data, uint16_t dlc, IsoTpShims *shim, IsoTpMessage* recv_mess, IsoTpReceiveHandle* recv_handle, ULONG completiton_flag_bit, TX_EVENT_FLAGS_GROUP* can_tp_receiver_flag);
```
