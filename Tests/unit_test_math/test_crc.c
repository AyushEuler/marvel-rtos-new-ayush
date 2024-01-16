#include "unity.h"
#include "crc.h"

//ceedling test:all

void setUp(void)
{
}

void tearDown(void)
{
}

void test_crc_case_0(void)
{
  uint16_t result = 0;
  uint8_t data[8] = {0}; 
  uint32_t size = 8;

  result = calculateCRC(data, size);
  TEST_ASSERT_EQUAL_INT8( 105, result );

//   calculateCRC_ExpectAndReturn(data, size, 105);
//   result = calculateCRC(data, size);
}
