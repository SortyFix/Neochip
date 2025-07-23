# Neochip

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-orange.svg)](https://isocpp.org/)

CHIP-8 emulator/interpreter for C++.

## Installation

Clone the repository and compile the source code using CMake.

```bash
git clone https://github.com/your-username/Neochip.git
cd Neochip
cd build
cmake ..
make 
```

## Usage

To run Neochip with a CHIP-8 ROM, use the following command from the /build directory you created during the installation:

```bash
./neochip [file path to your ROM]
```

## Dependencies

- Requires SDL3 (https://www.libsdl.org/).

## License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).
