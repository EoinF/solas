set -x
mkdir -p build/test
pushd build/test
    cmake ../.. "-DBUILD_SCENARIO_VIEWER=OFF" "-DBUILD_TESTING=ON" "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    cmake --build .
    make test
popd
