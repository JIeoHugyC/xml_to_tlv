set(FIND_CXML_PATHS "/Users/Leonid_Kabanov/Documents/projects/test_task/lib/cxml")
find_path(CXML_INCLUDE_DIR  cxml
        PATH_SUFFIXES include
        PATHS ${FIND_CXML_PATHS})
find_library(CXML_LIBRARY
        NAMES cxml
        PATH_SUFFIXES lib
        PATHS ${FIND_CXML_PATHS})