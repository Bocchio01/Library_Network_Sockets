add_library(core
    sock_core.cpp
)

target_include_directories(core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core PRIVATE
    libs
)