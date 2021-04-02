set -x
mkdir -p build/test

TARGET="${1:-allTests}"
pushd build/test
    cmake ../.. "-DBUILD_SCENARIO_VIEWER=OFF" "-DBUILD_TESTING=ON" "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    cmake --build .
    make
    ./tests/$TARGET
popd
