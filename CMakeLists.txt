cmake_minimum_required(VERSION 3.5)

project(Colour_Parser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Colour_Parser main.cpp)

include(GNUInstallDirs)
install(TARGETS Colour_Parser
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
