cmake_minimum_required(VERSION 3.1)
project(AES)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -O3 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SRCS aescrypto.cpp bytevector.cpp utils.h)
add_library(aes ${SRCS})

add_executable(main main.cpp)
target_link_libraries(main aes)

add_executable(test_aes test.cpp)
target_link_libraries(test_aes aes)