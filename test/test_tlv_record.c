#include "unity.h"
#include "tlv_record.h"
#include "stdio.h"

void setUp(void){

}
void tearDown(void){
}
void test_fillLengthAndData( void ){
  TlvRecord* record = newTlvRecord("value", 5, "abc", 3);
  TEST_ASSERT_EQUAL_INT(record, 0);

  record = newTlvRecord("text", 4, "abc", 3);
  TEST_ASSERT_NOT_EQUAL_INT(record, 0);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->tag, 0xE1);
  TEST_ASSERT_NOT_EQUAL_INT(record->getTvlData(record)->length, 0);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->length[0], 3);
  TEST_ASSERT_NOT_EQUAL_INT(record->getTvlData(record)->data, 0);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->data[0], 'a');
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->data[1], 'b');
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->data[2], 'c');
  record->dispose(record);

  record = newTlvRecord("numeric", 7, "0", 4);
  TEST_ASSERT_NOT_EQUAL_INT(record, 0);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->tag, 0xE2);
  TEST_ASSERT_NOT_EQUAL_INT(record->getTvlData(record)->length, 0);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->length[0], sizeof(long int));
  for (int i = 0; i < sizeof(long int); ++i){
    TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[i], 0);
  }
  record->dispose(record);

  record = newTlvRecord("numeric", 7, "1", 4);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[0], 1);
  record->dispose(record);

  record = newTlvRecord("numeric", 7, "255", 4);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[0], 255);
  record->dispose(record);

  record = newTlvRecord("numeric", 7, "256", 4);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[0], 0);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[1], 1);
  record->dispose(record);

  record = newTlvRecord("numeric", 7, "256", 4);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[0], 0);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[1], 1);
  record->dispose(record);

  record = newTlvRecord("numeric", 7, "-1", 4);
  for (int i = 0; i < sizeof(long int); ++i){
    TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[i], 0xFF);
  }
  record->dispose(record);

  record = newTlvRecord("numeric", 7, "-256", 4);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[0], 0);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[1], 0xFF);
  record->dispose(record);


  record = newTlvRecord("numeric", 7, "205011", 4);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[0], 211);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[1], 32);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[2], 3);
  for (int i = 3; i < sizeof(long int); ++i){
    TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[i], 0);
  }
  record->dispose(record);

  record = newTlvRecord("numeric", 7, "7", 4);
  TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[0], 7);
  for (int i = 1; i < sizeof(long int); ++i){
    TEST_ASSERT_EQUAL_UINT(record->getTvlData(record)->data[i], 0);
  }
  record->dispose(record);

  int bigStringRepeat = 255;
  char* pattern = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int patternLength = 26;
  char* bigString = malloc(patternLength*bigStringRepeat);
  char* ptr = bigString;
  for(int i = 0; i < bigStringRepeat; ++i){
    memcpy(ptr, pattern, patternLength);
    ptr+=patternLength;
  }
  record = newTlvRecord("text", 4, bigString, patternLength*bigStringRepeat);
  TEST_ASSERT_NOT_EQUAL_INT(record->getTvlData(record)->length, 0);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->length[0], 0x82);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->length[1], 230);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->length[2], 25);
  TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->data[0], 'A');
  char* patternPtr = pattern;
  for (int i = 0; i < bigStringRepeat; ++i){
    TEST_ASSERT_EQUAL_INT(record->getTvlData(record)->data[i], *patternPtr);
    (*patternPtr == 'Z')? patternPtr = pattern : ++patternPtr;
  }
  record->dispose(record);


}
