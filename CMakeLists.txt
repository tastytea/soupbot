cmake_minimum_required (VERSION 3.7)
include(GNUInstallDirs)

project (soupbot
         VERSION 0.2.1
         LANGUAGES CXX
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

# Write version in header
configure_file (
    "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
    "${PROJECT_BINARY_DIR}/version.hpp"
)
include_directories(${PROJECT_BINARY_DIR})

add_executable(soupbot src/soupbot.cpp)
target_link_libraries(soupbot mastodon-cpp boost_system boost_filesystem ssl crypto)
install(TARGETS soupbot DESTINATION ${CMAKE_INSTALL_BINDIR})
