//
// Created by Leonid Kabanov on 30.07.2022.
//

#ifndef XML_TO_TLV_TLV_RECORD_H
#define XML_TO_TLV_TLV_RECORD_H
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

static const uint8_t CLASS_OF_TAG = 1<<7 | 1<<6; // custom class
static const uint8_t TYPE_OF_TAG = 1 << 5; // simple TLV
static const uint8_t TEMPLATE_OF_TAG = CLASS_OF_TAG | TYPE_OF_TAG;

static const uint8_t TAG_TEXT = TEMPLATE_OF_TAG | 1; // let string data be number 1
static const uint8_t TAG_NUMERIC = TEMPLATE_OF_TAG | 2; // let numeric data be number 2

typedef struct{
  uint8_t tag;
  uint8_t* length;
  uint8_t* data;
} TlvData;

typedef struct{
  uint8_t* data;
  uint32_t length;
} TlvEntry;

typedef struct TlvRecord{
  void* private;
  void (*dispose)(struct TlvRecord*);
  TlvData* (*getTvlData)(struct TlvRecord*); // @visible for testing
  TlvEntry (*getTvlBytesArray)(struct TlvRecord*);
} TlvRecord;

TlvRecord* newTlvRecord(const char* tag, unsigned int tagLength,
                        const char* data, unsigned int dataLength);
#endif //XML_TO_TLV_TLV_RECORD_H
