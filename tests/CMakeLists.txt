cmake_minimum_required(VERSION 3.7)

# compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

project(yaml-parser-tests)

set(YAML_PARSER_SOURCES
    ../src/stream.cpp
    ../src/tokens/token.cpp
    ../src/tokens/indent.cpp
    ../src/tokens/property.cpp
    ../src/tokens/plain_value.cpp
    ../src/tokens/dash.cpp
    ../src/tokens/pipe.cpp
    ../src/decoder.cpp
    ../src/element.cpp
)

# find_library(YAML_PARSER libyaml-parser.a)
# message(STATUS "Found yaml-parser library: ${YAML_PARSER}")

find_library(IO_BUFFER libio-buffer.a)
message(STATUS "Found io-buffer library: ${IO_BUFFER}")

find_library(CPP_UNIT_TEST libcpp-unit-test.a)
message(STATUS "Found cpp-unit-test: ${CPP_UNIT_TEST}")

add_executable(
    ${PROJECT_NAME} yaml.cpp
    ${YAML_PARSER_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    ${IO_BUFFER}
    ${CPP_UNIT_TEST}
)
