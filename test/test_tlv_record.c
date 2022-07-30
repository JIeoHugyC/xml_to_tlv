#include "unity.h"
#include "tlv_record.h"

void setUp(void){

}
void tearDown(void){
}
void test_fillLengthAndData( void ){
  TlvRecord* record0 = newTlvRecord("value", 4, "abc", 3);
  TEST_ASSERT_EQUAL_INT(record0, 0);

  TlvRecord* record = newTlvRecord("text", 4, "abc", 3);
  TEST_ASSERT_NOT_EQUAL_INT(record, 0);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->tag, 0xE1);
  TEST_ASSERT_NOT_EQUAL_INT(record->getTvlData(record)->data, 0);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->data[0], 'a');
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->data[1], 'b');
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->data[2], 'c');
  TEST_ASSERT_NOT_EQUAL_INT(record->getTvlData(record)->length, 0);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->length[0], 3);

}