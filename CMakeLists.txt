cmake_minimum_required(VERSION 3.9)

project(syscall_capture VERSION 0.0.1 DESCRIPTION "system calls capture")
add_library(syscall_capture SHARED syscall_capture.c syscall_info.c)
target_link_libraries(syscall_capture syscall_intercept)
set_property(TARGET syscall_capture APPEND PROPERTY COMPILE_FLAGS "-g")
