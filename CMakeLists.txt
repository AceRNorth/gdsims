cmake_minimum_required(VERSION 3.5.0)
project(gdsims VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

add_library(gdsimslib STATIC src/Simulation.cpp src/random.cpp src/Record.cpp src/BoundaryStrategy.cpp src/Dispersal.cpp src/GDRelease.cpp src/Aestivation.cpp src/Patch.cpp src/Model.cpp src/Seasonality.cpp)

target_include_directories(gdsimslib PUBLIC includes)
include_directories(includes)

add_executable(gdsimsapp src/main.cpp)
target_include_directories(gdsimsapp PUBLIC includes)

target_link_libraries(gdsimsapp PUBLIC gdsimslib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
