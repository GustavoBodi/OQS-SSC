cmake_minimum_required(VERSION 3.22)
project(oqs)
enable_testing()

set(CMAKE_CXX_FLAGS_INIT "-Wall -std=c++20")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(OPENSSL_ROOT_DIR "/home/labsec/quantumsafe/openssl")
set(OQS_INCLUDE_DIR "/home/labsec/quantumsafe/build/include")

set(PROJECT_SOURCE_DIR "src")
set(PROJECT_SOURCE_LIB_MODULE "${PROJECT_SOURCE_DIR}")
set(PROJECT_INCLUDE_DIR "include")
set(PROJECT_TEST_DIR "tests")
include_directories("./include/")
include_directories("${OQS_INCLUDE_DIR}")

file (GLOB SRC_FILES
  "${PROJECT_SOURCE_LIB_MODULE}/*.cpp")

file (GLOB INCLUDE_FILES
  "${PROJECT_INCLUDE_DIR}/*.h*")

file (GLOB TEST_FILES
  "${PROJECT_TEST_DIR}/*.cpp")

add_executable(oqs "${PROJECT_SOURCE_DIR}/main.cpp" ${SRC_FILES} ${INCLUDE_FILES})
find_package(OpenSSL REQUIRED)
target_link_libraries(oqs OpenSSL::SSL)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})
