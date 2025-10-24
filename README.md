# RPL4
Raspberry Pi Peripheral Library 4


## Build

A convenience script to configure and build the project (including examples) is provided at the repository root: `build.sh`.

Common options:

- `-b DIR` : build directory (default `build`)
- `-t TYPE`: CMake build type `Release` or `Debug` (default `Release`)
- `-j N`   : parallel jobs (autodetected by default)
- `-e 0|1` : build examples (default `1`)
- `-l LVL` : RPL4 log level (DEBUG, INFO, WARNING, ERROR, FATAL, OFF) (default OFF)
