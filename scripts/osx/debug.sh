set -x
mkdir -p build/debug
pushd build/debug
    cmake ../.. "-DCMAKE_BUILD_TYPE=Debug" "-DBUILD_TESTING=OFF" "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    cmake --build .
    pushd ScenarioViewer
        open ScenarioViewer.app
    popd
popd
