find_package(glm REQUIRED)
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googlebenchmark" "extern/googlebenchmark")

set(SOURCES_BENCHMARK
    benchmarks/main.cpp
    benchmarks/startup.cpp
    benchmarks/addLight.cpp
    benchmarks/updateLight.cpp
    benchmarks/removeLight.cpp
)

set(SOURCES_SHARED testUtils.hpp)

set(SOURCES_BOUND_RAY_CAST
    BoundRayCast/addLight.cpp
    BoundRayCast/updateLight.cpp
    BoundRayCast/updateTile.cpp
    BoundRayCast/removeLight.cpp
    BoundRayCast/wallTile.cpp
    BoundRayCast/span.cpp
    BoundRayCast/direction.cpp
    BoundRayCast/brightness.cpp
)

mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

if(UNIX AND NOT APPLE)
    set(GLM_LIB glm::glm)
else()
    set(GLM_LIB glm)
endif()

macro(package_add_test TESTNAME)
    message (INFO ${ARGN})
    # create an exectuable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})

    set(CMAKE_CXX_STANDARD 20)

    target_compile_features(${TESTNAME}
                        PRIVATE
                        cxx_std_20)

    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} PRIVATE gtest gmock gtest_main SolasLib ${GLM_LIB})
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

package_add_test(allTests ${SOURCES_BOUND_RAY_CAST} ${SOURCES_SHARED})
package_add_test(boundRayCastTests ${SOURCES_BOUND_RAY_CAST} ${SOURCES_SHARED})

package_add_test(quickTest ${SOURCES_SHARED} BoundRayCast/updateTile.cpp)

add_executable(benchmarktest ${SOURCES_SHARED} ${SOURCES_BENCHMARK})

set(CMAKE_CXX_STANDARD 20)

target_compile_features(benchmarktest
                    PRIVATE
                    cxx_std_20)

target_link_libraries(benchmarktest PRIVATE gtest benchmark::benchmark SolasLib ${GLM_LIB})
