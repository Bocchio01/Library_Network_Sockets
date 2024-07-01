project(cJSON
    VERSION 1.0
    DESCRIPTION "C library for JSON parsing"
    LANGUAGES C
)

add_library(${PROJECT_NAME} STATIC
    cJSON.c
    cJSON.h
)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})