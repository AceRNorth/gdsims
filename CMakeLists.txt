cmake_minimum_required(VERSION 3.5.0)
project(cmake_gdsimsapp VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(cmake_gdsimsapp main.cpp)
add_library(cmake_gdsimsapp_lib STATIC input.h Simulation.h Simulation.cpp random.h random.cpp constants.h Params.h Record.cpp Record.h
 Point.h BoundaryStrategy.h BoundaryStrategy.cpp Dispersal.h Dispersal.cpp GDRelease.h GDRelease.cpp
 Aestivation.h Aestivation.cpp Patch.h Patch.cpp Model.h Model.cpp)

find_library(GDSIMS_LIBRARY cmake_gdsimsapp_lib lib)
target_link_libraries(cmake_gdsimsapp LINK_PUBLIC ${GDSIMS_LIBRARY})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
