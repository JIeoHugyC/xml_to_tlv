//
// Created by Leonid Kabanov on 30.07.2022.
//

#include "tlv_maker.h"
#include <cxml/cxml.h>


void createTlv(){

  // create an event reader object ('true' allows the reader to close itself once all events are exhausted)
  cxml_sax_event_reader reader = cxml_stream_file("test.xml", true);

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
      printf("Element: `%s`\n", cxml_string_as_raw(&name));
      cxml_string_free(&name);
    }
      // or a text event
    else if (event == CXML_SAX_TEXT_EVENT)
    {
      printf("Text event\n\n\n");
      // consume the current event by collecting the text data
      cxml_sax_get_text_data(&reader, &text);
      printf("Text: `%s`\n", cxml_string_as_raw(&text));
      printf("string length: %i\n", text._len);
      printf("string cap: %i\n", text._cap);
      cxml_string_free(&text);
    }
  }
}