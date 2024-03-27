cmake_minimum_required(VERSION 3.5.0)
project(gdsims VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(gdsimsapp main.cpp)
add_library(gdsimslib STATIC input.h Simulation.h Simulation.cpp random.h random.cpp constants.h Params.h Record.cpp Record.h
 Point.h BoundaryStrategy.h BoundaryStrategy.cpp Dispersal.h Dispersal.cpp GDRelease.h GDRelease.cpp
 Aestivation.h Aestivation.cpp Patch.h Patch.cpp Model.h Model.cpp)

find_library(GDSIMS_LIBRARY gdsimslib lib)
target_link_libraries(gdsimsapp LINK_PUBLIC ${GDSIMS_LIBRARY})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
