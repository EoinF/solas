find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
find_package(OpenGL REQUIRED)
# Need to manually install glm and set glm_DIR to the correct location
find_package(glm REQUIRED)

set(SOURCES
    ${SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/src/ScenarioViewer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ScenarioViewer.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Game.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Game.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Scenarios/Scenarios.h
    ${CMAKE_CURRENT_LIST_DIR}/src/Scenarios/Scenarios.cpp
)

set(CMAKE_CXX_STANDARD 17)

# Add source to this project's executable.
if (APPLE)
    add_executable (ScenarioViewer MACOSX_BUNDLE ${SOURCES})
else (APPLE)
    add_executable (ScenarioViewer ${SOURCES})
endif(APPLE)

if(UNIX AND NOT APPLE)
    SET(GLM_LIB glm::glm)
else()
    SET(GLM_LIB glm)
endif()

target_compile_features(ScenarioViewer
                    PRIVATE
                    cxx_std_17) 

target_compile_options(ScenarioViewer
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>)

target_link_libraries(ScenarioViewer PRIVATE OpenGL::GL)

target_link_libraries(ScenarioViewer
                      PRIVATE
                      sfml-graphics)
                      
target_link_libraries(ScenarioViewer
                      PRIVATE
		      ${GLM_LIB})

target_link_libraries(ScenarioViewer
                      PRIVATE
                      SolasLib)

add_custom_command(
  TARGET ScenarioViewer
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/assets/
    $<TARGET_FILE_DIR:ScenarioViewer>/assets/
)
