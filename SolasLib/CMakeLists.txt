# Note that headers are optional, and do not affect add_library, but they will not
# show up in IDEs unless they are listed in add_library.

find_package(glm REQUIRED)

# Optionally glob, but only for CMake 3.12 or later:
# file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${ModernCMakeExample_SOURCE_DIR}/include/modern/*.hpp")
set(HEADER_LIST 
    ${CMAKE_CURRENT_LIST_DIR}/include/SolasLib/core.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/SolasLib/core/TileLightState.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/SolasLib/core/Light.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/SolasLib/core/LightmapManager.hpp
    ${CMAKE_CURRENT_LIST_DIR}/include/SolasLib/SimpleRayCast/RayCast.hpp
)

set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/SimpleRayCast/SimpleRayCast.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SolasLib/SimpleRayCast/RayCast.cpp
)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (BUILD_SCENARIO_VIEWER)
    # Make a .lib and .dll for release
    add_library (SolasLib ${SOURCES} ${HEADER_LIST})
else(BUILD_SCENARIO_VIEWER)
    # Make just a .lib for debugging
    add_library (SolasLib SHARED ${SOURCES} ${HEADER_LIST})
endif (BUILD_SCENARIO_VIEWER)

# We need this directory, and users of our library will need it too
target_include_directories(SolasLib PUBLIC "./include")

set_target_properties(SolasLib PROPERTIES LINKER_LANGUAGE cxx_std_17)

target_compile_features(SolasLib
                        PRIVATE
                        cxx_std_17)

target_compile_options(SolasLib
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>)
                       
target_link_libraries(SolasLib
                      PRIVATE
                      glm)
                      
# IDEs should put the headers in a nice place
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADER_LIST})
