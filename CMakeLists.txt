cmake_minimum_required(VERSION 3.15)
project(xml_to_tlv
        VERSION 0.0.1
        DESCRIPTION "An application for conversion XML to TLV"
        LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED  ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(FindPkgConfig)
find_package(cxml REQUIRED)

include_directories("include/")
set(SRC_LIST src/main.c src/tlv_maker.c src/tlv_record.c src/tlv_record.c)
add_executable(${PROJECT_NAME} ${SRC_LIST} )

target_include_directories(${PROJECT_NAME} PUBLIC ${CXML_INCLUDE_DIR})
target_compile_definitions(${PROJECT_NAME} PUBLIC "CXML_USE_XPATH_MOD" "CXML_USE_QUERY_MOD" "CXML_USE_SAX_MOD")

target_link_libraries(${PROJECT_NAME} ${CXML_LIBRARY})
