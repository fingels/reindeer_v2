# Dependencies

For Cmake:
```
sudo apt-get -y install cmake
```

# Usage

## Build

If necessary, `mdir build` and then:

```
cd build
cmake ..
cmake --build .
```

To build with a specific configuration, run `cmake --build . --config <ConfigName>` instead. Meaningful options right now are Debug or Release.

## Usage

The executable is a dummy code while the CMake is configured properly. Execute it while in `build` folder:

```
./app/reindeer2
```

### Acknowledgements

[Téo Lemane](https://github.com/tlemane) for the help with CMake.