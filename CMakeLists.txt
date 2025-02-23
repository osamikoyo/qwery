cmake_minimum_required(VERSION 3.10)
project(Cwindow)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})

add_executable(cwindow src/main.c
        src/words.c
        src/words.h
)
target_link_libraries(cwindow ${GTK3_LIBRARIES})
