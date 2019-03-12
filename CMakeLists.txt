cmake_minimum_required(VERSION 3.9)

project(syscall_capture VERSION 0.0.1 DESCRIPTION "system calls capture")

set(SOURCES
    managed_buffers.c
    syscall_capture.c
    syscall_info.c)

add_library(syscall_capture SHARED ${SOURCES})
target_link_libraries(syscall_capture syscall_intercept)
set_property(TARGET syscall_capture APPEND PROPERTY COMPILE_FLAGS "-g")
