# ch2/part-1

Introduces _"Hello, ~~world~~ CMake!"_ application.

## Visual Studio Code Debugging

Open Visual Studio Code from `ch2/part-1`.

```bash
cd ch2/part-1
code .

# or

code ch2/part-1
```

## Commands

```bash
# start
cd ch2/part-1
# configure
cmake -B build -G "Ninja Multi-Config"
# build
cmake --build build
# run
./build/Debug/minimal-cmake
```
