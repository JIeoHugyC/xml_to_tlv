#include "tlv_maker.h"
#include <stdio.h>

void printHelp(){
  printf("Usage:\nAdd necessary key\n"
         "--f xml_file_name.xml\n"
         "Also you can specify output file name by setting\n"
         "--o output_file_name.tlv\n"
         "By default, program will create out.tlv\n");
}

int main(int argc, char* argv[]){
  if (argc < 3){
    printHelp();
    return 0;
  }
  char* xml = 0;
  char* out = 0;
  for (int i = 1; i < argc; ++i){
    if (strcmp(argv[i], "--f") == 0){
      printf("-f found!\n");
      if (i+1 <= argc-1 && argv[i+1] != 0){
        xml = argv[i+1];
        printf("%s\n", xml);
      }
    }
    if (strcmp(argv[i], "--o") == 0){
      if (i+1 <= argc-1 && argv[i+1] != 0){
        out = argv[i+1];
      }
    }
  }
  if (xml == 0){
    printf("Please, specify XML file to convert\n");
  }
  if (createTlv(xml, out)){
    printf("TLV file was created\n");
  }
  else{
    printf("An error occurred during TLV creation\n");
  }
  return 0;
}