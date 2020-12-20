# Note: On later versions of macOS, keyboard input is disabled for certain programs
# It is advised to give keyboard permission to visual studio code and run the application from that instead

set -x
mkdir -p build/debug

BUILD_TYPE="Debug"
if [ "$1" = "--release" ]
then
    BUILD_TYPE="Release"
fi

pushd build/debug
    cmake ../.. "-DCMAKE_BUILD_TYPE=$BUILD_TYPE" "-DBUILD_TESTING=OFF" "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    cmake --build .
    pushd ScenarioViewer
        open ScenarioViewer.app
    popd
popd
