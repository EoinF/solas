set -ex
mkdir -p build/test

TARGET="${1:-allTests}"
pushd build/test
    cmake ../.. "-DBUILD_SCENARIO_VIEWER=OFF" "-DBUILD_TESTING=ON" "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    cmake --build . --target $TARGET
    ./tests/$TARGET
popd
