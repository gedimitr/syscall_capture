cmake_minimum_required(VERSION 3.9)

project(syscall_capture VERSION 0.0.1 DESCRIPTION "system calls capture")

set(SOURCES
    managed_buffers.cpp
    syscall_capture.cpp
    syscall_info.cpp)

add_library(syscall_capture SHARED ${SOURCES})
target_link_libraries(syscall_capture -nodefaultlibs -static-libgcc c syscall_intercept)
set_property(TARGET syscall_capture APPEND PROPERTY COMPILE_FLAGS "-g")
target_compile_options(syscall_capture PRIVATE -Wall -Wextra -pedantic -Werror)
set (CMAKE_CXX_STANDARD 11)
