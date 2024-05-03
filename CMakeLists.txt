cmake_minimum_required(VERSION 3.5.0)
project(gdsims VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_library(gdsimslib STATIC Simulation.cpp random.cpp Record.cpp BoundaryStrategy.cpp Dispersal.cpp GDRelease.cpp Aestivation.cpp Patch.cpp Model.cpp Seasonality.cpp)

target_include_directories(gdsimslib PUBLIC .)
include_directories(.)

add_executable(gdsimsapp main.cpp)
target_include_directories(gdsimsapp PUBLIC .)

target_link_libraries(gdsimsapp gdsimslib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
