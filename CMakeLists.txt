cmake_minimum_required(VERSION 3.26)
project(catgirl)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-DCC_TARGET_OS_IPHONE)
execute_process(COMMAND xcrun --show-sdk-path --sdk iphoneos
	OUTPUT_VARIABLE IOS_SDK
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_OSX_SYSROOT ${IOS_SDK})

add_subdirectory(auby2)

add_library(catgirl SHARED
  src/main.cc
)

target_link_libraries(catgirl PUBLIC auby2)
