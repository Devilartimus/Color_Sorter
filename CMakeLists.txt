cmake_minimum_required(VERSION 3.5)

project(Color_Sorter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(Color_Sorter
    include/ColoredObject.h
    include/ThreadManager.h
    include/utils.h
    src/main.cpp
    src/ThreadManager.cpp
    src/utils.cpp
)

include(GNUInstallDirs)
install(TARGETS Color_Sorter
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
