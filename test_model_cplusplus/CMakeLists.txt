cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(onnxtest)

option(USE_ONNX_LIB "USE_ONNX_LIB" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DELOG)
set(CMAKE_CXX_FLAGS "-fpermissive -fPIC -lm")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../simulation-2d-cpp/onnxruntime/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../simulation-2d-cpp/onnxruntime/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

set(RUNTIME_TEST learning_models.cpp)

foreach(src ${RUNTIME_TEST})
  list(APPEND RUNTIME_SOURCES ${src})
endforeach(src ${RUNTIME_TEST})

add_library(source_libs STATIC ${RUNTIME_SOURCES})
target_link_libraries(source_libs -Wl,--allow-multiple-definition -Wl,--no-whole-archive)

add_executable(runtimetest main.cpp)
target_link_libraries(runtimetest onnxruntime source_libs)

set_property(TARGET runtimetest PROPERTY CXX_STANDARD 20)