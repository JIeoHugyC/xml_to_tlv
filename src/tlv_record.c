//
// Created by Leonid Kabanov on 30.07.2022.
//

#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include "tlv_record.h"

const char* STR_TEXT = "text";
const char* STR_NUMERIC = "numeric";

bool fillTag(TlvRecord*, const char*, unsigned int);
bool fillLengthAndData(TlvRecord*, const char*, unsigned int);

typedef struct{
  TlvRecord* this;
  TlvData* tlvData;
  uint8_t lengthSize;
  uint8_t dataSize;
} Private;

TlvData* getTlvData(TlvRecord* tlvRecord){
  Private* private = ((Private*)(tlvRecord->private));
  return private->tlvData;
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
  private->lengthSize = -1;
  private->dataSize = -1;
  if (!fillTag(tlvRecord, tag, tagLength)){
    return 0;
  }
  if (!fillLengthAndData(tlvRecord, data, dataLength)){
    return 0;
  }
  tlvRecord->dispose = dispose;
  tlvRecord->getTvlData = getTlvData;
  return tlvRecord;
}

bool fillLengthAndData(TlvRecord* tlvRecord, const char* const data, unsigned int dataLength){

  // Important! I do it on the assumption that if we need to save space, we'll define
  // our own numeric types. Here, I think, it's enough to think than
  // numeric arguments is not bigger than long int and on both systems
  // (from and to) it will have the same size and byte order (usually, it's not)

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
    memcpy(private->tlvData->data, &val, private->dataSize);
  }
  else if (private->tlvData->tag == TAG_TEXT){
    if (dataLength <= 0x7F){
      private->lengthSize = 1;
    }
    else if (dataLength < 0xFF){
      private->lengthSize = 2;
    }
    else if (dataLength < 0xFFFF){
      private->lengthSize = 3;
    }
    else if (dataLength < 0xFFFFFF){
      private->lengthSize = 4;
    }
    else{
      return false;
    }
    private->tlvData->length = malloc(private->lengthSize);
    *private->tlvData->length = dataLength;
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
