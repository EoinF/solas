set -x
rm -rf build/*
pushd build
    BUILD_SCENARIO_VIEWER="ON" cmake .. "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    cmake --build .
    pushd ScenarioViewer
        open ScenarioViewer.app
    popd
popd
