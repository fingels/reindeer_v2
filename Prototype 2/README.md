# Dependencies

For Cmake:
```
sudo apt-get -y install cmake
```

# Usage

## Build

Go into the build folder

```
cd build
cmake ..
cmake --build . --clean-first
```

## Usage

Go into the app folder (still inside build folder). Right now the executable is a dummy code; while the CMake is configured.

```
cd app
./reindeer2
```