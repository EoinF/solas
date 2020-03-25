# CMakeList.txt : CMake project for LightsDemo, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)
project ("LightsDemo")

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
# Need to manually install glm and set glm_DIR to the correct location
find_package(glm REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/src/LightsDemo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/LightsDemo.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Game.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Game.h
    ${CMAKE_CURRENT_LIST_DIR}/src/LightmapManager/LightmapManager.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Scenarios/Scenarios.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Scenarios/Scenarios.cpp
)

# Add source to this project's executable.
add_executable (LightsDemo ${SOURCES})

target_compile_features(LightsDemo
                        PRIVATE
                        cxx_std_17)

target_compile_options(LightsDemo
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>)
                       
target_link_libraries(LightsDemo
                      PRIVATE
                      sfml-graphics)

target_link_libraries(LightsDemo
                      PRIVATE
                      glm)

target_link_libraries(LightsDemo PRIVATE Boost::filesystem)

add_custom_command(
  TARGET LightsDemo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/assets/
    $<TARGET_FILE_DIR:LightsDemo>/assets/
)