set -x
rm -rf build/*
pushd build
    cmake .. "-DBUILD_SCENARIO_VIEWER=OFF" "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    cmake --build .
popd
