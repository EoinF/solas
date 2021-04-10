find_package(glm REQUIRED)

# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")
set(HEADER_LIST
    ${CMAKE_CURRENT_LIST_DIR}/include/SolasLib/core.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/SolasLib/core/TileLightState.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/SolasLib/core/Light.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/SolasLib/core/LightmapManager.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/SolasLib/core/LightCaster.hpp
)

set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/RayCastUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/RayCastUtils.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/DiscreteLinePather.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/DiscreteLinePather.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/SimpleRayCast/SimpleRayCast.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/SimpleRayCast/SimpleRayCast.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/BoundRayCast/BoundRayCast.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/BoundRayCast/BoundRayCast.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/LightmapManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/SimpleRayCast/SimpleRayCast.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/BoundRayCast/BoundRayCast.hpp
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (BUILD_SCENARIO_VIEWER OR (CMAKE_BUILD_TYPE STREQUAL "DEBUG") OR (CMAKE_CONFIGURATION_TYPES STREQUAL "Debug"))
    # Make a .lib and .dll (Windows)
    # Make .lib (MacOS)
    add_library (SolasLib ${SOURCES} ${HEADER_LIST})
else()
    # Make just a .dll to release (Windows)
    # Make .dylib (MacOS)
    add_library (SolasLib SHARED ${SOURCES} ${HEADER_LIST})
endif()

# We need this directory, and users of our library will need it too
target_include_directories(SolasLib PUBLIC "./include")

if (APPLE)
    set(CMAKE_CXX_STANDARD 17)
else (APPLE)
    set_target_properties(SolasLib PROPERTIES LINKER_LANGUAGE cxx_std_17)
endif(APPLE)

target_compile_features(SolasLib
                    PRIVATE
                    cxx_std_17)

target_compile_options(SolasLib
                       PRIVATE
                    #    $<$<CXX_COMPILER_ID:AppleClang,Clang>:-O0>
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>)

target_link_libraries(SolasLib
                      PRIVATE
                      glm)
                      
# IDEs should put the headers in a nice place
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
