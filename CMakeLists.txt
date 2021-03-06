cmake_minimum_required (VERSION 3.7)
include(GNUInstallDirs)

project (soupbot
         VERSION 0.3.2
         LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(mastodonpp REQUIRED CONFIG)

# Write version in header
configure_file (
    "${PROJECT_SOURCE_DIR}/src/version.hpp.in"
    "${PROJECT_BINARY_DIR}/version.hpp"
)
include_directories(${PROJECT_BINARY_DIR})

add_executable(soupbot src/soupbot.cpp)
target_link_libraries(soupbot mastodonpp boost_system boost_filesystem ssl crypto)
install(TARGETS soupbot DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ingredients.json
        DESTINATION ${CMAKE_INSTALL_DATADIR}/soupbot)
