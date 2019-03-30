cmake_minimum_required(VERSION 3.9)

project(syscall_capture VERSION 0.0.1 DESCRIPTION "system calls capture")

set(SOURCES
    clocks.cpp
    header_writer.cpp
    managed_buffers.cpp
    syscall_capture.cpp
    syscall_info.cpp
    thread_id.cpp
    tlv.cpp)

set (CMAKE_CXX_STANDARD 17)

add_library(syscall_capture SHARED ${SOURCES})
target_compile_options(syscall_capture PRIVATE -fno-rtti -fno-exceptions -fno-threadsafe-statics -g)
target_compile_options(syscall_capture PRIVATE -Wall -Wextra -pedantic -Werror)
target_link_libraries(syscall_capture -nodefaultlibs c syscall_intercept -Wl,--no-undefined)
