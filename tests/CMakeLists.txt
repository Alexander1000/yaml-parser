cmake_minimum_required(VERSION 3.7)

# compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

project(yaml-parser-tests)

find_library(YAML_PARSER libyaml-parser.a)
message(STATUS "Found yaml-parser library: ${YAML_PARSER}")

find_library(IO_BUFFER libio-buffer.a)
message(STATUS "Found io-buffer library: ${IO_BUFFER}")

add_executable(${PROJECT_NAME} yaml.cpp)

target_link_libraries(${PROJECT_NAME}
    ${YAML_PARSER}
    ${IO_BUFFER}
)
