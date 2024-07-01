project(cFILE
    VERSION 1.0
    DESCRIPTION "C library for file management"
    LANGUAGES C
)

add_library(${PROJECT_NAME} STATIC
    cFILE.c
    cFILE.h
)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
