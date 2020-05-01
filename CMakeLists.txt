# CMakeList.txt : CMake project for Solas, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)
project ("Solas")

option(BUILD_SCENARIO_VIEWER "Build the scenario viewer as well" ON)

# Let's nicely support folders in IDE's
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	# add tests here maybe?
endif()

add_subdirectory(SolasLib)
if (BUILD_SCENARIO_VIEWER)
	add_subdirectory(ScenarioViewer)
endif(BUILD_SCENARIO_VIEWER)