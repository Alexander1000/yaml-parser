cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

set(YAML_PARSER_SOURCES
    src/stream.cpp
    src/tokens/token.cpp
    src/tokens/indent.cpp
    src/tokens/property.cpp
    src/tokens/plain_value.cpp
    src/tokens/dash.cpp
    src/tokens/pipe.cpp
    src/decoder.cpp
    src/element.cpp
)

if(NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
    set(CMAKE_INSTALL_INCLUDEDIR include)
endif(NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)

# installation of json-stream-analyzer CXX library headers
install(DIRECTORY include/yaml-parser DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

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

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE src)

find_library(IO_BUFFER libio-buffer.a)
message(STATUS "Found io-buffer library: ${IO_BUFFER}")

include_directories(/usr/local/include)

target_link_libraries(${PROJECT_NAME} ${IO_BUFFER})

install (TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    COMPONENT library
)
