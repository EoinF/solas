git clone https://github.com/Microsoft/vcpkg %HOMEPATH%/vcpkg
%HOMEPATH%\vcpkg\bootstrap-vcpkg.bat
%HOMEPATH%\vcpkg\vcpkg install sfml:x64-windows
%HOMEPATH%\vcpkg\vcpkg install glm:x64-windows
git submodule update --init --recursive