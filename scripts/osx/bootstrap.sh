git clone https://github.com/Microsoft/vcpkg ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
brew install sfml
~/vcpkg/vcpkg install glm
git submodule update --init --recursive