git clone https://github.com/Microsoft/vcpkg ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
brew install sfml
~/vcpkg/vcpkg install glm

mkdir -p build/debug
mkdir -p build/release
mkdir -p build/test