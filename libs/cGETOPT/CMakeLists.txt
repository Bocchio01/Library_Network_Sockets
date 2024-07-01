project(cGETOPT
    VERSION 1.0
    DESCRIPTION "Custom getopt for C"
    LANGUAGES C
)

add_library(${PROJECT_NAME} STATIC
    getopt.c
    getopt.h
)

if(MSVC)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -w)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /w)
endif()

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
