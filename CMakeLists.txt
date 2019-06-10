cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(YAML_PARSER_SOURCES
    src/stream.cpp
    src/tokens/token.cpp
)

#----------------#
# Static library #
#----------------#

project(yaml-parser VERSION 0.0.1 DESCRIPTION "yaml-parser")

add_library(${PROJECT_NAME} STATIC ${YAML_PARSER_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/yaml-parser.h)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "yaml-parser")
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -fPIC)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

find_library(IO_BUFFER libio-buffer.a)
message(STATUS "Found io-buffer library: ${IO_BUFFER}")

target_link_libraries(${PROJECT_NAME} ${IO_BUFFER})

install (TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT library
)
