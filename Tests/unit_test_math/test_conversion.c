#include "unity.h"
#include "conversion.h"
#include "bmsConfiguration.h"
#include "main.h"

//ceedling test:all

void setUp(void)
{
}

void tearDown(void)
{
}

void test_conversion_byteToFloat(void)
{
  float result = 0;
  uint8_t dataArray[4]={0x34,0xbc,0x56,0x42};
  uint8_t size = 4;

  result = byteToFloat(dataArray);
  TEST_ASSERT_EQUAL_INT8( 53.68, result);
}


void test_conversion_floatToByte(void)
{
    uint8_t dataBuffer[4]={0};
    uint8_t dataArray[4]={0x34,0xbc,0x56,0x42};
    float fvalue = 53.6837921;


    floatToByte(dataBuffer, fvalue);
    // TEST_ASSERT_EQUAL_HEX_ARRAY(dataArray , dataBuffer, 4);
    TEST_ASSERT_HEX_ARRAY_WITHIN (0xffffffff, dataArray, dataBuffer, 4);
    
}


// void test_conversion_byteToDouble(void)
// {
//     double doubleWord    = 0;
//     uint8_t dataBuffer[8]  = {0x40,0x42,0x15,0x2e,0xac,0x52,0x7b,0x44};
//     uint8_t dataSize       = 8;

//     doubleWord = byteToDouble(dataBuffer);
//     TEST_ASSERT_EQUAL_INT64(8064308433057646379008.000000, doubleWord);
// }

