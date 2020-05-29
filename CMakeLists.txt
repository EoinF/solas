# CMakeList.txt : CMake project for Solas, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)
project ("Solas")

option(BUILD_SCENARIO_VIEWER "Build the scenario viewer as well" ON)

# Let's nicely support folders in IDE's
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(SolasLib)
if (BUILD_SCENARIO_VIEWER)
	add_subdirectory(ScenarioViewer)
endif(BUILD_SCENARIO_VIEWER)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
