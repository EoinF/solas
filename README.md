# Solas

A cross platform library for dynamic 2d lighting.

1. [OSX Setup](#osx-setup-steps)
2. [Example Usage](#example-usage)
3. [Scenario Viewer](#scenario-viewer)

## OSX setup steps

Use the following to set up a dev environment on macOS (tested with Catalina)

### Initial setup

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

Next add the following to settings.json

    "cmake.configureArgs": [
        "-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake"
    ],
    "C_Cpp.default.includePath": [
        "~/vcpkg/installed/x64-osx/include",
        "./SolasLib/include",
        "./extern/googletest/googletest/include",
        "./extern/googletest/googlemock/include"
    ]

## Example Usage

The ScenarioViewer sub project is an example project that directly uses the Solas API

TODO: Add example function calls

## Scenario Viewer

This is a demo app that uses the solas API

- Use arrow keys left and right to switch between scenarios.
- Use up and down to enable/disable lighting.
- Use 'L' key to toggle lighting on/off
