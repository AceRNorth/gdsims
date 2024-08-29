cmake_minimum_required(VERSION 3.5.0)
project(gdsims VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Specify the C++ compiler
set(CMAKE_CXX_COMPILER "/usr/bin/g++-13")

add_executable(gdsimsapp src/main2.cpp)
add_library(gdsimslib STATIC src/Simulation.cpp src/random.cpp src/Record.cpp src/BoundaryStrategy.cpp src/Dispersal.cpp src/GDRelease.cpp src/Aestivation.cpp src/Patch.cpp src/Model.cpp src/Seasonality.cpp)

target_include_directories(gdsimslib PUBLIC includes)
include_directories(includes)

# Link the gdsimslib library directly using keyword signature
target_link_libraries(gdsimsapp PUBLIC gdsimslib)

# Link filesystem library if necessary using keyword signature
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_link_libraries(gdsimsapp PUBLIC stdc++fs)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
