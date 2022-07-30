//
// Created by Leonid Kabanov on 30.07.2022.
//
#include "tlv_record.h"

const char* STR_TEXT = "text";
const char* STR_NUMERIC = "numeric";

bool fillTag(TlvRecord*, const char*, unsigned int);
bool fillLengthAndData(TlvRecord*, const char*, unsigned int);
static bool invertByteOrder = false;

typedef struct{
  TlvRecord* this;
  TlvData* tlvData;
  uint32_t lengthSize;
  uint32_t dataSize;
} Private;

TlvData* getTlvData(TlvRecord* tlvRecord){
  Private* private = ((Private*)(tlvRecord->private));
  return private->tlvData;
}


TlvEntry getTvlBytesArray(struct TlvRecord* tlvRecord){
  Private* private = ((Private*)(tlvRecord->private));
  TlvEntry* entry = (TlvEntry*)malloc(sizeof(TlvEntry));
  entry->length = 1 /*tag always 1 byte size*/ + private->lengthSize + private->dataSize;
  entry->data = malloc(entry->length);
  uint8_t * entryPtr = entry->data;
  memcpy(entryPtr, &private->tlvData->tag, 1);
  entryPtr++;
  memcpy(entryPtr, private->tlvData->length, private->lengthSize);
  entryPtr += private->lengthSize;
  memcpy(entryPtr, private->tlvData->data, private->dataSize);
  return *entry;
}

void dispose(TlvRecord* tlvRecord){
  Private* private = ((Private*)(tlvRecord->private));
  free(private->tlvData->length);
  if (private->dataSize > 0){
    free(private->tlvData->data);
  }
  free(private->tlvData);
  free(private->this);
}

TlvRecord* newTlvRecord(const char* const tag, unsigned int tagLength,
                        const char* const data, unsigned int dataLength){
  TlvRecord* tlvRecord;
  tlvRecord = (TlvRecord*)malloc(sizeof(TlvRecord));
  tlvRecord->private = (Private*)malloc(sizeof(Private));
  Private* private = ((Private*)(tlvRecord->private));
  private->this = tlvRecord;
  private->tlvData = (TlvData*) malloc(sizeof (TlvData));
  private->lengthSize = 0;
  private->dataSize = 0;
  if (!fillTag(tlvRecord, tag, tagLength)){
    return 0;
  }
  if (!fillLengthAndData(tlvRecord, data, dataLength)){
    return 0;
  }
  tlvRecord->dispose = dispose;
  tlvRecord->getTvlData = getTlvData;
  tlvRecord->getTvlBytesArray = getTvlBytesArray;
  uint16_t testVal = 0x00FF;
  invertByteOrder = ((uint8_t*)&testVal);
  return tlvRecord;
}

bool fillLengthAndData(TlvRecord* tlvRecord, const char* const data, unsigned int dataLength){

  // Important! I do it on the assumption that if we need to save space, we'll define
  // our own numeric types. Here, I think, it's enough to think than
  // numeric arguments is not bigger than long int and on both systems
  // (from and to) it will have the same size (usually, it's not)

  Private* private = ((Private*)(tlvRecord->private));
  if (private->tlvData->tag == TAG_NUMERIC){
    long int val = strtol(data, 0, 0);
    if (val == LONG_MAX || val == LONG_MIN){
      return false;
    }
    // length
    private->lengthSize = 1;
    private->tlvData->length = malloc(private->lengthSize);
    *private->tlvData->length = sizeof(long int);
    // data
    private->dataSize = sizeof(long int);
    private->tlvData->data = malloc(private->dataSize);
    // fill fields
    if (invertByteOrder){
      for (int i = 0; i < sizeof(long int); i++){
        private->tlvData->data[sizeof(long int)-i-1] = *((uint8_t*)&val + i);
      }
    }
    else{
      for (int i = 0; i < sizeof(long int); i++){
        private->tlvData->data[i] = *((uint8_t*)&val + i);
      }
    }

  }
  else if (private->tlvData->tag == TAG_TEXT){
    if (dataLength <= 0x7F){
      private->lengthSize = 1;
    }
    else if (dataLength <= 0xFF){
      private->lengthSize = 2;
    }
    else if (dataLength <= 0xFFFF){
      private->lengthSize = 3;
    }
    else if (dataLength <= 0xFFFFFF){
      private->lengthSize = 4;
    }
    else{
      return false;
    }
    private->dataSize = dataLength;
    private->tlvData->length = malloc(private->lengthSize);
    private->tlvData->length[0] = (private->lengthSize > 1)? 0x80+private->lengthSize-1 : dataLength;
    if (private->lengthSize > 1){
      memcpy(private->tlvData->length+1, &dataLength, private->lengthSize);
    }
    private->tlvData->data = malloc(dataLength);
    memcpy(private->tlvData->data, data, dataLength);
  }
  else{
    return false;
  }
  return true;

}

bool fillTag(TlvRecord* tlvRecord, const char* const tag, unsigned int length){
  Private* private = ((Private*)(tlvRecord->private));
  if (strcmp(tag, STR_TEXT) == 0){
    private->tlvData->tag = TAG_TEXT;
  }
  else if (strcmp(tag, STR_NUMERIC) == 0) {
    private->tlvData->tag = TAG_NUMERIC;
  }
  else{
    // we can't create object with invalid tag
    return false;
  }
  return true;
}
