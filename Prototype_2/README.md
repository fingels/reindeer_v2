# Dependencies

```
sudo apt-get -y install cmake
sudo apt install doxygen
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

To build the documentation, after initial build, use
```
make documentation
firefox documentation/html/index.html
``̀

## Usage

The executable is a dummy code while the CMake is configured properly. Execute it while in `build` folder:

```
./app/reindeer2
```

### Acknowledgements

[Téo Lemane](https://github.com/tlemane) for the help with CMake and Doxygen.