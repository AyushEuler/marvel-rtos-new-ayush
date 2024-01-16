#include "unity.h"
#include "bmsConfiguration.h"
#include "mock_flashControls.h"

//ceedling test:all

void setUp(void)
{
}

void tearDown(void)
{
}


void test_flashMemory_case_0(void)
{
    uint32_t startAddress = 0x00;
    uint64_t dataBuffer[8]={0};
    uint16_t dataSize     = 8;
    uint8_t result        = 0;

    // result = flashWrite(startAddress,dataBuffer,dataSize);
    // printf("print %d", result);
    // TEST_ASSERT_EQUAL_UINT8 (0, result);
    
    // In the test case, set up the expectation for the flashWrite function call
    flashWrite_ExpectAndReturn(startAddress, dataBuffer, dataSize, BMS_OK);

    // Call the function being tested
    result = flashWrite(startAddress, dataBuffer, dataSize);
    printf("result: %d", result);
    // Perform the test assertion
    TEST_ASSERT_EQUAL_UINT8 (BMS_OK, result);

}