//
// Created by Leonid Kabanov on 30.07.2022.
//

#include "tlv_maker.h"
#include <cxml/cxml.h>


bool createTlv(const char* fileName, char* tlvFileName){
  FILE *fp;
  if (tlvFileName == 0){
    tlvFileName = "out.tlv";
  }
  remove(tlvFileName);
  // create an event reader object ('true' allows the reader to close itself once all events are exhausted)
  cxml_sax_event_reader reader = cxml_stream_file(fileName, true);

  // event object for storing the current event
  cxml_sax_event_t event;

  // cxml string objects to store name and text
  cxml_string name = new_cxml_string();
  cxml_string text = new_cxml_string();

  while (cxml_sax_has_event(&reader)){ // while there are events available to be processed.
    // get us the current event
    event = cxml_sax_get_event(&reader);

    // check if the event type is the beginning of an element
    if (event == CXML_SAX_BEGIN_ELEMENT_EVENT){
      // consume the current event by collecting the element's name
      cxml_sax_get_element_name(&reader, &name);
    }
    // or a text event
    else if (event == CXML_SAX_TEXT_EVENT)    {
      // consume the current event by collecting the text data
      cxml_sax_get_text_data(&reader, &text);
      printf("Element: `%s`\n", cxml_string_as_raw(&name));
      printf("Text: `%s`\n", cxml_string_as_raw(&text));
      if (name._raw_chars != 0 && text._raw_chars != 0) {
        TlvRecord *tlvRecord = newTlvRecord(name._raw_chars,
                                            name._len,
                                            text._raw_chars,
                                            text._len);
        if (tlvRecord != 0) {
          printf("Tlv record created!\n");
          TlvEntry entry = tlvRecord->getTvlBytesArray(tlvRecord);
          tlvRecord->dispose(tlvRecord);
          printf("length = %i\n", entry.length);
          for (int i = 0; i < entry.length; ++i) {
            printf("%u ", entry.data[i]);
          }
          if ((fp = fopen(tlvFileName, "a")) == NULL) {
            printf("Couldn't open file %s", fileName);
            return false;
          }
          fwrite(entry.data, 1, entry.length, fp);
          free(entry.data);
          fclose(fp);
        }
      }
      cxml_string_free(&text);
      cxml_string_free(&name);
    }
  }
  return true;
}