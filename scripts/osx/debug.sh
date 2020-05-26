set -x
rm -rf build/*
pushd build
    cmake .. "-DCMAKE_BUILD_TYPE=Debug" "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    cmake --build .
    pushd ScenarioViewer
        open ScenarioViewer.app
    popd
popd
