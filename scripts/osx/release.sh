set -x
mkdir -p build/release
pushd build/release
    cmake ../.. "-DBUILD_SCENARIO_VIEWER=OFF" "-DBUILD_TESTING=OFF" "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    cmake --build .
popd
