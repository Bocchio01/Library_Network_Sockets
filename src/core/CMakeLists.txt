add_library(core
    SocketCommon.cpp
    SocketCommon.hpp
    SocketCore.cpp
    SocketCore.hpp
    SocketError.cpp
    SocketError.hpp
)

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core PRIVATE
    libs
)