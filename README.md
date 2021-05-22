# Solas

A cross platform library for dynamic 2d lighting.

1. Library development
   1. [Windows Setup](#windows-setup-steps)
   2. [OSX Setup](#osx-setup-steps)
   3. [Scenario Viewer](#scenario-viewer)

## Windows setup steps

Use the following to set up a dev environment on Windows 10

### Initial setup (Windows)

Open command prompt and run `./scripts/windows/bootstrap.bat`

This will install vcpkg to your HOMEPATH (usually /Users/{username})

### Visual Studio

Ensure that cmake support is installed as part of Visual Studio so that it can automatically build the cmake project.

Open the project using either 'File > Open > Folder..' or 'Open local folder' from the Getting started screen

### Visual Studio Code (See below)

See below in Mac setup instructions

### Running tests in Visual Studio

- Install the "google test adapter" extension
- Switch to the x64-Test config
- Wait for build to complete (autoconfigure cmake must be enabled)
- Select and run the all_tests.exe target

A window should open with test results displayed inside

## OSX setup steps

Use the following to set up a dev environment on macOS (tested with Catalina)

### Initial setup (Mac)

Run the script `./scripts/osx/bootstrap.sh` to install the necessary dependencies.

This will allow you to run the other scripts:

- `./scripts/osx/debug.sh` - Build both SolasLib and ScenarioViewer and run the latter
- `./scripts/osx/release.sh` - Build SolasLib only as a .dylib
- `./scripts/osx/test.sh` - Build SolasLib only and run the tests

### Visual Studio Code

You can run the demo app directly from visual studio code if you follow these next steps

First install two plugins

- C/C++
- Cmake Tools

Next add the following to .vscode/settings.json

    "cmake.configureOnOpen": true,
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "cmake.configureArgs": [
        "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    ],
    "C_Cpp.default.includePath": [
        "~/vcpkg/installed/x64-osx/include",
        "./SolasLib/include",
        "./extern/googletest/googletest/include",
        "./extern/googletest/googlemock/include",
        "./extern/googlebenchmark/include"
    ],
    "C_Cpp.default.cppStandard": "c++17",
    "cmake.buildDirectory": "\${workspaceFolder}/build/vscode"

Finally, make sure to configure the IDE to use the correct compiler.

- Press cmd + shift + p to open the command pallette and enter "CMake: Select a kit"
- Select the appropriate compiler from the options provided (may require some trial and error to find the most optimized one)

## Scenario Viewer

This is a demo app that uses the solas API

- Use arrow keys left and right to switch between scenarios.
- Use WASD to move the placement cursor around
- Hold left shift to move around slowly for more precise placement
- Left click adds a new light
- Use 'L' key to toggle lighting on/off
- Use 'R' key to alter the light span
- Use 'P' to switch between Light placement and Wall placement
